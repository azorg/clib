/*
 * Project: 'ini_t'
 * Version: 0.2b
 * File: "ini.h"
 * (C) 2007-2015 Alex Zorg <azorg@mail.ru>
 */

#ifndef INI_H
#define INI_H
//----------------------------------------------------------------------------
#include "str.h" // 'str_t'
//----------------------------------------------------------------------------
// inline macro (platform depended)
#ifndef INI_INLINE
#  ifdef __GNUC__
#    define INI_INLINE static inline
#  else
#    ifdef _WIN32 // Visual C++
#      define INI_INLINE __forceinline
#    else
#      define INI_INLINE inline
#    endif
#  endif
#endif // INI_INLINE
//----------------------------------------------------------------------------
// if "<Ident_1> = <Value 1>" style then INI_EQ defined
// if "<Ident_1> <Value 1>" style then INI_EQ not defined
#define INI_EQ "="
#define INI_EQ_LIST ":="

// comment separator
#define INI_COM ";#"

// line return
#define INI_LR "\n"
#define INI_LR_LIST "\n\r"

// spaces
#define INI_SP " "
#define INI_SP_LIST " \t"

// define section brackets
#define INI_LBR '['
#define INI_RBR ']'

// 'backslash' to code/decode strings
#define INI_BSLASH '\\'
#define INI_BSLASH_X 'x'
#define INI_BSLASH_PREF "\\x"

// sector size for data in memory (may be not defined)
#define INI_SECTOR_SIZE 2048
//----------------------------------------------------------------------------
typedef enum {INI_CLOSE = 0, INI_READ, INI_WRITE} ini_state_t;
//----------------------------------------------------------------------------
// `ini_t` type structure
typedef struct {
  str_t fname;       // file name
  str_t data;        // file data as string
  ini_state_t state; // current state (look enum ini_state_t)
} ini_t;
//----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
//----------------------------------------------------------------------------
// constructor and set file name
void ini_init_fname(ini_t *f, const char *fname);
//----------------------------------------------------------------------------
// default constructor
INI_INLINE void ini_init(ini_t *f) { ini_init_fname(f, ""); }
//----------------------------------------------------------------------------
// destructor
void ini_free(ini_t *f);
//----------------------------------------------------------------------------
// save to file if open for write or always (if save OK return 1, else 0)
STR_BOOL ini_save_ex(ini_t *f, STR_BOOL always_save);
//----------------------------------------------------------------------------
// save to file if open for write (if save OK return 1, else 0)
INI_INLINE STR_BOOL ini_save(ini_t *f)
{ return ini_save_ex(f, STR_BOOL_FALSE); }
//----------------------------------------------------------------------------
// get/set state
INI_INLINE ini_state_t ini_get_state(const ini_t *f) { return f->state; }
INI_INLINE void ini_set_state(ini_t *f, ini_state_t st) { f->state = st; }
//----------------------------------------------------------------------------
// load from string
void ini_load(ini_t *f, const char *text);
//----------------------------------------------------------------------------
// return as pointer to `str_t`
INI_INLINE const str_t *ini_str(const ini_t *f) { return &f->data; }
//----------------------------------------------------------------------------
// return as pointer to `char`
INI_INLINE const char *ini_cstr(const ini_t *f)
{ return str_c(&f->data); }
//----------------------------------------------------------------------------
// return file name
INI_INLINE const char *ini_get_fname(const ini_t *f)
{ return str_c(&f->fname); }
//----------------------------------------------------------------------------
// set file name
void ini_set_fname(ini_t *f, const char *fname);
//----------------------------------------------------------------------------
// check section
STR_BOOL ini_has_section(ini_t *f, const char *section);
//----------------------------------------------------------------------------
// check ident
STR_BOOL ini_has_ident(ini_t *f, const char *section, const char *ident);
//----------------------------------------------------------------------------
// read and write value "as-is" (main functions of `ini_t`)
str_t ini_read_value(ini_t *f,
                     const char *section, const char *ident,
                     const char *default_value);

void ini_write_value(ini_t *f,
                     const char *section, const char *ident,
                     const char *value);
//----------------------------------------------------------------------------
// code/decode strings to write/read to/from INI-file
str_t ini_code_str(const char *str);
str_t ini_decode_str(const char *str);
//----------------------------------------------------------------------------
// read and write value as string
str_t ini_read_str(ini_t *f,
                   const char *section, const char *ident,
                   const char *default_value);

void ini_write_str(ini_t *f,
                   const char *section, const char *ident,
                   const char *value);
//----------------------------------------------------------------------------
#ifdef STR_EXTRA
// read and write value as byte array
str_t ini_read_bin(ini_t *f,
                   const char *section, const char *ident,
                   const char *default_value, int default_size);

void ini_write_bin(ini_t *f,
                   const char *section, const char *ident,
                   const char *value, int size);
#endif // STR_EXTRA
//----------------------------------------------------------------------------
// read and write value as long
long ini_read_long(ini_t *f,
                   const char *section, const char *ident,
                   long default_value);

void ini_write_long(ini_t *f,
                    const char *section, const char *ident,
                    long value);
//----------------------------------------------------------------------------
#ifdef STR_INT64_DEF
// read and write value as int64 (HEX format)
STR_INT64 ini_read_int64(ini_t *f,
                         const char *section, const char *ident,
                         STR_INT64 default_value);

void ini_write_int64(ini_t *f,
                     const char *section, const char *ident,
                     STR_INT64 value);
#endif // STR_INT64_DEF
//----------------------------------------------------------------------------
#ifdef STR_FLOAT
// read and write value as float
float ini_read_float(ini_t *f,
                     const char *section, const char *ident,
                     float default_value);

void ini_write_float(ini_t *f,
                     const char *section, const char *ident,
                     float value);
//----------------------------------------------------------------------------
// read and write value as double
double ini_read_double(ini_t *f,
                       const char *section, const char *ident,
                       double default_value);

void ini_write_double(ini_t *f,
                      const char *section, const char *ident,
                      double value);
#endif // STR_FLOAT
//----------------------------------------------------------------------------
// read and write value as bool
STR_BOOL ini_read_bool(ini_t *f,
                       const char *section, const char *ident,
                       STR_BOOL default_value);

void ini_write_bool(ini_t *f,
                    const char *section, const char *ident,
                    STR_BOOL value);
//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus
//----------------------------------------------------------------------------
#endif // INI_H

/*** end of "ini.h" ***/

