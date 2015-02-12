/*
 * Project: C string type
 * Version: 0.2b
 * File: "str.h"
 * (C) 2007-2015 Alex Zorg <azorg@mail.com>
 */

#ifndef STR_H
#define STR_H
//----------------------------------------------------------------------------
#ifdef _WIN32 // use "deprecated" fopen(), vsnprintf() under Visual C++
#  define _CRT_SECURE_NO_DEPRECATE
#endif
//----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> // va_list, va_start, va_end, va_arg, vsnprintf()
//----------------------------------------------------------------------------
// inline macro (platform depended)
#ifndef STR_INLINE
#  ifdef __GNUC__
#    define STR_INLINE static inline
#  else
#    ifdef _WIN32 // Visual C++
#      define STR_INLINE __forceinline
#    else
#      define STR_INLINE inline
#    endif
#  endif
#endif // STR_INLINE
//----------------------------------------------------------------------------
// debug output
//#define STR_DEBUG
//#define STR_DEBUG_EXTRA

// if type 'int64' defined
#define STR_INT64_DEF

#ifdef STR_INT64_DEF
#  include <inttypes.h> // PRI*64, int64_t, uint64_t (<stdint.h>)
typedef int64_t  STR_INT64;
typedef uint64_t STR_UINT64;
//typedef __int64  STR_INT64;
//typedef __uint64 STR_UINT64;

// int64 printf() formats:
#  if defined(PRIi64) // portable PRIi64, PRIu64, PRIX64 defined
#    define STR_PRId64 PRIi64
#    define STR_PRIu64 PRIu64
#    define STR_PRIx64 PRIX64
#  elif defined(__MINGW32__) // MinGW32 (MinGW-w64)
#    warning "PRIi64 undefined, __MINGW32__ defined"
#    define STR_PRId64 "I64i"
#    define STR_PRIu64 "I64u"
#    define STR_PRIx64 "I64X"
#  elif defined(__BORLANDC__) // Borland C++ Builder
#    warning "PRIi64 undefined, __BORLANDC__ defined"
#    define STR_PRId64 "Li"
#    define STR_PRIu64 "Lu"
#    define STR_PRIx64 "LX"
#  else
#    warning "PRIi64 undefined"
#    define STR_PRId64 "lli"
#    define STR_PRIu64 "llu"
#    define STR_PRIx64 "llX"
#  endif
#endif // STR_INT64_DEF

// include some extra function
#define STR_EXTRA

// if type float and double exist
#define STR_FLOAT

// use <stdbool.h>
//#define STR_STDBOOL

// bool type
#ifdef STR_STDBOOL
#  include <stdbool.h>
#  define STR_BOOL bool
#  define STR_BOOL_TRUE  true
#  define STR_BOOL_FALSE false
#else
#  define STR_BOOL int
#  define STR_BOOL_TRUE  1
#  define STR_BOOL_FALSE 0
#endif

// keywords for bool strings and values
#define STR_BOOL_TRUE_STR  "true"
#define STR_BOOL_FALSE_STR "false"

// step of allocation of memory by default
#ifndef STR_DEF_SECTOR_SIZE
#  define STR_DEF_SECTOR_SIZE 512
#endif // STR_DEF_SECTOR_SIZE

// error character for str_at() function
#define STR_ERROR_CHAR '\0'

// string of default delimiters for "trim_xxx()" functions
#define STR_TRIM_DELIMITERS " \t\n\r\v"

// exception string for stderr if out of memory
#define STR_EXCEPTION_STRING "error in str.c: malloc(%u) return NULL!\n"

// fast read/write from/to file
#define STR_FAST_FILE_IO

// max item in str_sum() (or use str_malloc_mem())
#define STR_SUM_MAX_ITEM 16
//----------------------------------------------------------------------------
// choose one of your local 8-bit codepage
#define STR_KOI8R  // (+)
#undef  STR_CP1251 //
#undef  STR_CP866  //

#ifdef STR_KOI8R
#define STR_LOCAL_AL "ÁÂ×ÇÄÅ£ÖÚÉÊËÌÍÎÏÐÒÓÔÕÆÈÃÞÛÝßÙØÜÀÑ"
#define STR_LOCAL_AU "áâ÷çäå³öúéêëìíîïðòóôõæèãþûýÿùøüàñ"
#endif // STR_KOI8R

#ifdef STR_CP1251
#define STR_LOCAL_AL "àáâãäå¸æçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
#define STR_LOCAL_AU "ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß"
#endif // STR_CP1251

#ifdef STR_CP866
#define STR_LOCAL_AL " ¡¢£¤¥ñ¦§¨©ª«¬­®¯àáâãäåæçèéêëìíîï"
#define STR_LOCAL_AU "€‚ƒ„…ð†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ"
#endif // STR_CP866
//----------------------------------------------------------------------------
// `str_t` type structure
typedef struct {
  char *ptr;  // valid pointer to array of char if size!=0
  int size;   // current size of string
  int sector; // size of step of allocation of memory
} str_t;
//----------------------------------------------------------------------------
extern char _str_delim[]; // default trim delimiters
#ifdef STR_EXCEPTION_STRING
extern char _str_exstr[]; // exception string
#endif // STR_EXCEPTION_STRING
extern int  _str_def_sector; // default sector size
extern char _str_loc_al[];   // local alphabet low case
extern char _str_loc_au[];   // local alphabet upper case
#ifdef STR_DEBUG
extern unsigned _str_malloc_num;
extern unsigned _str_malloc_bytes;
#endif // STR_DEBUG
//----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
//----------------------------------------------------------------------------
// protected functions have '_' preffix
// memory allocate functions
STR_INLINE void* _str_malloc_mem(int size)
{
  void *ptr = malloc(size); // allocate memory
#if defined(STR_DEBUG) || defined(STR_EXCEPTION_STRING) 
  if (ptr != (void*) NULL)
  {
#  ifdef STR_DEBUG
    _str_malloc_num++;
#  endif // STR_DEBUG
  }
#  ifdef STR_EXCEPTION_STRING
  else
    fprintf(stderr, _str_exstr, size); // show exception
#  endif // STR_EXCEPTION_STRING
#endif // STR_DEBUG || STR_EXCEPTION_STRING 
  return ptr;
}
//----------------------------------------------------------------------------
STR_INLINE char* _str_malloc(int size)
{
  char *ptr = (char*) _str_malloc_mem(size); // allocate memory

#ifdef STR_DEBUG
  if (ptr != (char*) NULL)
    _str_malloc_bytes += size;
#endif // STR_DEBUG

#ifdef STR_DEBUG_EXTRA
  if (ptr != (char*) NULL)
    fprintf(stderr, "str.c: str_malloc_mem(%i)\n", size);
#endif // STR_DEBUG_EXTRA

  return ptr;
}
//----------------------------------------------------------------------------
// memory free functions
STR_INLINE void _str_free_mem(void *ptr)
{
  if (ptr != (void*) NULL)
  {
#ifdef STR_DEBUG
    _str_malloc_num--;
#endif // STR_DEBUG
    free(ptr);
  }
}
//----------------------------------------------------------------------------
void _str_create_size(str_t *s, int size); // size > 0
void _str_create_cstr(str_t *s, const char *src); // src != NULL
//----------------------------------------------------------------------------
#ifdef STR_DEBUG
STR_INLINE unsigned str_malloc_num()   { return _str_malloc_num;   }
STR_INLINE unsigned str_malloc_bytes() { return _str_malloc_bytes; } 
#endif // STR_DEBUG
//----------------------------------------------------------------------------
// destructor (free memory)
STR_INLINE void str_free(str_t *s)
{
  if (s->size != 0)
  {
#if defined(STR_DEBUG) || defined (STR_DEBUG_EXTRA)
    int blk = (s->size + s->sector) / s->sector;
#endif

#ifdef STR_DEBUG
    _str_malloc_bytes -= s->sector * blk;
#endif // STR_DEBUG

    _str_free_mem((void*) s->ptr);

#ifdef STR_DEBUG_EXTRA
    if (s->ptr != (char*) NULL)
      fprintf(stderr, "str.c: str_free_mem(%i)\n", s->sector * blk);
#endif // STR_DEBUG_EXTRA
  }
}
//----------------------------------------------------------------------------
// constructors
STR_INLINE void str_init_zero(str_t *s)
{ s->sector = _str_def_sector; s->size = 0; s->ptr = (char*) NULL; }
//----------------------------------------------------------------------------
STR_INLINE void str_init_chars(str_t *s, int size, char chr)
{
  int i;
  s->sector = _str_def_sector;
  _str_create_size(s, size);
  for (i = 0; i < size; i++) s->ptr[i] = chr;
}
//----------------------------------------------------------------------------
void str_init_size_cstr(str_t *s, int size, const char *src);
void str_init_cstr(str_t *s, const char *src);
void str_init_str(str_t *s, const str_t *src);
void str_init_char(str_t *s, char src);
void str_init_vsprintf(str_t *s, const char *fmt, va_list ap);
//----------------------------------------------------------------------------
STR_INLINE void str_init_sprintf(str_t *s, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  str_init_vsprintf(s, fmt, ap);
  va_end(ap);
}
//----------------------------------------------------------------------------
STR_INLINE void str_init_short(str_t *s, short src)
{ str_init_sprintf(s, "%hd", src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_ushort(str_t *s, unsigned short src)
{ str_init_sprintf(s, "%hu", src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_int(str_t *s, int src)
{ str_init_sprintf(s, "%i", src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_uint(str_t *s, unsigned int src)
{ str_init_sprintf(s, "%u", src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_int_hex(str_t *s, int src)
{ str_init_sprintf(s, "%X", src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_long(str_t *s, long src)
{ str_init_sprintf(s, "%ld", src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_ulong(str_t *s, unsigned long src)
{ str_init_sprintf(s, "%lu", src); }
//----------------------------------------------------------------------------
#ifdef STR_INT64_DEF
STR_INLINE void str_init_int64(str_t *s, STR_INT64 src)
{ str_init_sprintf(s, "%" STR_PRId64, src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_uint64(str_t *s, STR_UINT64 src)
{ str_init_sprintf(s, "%" STR_PRIu64, src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_uint64_hex(str_t *s, STR_UINT64 src)
{ str_init_sprintf(s, "%" STR_PRIx64, src); }
#endif // STR_INT64_DEF
//----------------------------------------------------------------------------
#ifdef STR_FLOAT
STR_INLINE void str_init_float(str_t *s, float src)
{ str_init_sprintf(s, "%.8g", src); }
//----------------------------------------------------------------------------
STR_INLINE void str_init_double(str_t *s, double src)
{ str_init_sprintf(s, "%.17g", src); }
#endif // STR_FLOAT
//----------------------------------------------------------------------------
void str_init_bool(str_t *s, STR_BOOL src);
//----------------------------------------------------------------------------
// creators
STR_INLINE str_t str_zero()
{ str_t s; str_init_zero(&s); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_chars(int size, char chr)
{ str_t s; str_init_chars(&s, size, chr); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_size_cstr(int size, const char *src)
{ str_t s; str_init_size_cstr(&s, size, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_cstr(const char *src)
{ str_t s; str_init_cstr(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_str(const str_t *src)
{ str_t s; str_init_str(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_char(char src)
{ str_t s; str_init_char(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_short(short src)
{ str_t s; str_init_short(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_ushort(unsigned short src)
{ str_t s; str_init_ushort(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_int(int src)
{ str_t s; str_init_int(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_uint(unsigned int src)
{ str_t s; str_init_uint(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_int_hex(int src)
{ str_t s; str_init_int_hex(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_long(long src)
{ str_t s; str_init_long(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_ulong(unsigned long src)
{ str_t s; str_init_ulong(&s, src); return s; }
//----------------------------------------------------------------------------
#ifdef STR_INT64_DEF
STR_INLINE str_t str_int64(STR_INT64 src)
{ str_t s; str_init_int64(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_uint64(STR_UINT64 src)
{ str_t s; str_init_uint64(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_uint64_hex(STR_UINT64 src)
{ str_t s; str_init_uint64_hex(&s, src); return s; }
#endif // STR_INT64_DEF
//----------------------------------------------------------------------------
#ifdef STR_FLOAT
STR_INLINE str_t str_float(float src)
{ str_t s; str_init_float(&s, src); return s; }
//----------------------------------------------------------------------------
STR_INLINE str_t str_double(double src)
{ str_t s; str_init_double(&s, src); return s; }
#endif // STR_FLOAT
//----------------------------------------------------------------------------
STR_INLINE str_t str_bool(STR_BOOL src)
{ str_t s; str_init_bool(&s, src); return s; }
//----------------------------------------------------------------------------
// additional creators
str_t str_hex(unsigned long value, int digits);
str_t str_bin(unsigned long value, int digits);
str_t str_oct(unsigned long value, int digits);
str_t str_dec(unsigned long value, int digits);
//----------------------------------------------------------------------------
// comparison
STR_BOOL str_is_equal(const str_t *s1, const str_t *s2);
STR_BOOL str_is_equal_cstr(const str_t *s1, const char *s2);
//----------------------------------------------------------------------------
// transformations
long str_to_long_cstyle(const str_t *s, long def_val, unsigned char base);
//----------------------------------------------------------------------------
#ifdef STR_INT64_DEF
STR_INT64 str_to_int64(const str_t *s, STR_INT64 def_val, unsigned char base);
#endif // STR_INT64_DEF
//----------------------------------------------------------------------------
long str_to_long_ex(const str_t *s, long def_val, unsigned char base);
//----------------------------------------------------------------------------
STR_INLINE long str_to_long_def(const str_t *s, long def_val)
{ return str_to_long_ex(s, def_val, 10); }
//----------------------------------------------------------------------------
STR_INLINE long str_to_long(const str_t *s)
{ return str_to_long_def(s, 0); }
//----------------------------------------------------------------------------
STR_INLINE int str_to_int_def(const str_t *s, int def_val)
{ return (int) str_to_long_ex(s, (long) def_val, 10); }
//----------------------------------------------------------------------------
STR_INLINE int str_to_int(const str_t *s)
{ return str_to_int_def(s, 0); }
//----------------------------------------------------------------------------
#ifdef STR_FLOAT
STR_INLINE double str_to_double(const str_t *s)
{
  if (s->size == 0) return 0.;
  return atof(s->ptr);
}
//----------------------------------------------------------------------------
STR_INLINE double str_to_float(const str_t *s)
{ return (float) str_to_double(s); }
#endif // STR_FLOAT
//----------------------------------------------------------------------------
STR_BOOL str_to_bool(const str_t *s);
//----------------------------------------------------------------------------
// assignments
void str_assign(str_t *dst, const str_t *src);
void str_assign_cstr(str_t *dst, const char *src);
//----------------------------------------------------------------------------
// add to end
void str_add(str_t *s, const str_t *add);
void str_add_cstr(str_t *s, const char *add);
void str_add_char(str_t *s, char add);
//----------------------------------------------------------------------------
// concatenation
// fmt: S - str_t*, s - char*, c - char, i - int, u - uint, x - hex,
//      f - float, d - double, I - int64, U - uint64, X - uint64-HEX, ...
str_t str_sum(const char *fmt, ...);
//----------------------------------------------------------------------------
// vsprintf-like concatenation
STR_INLINE str_t str_vsprintf(const char *fmt, va_list ap)
{ str_t s; str_init_vsprintf(&s, fmt, ap); return s; }
//----------------------------------------------------------------------------
// sprintf-like concatenation
STR_INLINE str_t str_sprintf(const char *fmt, ...)
{
  str_t s;
  va_list ap;
  va_start(ap, fmt);
  str_init_vsprintf(&s, fmt, ap);
  va_end(ap);
  return s;
}
//----------------------------------------------------------------------------
// accessing character by specified index
char *str_at(const str_t *s, int index);
//----------------------------------------------------------------------------
// convert to "C" string
const char *str_c(const str_t *s);
//----------------------------------------------------------------------------
STR_INLINE char *str_last_char(const str_t *s)
{ return str_at(s, s->size - 1); }
//----------------------------------------------------------------------------
STR_INLINE char *str_ptr(const str_t *s)
{ return (s->size != 0) ? s->ptr : (char*) NULL; }
//----------------------------------------------------------------------------
STR_INLINE const void *str_data(const str_t *s)
{ return (s->size != 0) ? s->ptr : NULL; }
//----------------------------------------------------------------------------
// tools
str_t str_substr(const str_t *s, int index, int count);
int str_strpos(const str_t *s, const str_t *substr, int index);
int str_chrpos(const str_t *s, char subchr, int index);
//----------------------------------------------------------------------------
STR_INLINE int str_find_first(const str_t *s, const str_t *substr)
{ return str_strpos(s, substr, 0); }
//----------------------------------------------------------------------------
int str_find_last(const str_t *s, const str_t *substr);
//----------------------------------------------------------------------------
int str_find_one_of(
  const str_t *s,   // find in this string
  const char *cstr, // find first one of char from this C string
  int index,        // begin from this index
  int dir,          // direction <0 or >=0
  int count         // maximum count of search (<0 - unlimited)
); // return -1 if can't find
//----------------------------------------------------------------------------
int str_skip_for(
  const str_t *s,   // find in this string
  const char *cstr, // skip any char from this C string
  int index,        // begin from this index
  int dir,          // direction <0 or >=0
  int count         // maximum count of search (<0 - unlimited)
); // return -1 if skip all
//----------------------------------------------------------------------------
str_t str_lower_case(const str_t *s);
str_t str_upper_case(const str_t *s);
str_t str_trim_ex(const str_t *s, const char *delim);
//----------------------------------------------------------------------------
STR_INLINE str_t str_trim(const str_t *s)
{ return str_trim_ex(s, _str_delim); }
//----------------------------------------------------------------------------
str_t str_trim_left_ex(const str_t *s, const char *delim);
//----------------------------------------------------------------------------
STR_INLINE str_t str_trim_left(const str_t *s)
{ return str_trim_left_ex(s, _str_delim); }
//----------------------------------------------------------------------------
str_t str_trim_right_ex(const str_t *s, const char *delim);
//----------------------------------------------------------------------------
STR_INLINE str_t str_trim_right(const str_t *s)
{ return str_trim_right_ex(s, _str_delim); }
//----------------------------------------------------------------------------
void str_swap(str_t *s1, str_t *s2);
//----------------------------------------------------------------------------
// modify string
void str_set_size(str_t *s, int new_size);
void str_insert(str_t *s, const str_t *str, int index);
void str_insert_cstr(str_t *s, const char *cstr, int index);
void str_insert_char(str_t *s, char chr, int index);
void str_delete(str_t *s, int index, int count);
int str_replace(str_t *s, const str_t *old_substr, const str_t *new_substr);
//----------------------------------------------------------------------------
STR_INLINE void str_clear(str_t *s)
{ if (s->size != 0) { str_free(s); s->size = 0; } }
//----------------------------------------------------------------------------
void str_fill(str_t *s, char chr, int index, int count);
void str_set_sector(str_t *s, int new_sector);
//----------------------------------------------------------------------------
STR_INLINE void str_set_def_sector(int def_sector)
{ _str_def_sector = def_sector > 0 ? def_sector : 1; }
//----------------------------------------------------------------------------
STR_INLINE int str_get_def_sector() { return _str_def_sector; }
//----------------------------------------------------------------------------
// scrolling
void str_rol(str_t *s, int count); // left scrolling
void str_ror(str_t *s, int count); // right scrolling
//----------------------------------------------------------------------------
// query attributes of string
STR_INLINE int str_size      (const str_t *s) { return s->size; }
STR_INLINE int str_get_size  (const str_t *s) { return s->size; }
STR_INLINE int str_get_sector(const str_t *s) { return s->sector; }
//----------------------------------------------------------------------------
// multiplications
str_t str_mul(const str_t *str, int times);
//----------------------------------------------------------------------------
// additions
STR_INLINE str_t str_string_of_char(char chr, int count)
{ return str_chars(count, chr); }
//----------------------------------------------------------------------------
STR_INLINE str_t str_long2str(long value){ return str_long(value); }
STR_INLINE str_t str_int2str(int value){ return str_int(value); }
//----------------------------------------------------------------------------
#ifdef STR_EXTRA
// convert binary array to hex string (like "01:23:AB:CD")
str_t str_bin2hex(const char *bin, int size);
//----------------------------------------------------------------------------
// convert hex string (like "01:23:AB:CD") to binary array
str_t str_hex2bin(const char *str);
#endif // STR_EXTRA
//----------------------------------------------------------------------------
#ifdef STR_FLOAT
STR_INLINE str_t str_float2str(float value){ return str_float(value); }
STR_INLINE str_t str_double2str(double value){ return str_double(value); }
#endif // STR_FLOAT
//----------------------------------------------------------------------------
// read from file & write to text file
int str_read_from_file(str_t *s, const char *fname, int add_flag);
int str_write_to_file(const str_t *s, const char *fname, int add_flag);
//----------------------------------------------------------------------------
// read from file & write to binary file
int str_read_from_bin_file(str_t *s, const char *fname, int add_flag);
int str_write_to_bin_file(const str_t *s, const char *fname, int add_flag);
//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus
//----------------------------------------------------------------------------
#endif // STR_H

/*** end of "str.h" ***/

