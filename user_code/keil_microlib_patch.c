#include <wchar.h>
#include <time.h>
#include "tkc/wstr.h"
#include "tkc/time_now.h"

size_t wcslen(const wchar_t* s) {
  return wcs_len(s);
}

int wcscmp(const wchar_t* s1, const wchar_t* s2) {
  return wcs_cmp(s1, s2);
}

wchar_t* wcscpy(wchar_t* s1, const wchar_t* s2) {
  return wcs_cpy(s1, s2);
}

wchar_t* wcschr(const wchar_t* s, wchar_t c) {
  return (wchar_t*)wcs_chr(s, c);
}

wchar_t* wcsdup(const wchar_t* s) {
  return wcs_dup(s);
}

int iswspace(wint_t wc) {
  return isspace(wc);
}

time_t time(time_t* t) {
  return time_now_s();
}
