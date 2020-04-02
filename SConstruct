import os
import tempfile
import sys
import subprocess

AWTK_ROOT ='./output/awtk'
AWTK_LIB_NAME=''


SetOption('num_jobs', 4)
print("running with -j %s"%GetOption('num_jobs'))

###########################################################################################################
def run_external_cmd_and_get_stderr_line_list(cmd_argv_list,error_str,cwd=None):
    if None == cwd:
        p = subprocess.Popen(cmd_argv_list,stderr=subprocess.PIPE)
    else :
        p = subprocess.Popen(cmd_argv_list,cwd = cwd,stderr=subprocess.PIPE)
    p.wait()
    sys.stdout.flush();
    if p.returncode != 0:
        error_exit(error_str);
    sz_lines_temp = p.stderr.readlines();
    sz_lines = [];
    for sz in sz_lines_temp:
        sz_lines.append(sz.decode(sys.getdefaultencoding() ) );
    return sz_lines;

def windows_cmd_str_to_cmd_list(cmd_str):
    maohao_count = 0;
    cmd_list = [];
    sz = '';

    for c in cmd_str:
        if c == ' ':
            if 1 == maohao_count:
                sz += c;
                continue;
            else:
                if len(sz) != 0:
                    cmd_list.append(sz);
                    sz = '';

        elif c == r'"':
            maohao_count = maohao_count + 1;
            if 2 == maohao_count:
                maohao_count = 0;
                if len(sz) != 0:
                    cmd_list.append(sz);
                    sz = '';
        else:
            sz += c;
    if len(sz) != 0:
        cmd_list.append(sz);
        sz = '';

    if 1 == maohao_count:
        error_exit("error")
    return cmd_list;


def mergelib_function(target, source, env):
    AR_CMD = env['AR']
    CUR_DIR = os.getcwd()

    if len(target) != 1 :
        error_exit("target must be only one")
    target = str(target[0]);
    # delete abspath to avoid abspath include + char, result in ar cmd syntax error
    #target = os.path.abspath(target);

    ar_flags_list = windows_cmd_str_to_cmd_list(env['RANLIBFLAGS'])

    
    fd, tmp_ar_name = tempfile.mkstemp()

    file = os.fdopen(fd, 'w+')
    file.write('create ' + target)

    print('target: ' + target)
    for src in source :
        src_file_path = str(src);
        if os.path.isfile(src_file_path) :
            print('add ' + src_file_path)
            file.write('\naddlib ' + src_file_path)

    file.write('\nsave')
    file.write('\nend')

    
    ar_cmd_list = [AR_CMD, '-M',]
    ar_cmd_list.extend(ar_flags_list)
    if isinstance(target ,bytes):
        target = target.decode(sys.getdefaultencoding())

    ar_cmd_list.append(target)
    ar_cmd_list = tuple(ar_cmd_list)

    file.seek(0)
    ps = subprocess.Popen(ar_cmd_list, stdin=file)
    ps.wait()
    if ps.returncode != 0:
        error_exit(target + ' failed.')

    file.close()

    while True:
        try:
            os.remove(tmp_ar_name)
            break;
        except :
            print( 'error rm;')
            time.sleep(1)

    return None
  

def tool_add_merge_lib(env):
    tool_merge_lib = Builder(action=mergelib_function)
    env.Append(BUILDERS = {'merge_lib' : tool_merge_lib})

def mk_gcc_environment(gcc_prefix):
    env = DefaultEnvironment(tools = ['as','gcc','g++','ar','gnulink'],
        ENV = {'PATH' : os.environ['PATH']},
        CC= gcc_prefix + 'gcc',
        AR= gcc_prefix + 'ar',
        RANLIB= gcc_prefix + 'ranlib',
        AS= gcc_prefix + 'gcc -c',
        CXX= gcc_prefix + 'g++',
        LINK=gcc_prefix + 'gcc',)

    if ARGUMENTS.get('VERBOSE') != "1" :
        env['CCCOMSTR'] = u"GCC $SOURCE"
        env['CXXCOMSTR'] = u"G++ $SOURCE"
        env['ARCOMSTR'] = u"AR $TARGET"        

    CPU_FLAGS =  ' '
    CPU_FLAGS += ' -mcpu='+'cortex-m7'
    CPU_FLAGS += ' -m' + 'thumb'
    CPU_FLAGS += ' -mthumb-interwork '
    CPU_FLAGS += ' -mfloat-abi=' + 'hard'
    CPU_FLAGS += ' -mfpu=' + 'fpv5-d16'
    CPU_FLAGS += ' -mno-unaligned-access'
    #CPU_FLAGS += ' -fno-short-enums'
    ###########################################################################################################
    DEBUG_FLAGS = ' -g3 -gdwarf-2 '
    ###########################################################################################################
    __build_type_debug_opt_flags = ('-O0','-O3','-Os','-O2')
    __permit_build_type=('Debug','Release','MinSizeRel','RelWithDebInfo')
    BUILD_TYPE = 'Release'
    OPT_FLAGS = __build_type_debug_opt_flags[__permit_build_type.index(BUILD_TYPE)];
    OPT_FLAGS += ' -ffunction-sections -fdata-sections ';
    ###########################################################################################################
    OTHER_FLAGS = ' -fmessage-length=0 -MMD -MP '
    ###########################################################################################################
    TEMP = ' '
    AWTK_CFLAGS = TEMP.join((CPU_FLAGS,DEBUG_FLAGS,OPT_FLAGS,OTHER_FLAGS))
    env['CFLAGS'] = AWTK_CFLAGS

    env['CXXFLAGS'] =AWTK_CFLAGS

    tool_add_merge_lib(env)
    env['ARFLAGS']     = 'rcPv '
    env['RANLIBFLAGS'] = ''
    #tool_set_arm_none_eabi_ar_with_lto_plugin(env)

    return env

def mk_armcc_environment():
  env = DefaultEnvironment(
    tools = ['as','cc','cc','ar','link'],
    ENV = {'PATH' : os.environ['PATH']},
    CC='armcc',
    AR='armar',
    LIBPREFIX='',
    LIBSUFFIX=".lib",
    CXX='armcc',    
    ARFLAGS = '-r -c -s --create',
    OBJSUFFIX = '.o',
    CXXCOM = '$CXX -o $TARGET -c $CXXFLAGS $CCFLAGS $_CCCOMCOM $SOURCES',)
  
  if ARGUMENTS.get('VERBOSE') != "1" :
    env['CCCOMSTR'] = u"CC $SOURCE"
    env['CXXCOMSTR'] = u"CXX $SOURCE"
    env['ARCOMSTR'] = u"AR $TARGET"  

    env['CFLAGS'] = '--c99'
    env['CCFLAGS'] = '--cpu=Cortex-M7.fp.dp --apcs=interwork --gnu -O3 -Ospace'
    env['CXXFLAGS'] = '--cpp'

    import SCons.Tool
    import SCons.Defaults

    static_obj, shared_obj = SCons.Tool.createObjBuilders(env)
    suffix = '.cpp'
    static_obj.add_action(suffix, SCons.Defaults.CXXAction)

    return env

###########################################################################################################
if ARGUMENTS.get('COMPILER_TOOLS') == 'gcc':
  env = mk_gcc_environment(ARGUMENTS.get('GCC_PREFIX'))
elif ARGUMENTS.get('COMPILER_TOOLS') == 'armcc':
  env = mk_armcc_environment()

AWTK_LIB_NAME =ARGUMENTS.get('AWTK_LIB_NAME')



env['CPPDEFINES'] = ['HAS_AWTK_CONFIG', '__BUILDING_AWTK_LIB__']

AWTK_COMMON_INCLUDE=['./output/awtk/src']
AWTK_COMMON_INCLUDE.append( './output/awtk/src/ext_widgets' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd/agg/include' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd/agge' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd/cairo' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd/nanovg' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd/nanovg/base' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd/gpinyin/include' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd/libunibreak' )
AWTK_COMMON_INCLUDE.append( './output/awtk/3rd/pixman' )
AWTK_COMMON_INCLUDE.append( './output/awtk-port' )

env['CPPPATH'] = AWTK_COMMON_INCLUDE

def get_source_file(path,list):
  files = os.listdir(path)
  for file in files:
    f = os.path.join(path,file)
    if os.path.isfile(f):
      if file.endswith('.c') or file.endswith('.cpp'):
      #if file.endswith('.cpp'):
        list.append(f)
    if os.path.isdir(f):
      get_source_file(f,list) 

__all_file =[]
get_source_file(AWTK_ROOT,__all_file)     


all_lib =[]
__source =[]
for i in range(0,len(__all_file),100):
    __source = __all_file[i:i+100]
    all_lib += env.StaticLibrary(__source)

if ARGUMENTS.get('COMPILER_TOOLS') == 'gcc':
  env.merge_lib(AWTK_LIB_NAME,all_lib)
if ARGUMENTS.get('COMPILER_TOOLS') == 'armcc':
  env.StaticLibrary(AWTK_LIB_NAME,all_lib)
