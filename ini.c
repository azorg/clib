/*
 * Project: 'ini_t'
 * Version: 0.2b
 * File: "ini.c"
 * (C) 2007-2015 Alex Zorg <azorg@mail.ru>
 */

//----------------------------------------------------------------------------
#include "ini.h"
//----------------------------------------------------------------------------
static STR_BOOL _ini_open_rd(ini_t *f) // if OK return 1, else 0
{
  if (f->state == INI_CLOSE)
  { // file is not at memory yet (f->data is empty)
    if (str_size(&f->fname) == 0)
      return STR_BOOL_FALSE; // work in memory only
    if (!str_read_from_bin_file(&f->data, str_c(&f->fname), 0))
      return STR_BOOL_FALSE; // can't read file
    f->state = INI_READ;
  }
  return STR_BOOL_TRUE; // read from file successful
}
//----------------------------------------------------------------------------
static void _ini_open_wr(ini_t *f)
{
  if (f->state != INI_WRITE)
  { // file is not modified
    _ini_open_rd(f); // read file to memory if needed
    f->state = INI_WRITE;
  }
}
//----------------------------------------------------------------------------
void ini_init_fname(ini_t *f, const char *fname)
{
  str_init_cstr(&f->fname, fname);
  str_init_zero(&f->data);
#ifdef INI_SECTOR_SIZE
  str_set_sector(&f->data, INI_SECTOR_SIZE);
#endif
  f->state = INI_CLOSE;
}
//----------------------------------------------------------------------------
void ini_free(ini_t *f)
{
  ini_save(f);
  str_free(&f->data);
  str_free(&f->fname);
}
//----------------------------------------------------------------------------
STR_BOOL ini_save_ex(ini_t *f, STR_BOOL always_save)
{
  if (f->state == INI_WRITE || always_save)
  { // file is modified
    if (str_size(&f->fname) == 0)
      return STR_BOOL_FALSE; // can't save to file
    if (!str_write_to_bin_file(&f->data, str_c(&f->fname), 0))
      return STR_BOOL_FALSE; // can't save to file
    f->state = INI_READ;
  }
  return STR_BOOL_TRUE; // write to file successful
}
//----------------------------------------------------------------------------
void ini_load(ini_t *f, const char *data)
{
  f->state = INI_WRITE;
  str_assign_cstr(&f->data, data);
}
//----------------------------------------------------------------------------
void ini_set_fname(ini_t *f, const char *fname)
{
  ini_save(f);
  f->state = INI_CLOSE;
  str_assign_cstr(&f->fname, fname);
}
//----------------------------------------------------------------------------
static int _ini_find_section(const ini_t *f, const char *section)
{
  int i, j;
  str_t h, h1, h2, h3;
  
  // trim and upper case section
  h = str_cstr(section);
  h1 = str_trim(&h);
  str_free(&h);
  h = str_upper_case(&h1);
  str_free(&h1);
  
  // find section
  i = 0; // begin index
  if (str_size(&h) != 0)
  {
    h1 = str_sum("sc", INI_LR_LIST INI_COM, INI_RBR);
    // h1 = "\n\r;#]"
    
    for (; i != -1;
         i = str_find_one_of(&f->data, INI_LR_LIST, i, 0, -1))
    {
      i = str_skip_for(&f->data, INI_SP_LIST INI_LR_LIST, i, 0, -1);
      if (i == -1) break;
      if (*str_at(&f->data, i) != INI_LBR) continue;
      j = str_find_one_of(&f->data, str_c(&h1), ++i, 0, -1);
      if (j == -1)
      {
        i = -1;
        break;
      } 
      if (*str_at(&f->data, j) != INI_RBR)
        continue;
      
      h2 = str_substr(&f->data, i, j - i);
      h3 = str_trim(&h2);
      str_free(&h2);
      h2 = str_upper_case(&h3);
      str_free(&h3);
      
      if (str_is_equal(&h, &h2))
      { // section found
        str_free(&h2);
        i = str_find_one_of(&f->data, INI_LR_LIST, j + 1, 0, -1);
        if (i == -1) 
          i = str_size(&f->data);
        break;
      }

      // not found
      str_free(&h2);
    } // for
    str_free(&h1);
  } // if
  str_free(&h);
  return i; // return index of section data or -1
}
//----------------------------------------------------------------------------
static int _ini_find_ident(const ini_t *f, const char *ident, int i, int *end)
{
  int j;
  str_t h, h1, h2;
  
  // trim and upper case ident
  h = str_cstr(ident);
  h1 = str_trim(&h);
  str_free(&h);
  h = str_upper_case(&h1);
  str_free(&h1);
  
  // find ident
  for (*end = i; i != -1;
       i = str_find_one_of(&f->data, INI_LR_LIST, i, 0, -1))
  {
    i = str_skip_for(&f->data, INI_SP_LIST INI_LR_LIST,
                     *end = i, 0, -1);
    if (i == -1 || *str_at(&f->data, i) == INI_LBR)
      break; // can't find ident
    j = str_find_one_of(
          &f->data,
#ifdef INI_EQ
          INI_EQ_LIST INI_LR_LIST INI_COM, // remove INI_SP by 2015.01.26
#else
          INI_EQ_LIST INI_LR_LIST INI_COM INI_SP,
#endif
          i + 1, 0, -1);
    if (j == -1) j = str_size(&f->data);
    
    h1 = str_substr(&f->data, i, j - i);
    h2 = str_trim(&h1);
    str_free(&h1);
    h1 = str_upper_case(&h2);
    str_free(&h2);
    
    if (str_is_equal(&h, &h1))
    { // ident found
      str_free(&h1);
      str_free(&h);
      return j; // return index
    }
    str_free(&h1);
  } // for
  str_free(&h);
  return -1; // ident not found
}
//----------------------------------------------------------------------------
static int _ini_add_section(ini_t *f, const char *section)
{
  str_t h, h1;
  
  // trim section
  h1 = str_cstr(section);
  h = str_trim(&h1);
  str_free(&h1);

  if (str_size(&f->data) != 0)
  { // check  end of file
    int i = str_skip_for(&f->data, INI_LR_LIST, str_size(&f->data) - 1,
                         -1, -1);
    if (i == -1) // file hasn't any '\n' or '\r'
      str_add_cstr(&f->data, INI_LR);
  }
  h1 = str_sum("cScs", INI_LBR, &h, INI_RBR, INI_LR);
  str_add(&f->data, &h1);
  str_free(&h1);
  str_free(&h);
  return str_size(&f->data) - 1;
}
//----------------------------------------------------------------------------
// check section
STR_BOOL ini_has_section(ini_t *f, const char *section)
{
  if (!_ini_open_rd(f))
    return STR_BOOL_FALSE; // can't open file

  if (_ini_find_section(f, section) < 0)
    return STR_BOOL_FALSE; // can't find section

  return STR_BOOL_TRUE; // section exist
}
//----------------------------------------------------------------------------
// check ident
STR_BOOL ini_has_ident(ini_t *f, const char *section, const char *ident)
{
  int i, end;

  if (!_ini_open_rd(f))
    return STR_BOOL_FALSE; // can't open file
  
  i = _ini_find_section(f, section);
  if (i < 0)
    return STR_BOOL_FALSE; // can't find section

  i = _ini_find_ident(f, ident, i, &end);
  if (i == -1)
    return STR_BOOL_FALSE; // can't find ident
  
  return STR_BOOL_TRUE; // ident exist
}
//----------------------------------------------------------------------------
str_t ini_read_value(ini_t *f,
                     const char *section, const char *ident,
                     const char *default_value)
{
  int i, j;
  str_t h, h1;

  if (!_ini_open_rd(f))
    goto return_default_value; // can't open file
  
  // find section
  i = _ini_find_section(f, section);
  
  if (i == str_size(&f->data))
    goto return_default_value; // last empty section
    
  if (i == -1)
    goto return_default_value; // can't find section

  // find ident
  i = _ini_find_ident(f, ident, i, &j);

  if (i == -1)
    goto return_default_value; // can't find ident
  
  i = str_skip_for(&f->data, INI_SP_LIST INI_EQ_LIST, i, 0, -1);
  if (i == -1)
    return str_zero(); // end of file => return empty string
  
  j = str_find_one_of(&f->data, INI_LR_LIST INI_COM, i, 0, -1);
  if (j == -1)
    j = str_size(&f->data); // fixed 2008.02.06
  h = str_substr(&f->data, i, j - i);
  h1 = str_trim(&h);
  str_free(&h);
  return h1;
  
return_default_value:
  return str_cstr(default_value);
}
//----------------------------------------------------------------------------
void ini_write_value(ini_t *f,
                     const char *section, const char *ident,
                     const char *value)
{
  int i, j, k;
  str_t val, id, h;

  // open to write
  _ini_open_wr(f);

  // trim value
  h = str_cstr(value);
  val = str_trim(&h);
  str_free(&h);

  // find section
  i = _ini_find_section(f, section);
  
  if (i == str_size(&f->data) && i != 0)
  { // last empty section => add '\n' to end of file
    str_add_cstr(&f->data, INI_LR);
    i = str_size(&f->data) - 1;
  }
  else if (i == -1)
    i = _ini_add_section(f, section); // can't find section => add section

  // find ident
  i = _ini_find_ident(f, ident, i, &j);
  
  if (i == -1)
  { // can't find ident => add string ("ident=value\n")
    
    // trim ident
    h = str_cstr(ident);
    id = str_trim(&h);
    str_free(&h);

    if (j != 0)
    {
      char c = *str_at(&f->data, j);
      if (strchr(INI_LR, c) != (char*) NULL)
        j++;
    }

#ifdef INI_EQ
    h = str_sum("SsSs", &id, INI_EQ, &val, INI_LR);
#else
    h = str_sum("SsSs", &id, INI_SP, &val, INI_LR);
#endif
    str_insert(&f->data, &h, j);
    str_free(&h);
    str_free(&id);
    str_free(&val);
    return;
  } // if ident not found

  // ident found, find end of value (k)
  k = str_find_one_of(&f->data, INI_LR_LIST INI_COM, i, 0, -1);
  if (k == -1)
  {
    k = str_size(&f->data);
    str_add_cstr(&f->data, INI_LR);
  }
  else
  {
    char c = *str_at(&f->data, k);
    if (strchr(INI_COM, c) != (char*) NULL)
      str_add_cstr(&val, INI_SP);
  }

#ifdef INI_EQ
  j = str_find_one_of(&f->data, INI_EQ_LIST, i, 0, k - i);
  if (j == -1)
  {
    str_insert_cstr(&val, INI_EQ, 0);
    j = i;
  }
  else
  {
    j = str_skip_for(&f->data, INI_SP_LIST INI_EQ_LIST, j, 0, -1);
    if (j == -1)
      j = str_size(&f->data); // end of file
  }
#else
  j = str_skip_for(&f->data, INI_SP_LIST, i, 0, -1);
  if (j == -1)
    j = i; // end of file
  if (i == j)
    str_insert_cstr(&val, INI_SP, 0);
#endif
  // j - begin of value index, k - end of value index
  str_delete(&f->data, j, k - j);
  str_insert(&f->data, &val, j);
  str_free(&val);
}
//----------------------------------------------------------------------------
str_t ini_code_str(const char *str)
{
  str_t retv = str_zero();
  while (1)
  { 
    char c = *str++;
    if (c == '\0')
      break;
    if (strchr(INI_COM, c) != (char*) NULL ||
        (c < ' ' && c > '\0'))
    {
      str_t hex_code = str_hex((unsigned long) c, 2);
      str_add_cstr(&retv, INI_BSLASH_PREF);
      str_add(&retv, &hex_code);
      str_free(&hex_code);
      continue;
    }
    if (c == INI_BSLASH)
      str_add_char(&retv, INI_BSLASH);
    str_add_char(&retv, c);
  }
  return retv;
}
//----------------------------------------------------------------------------
str_t ini_decode_str(const char *str)
{
  int i;
  str_t retv = str_zero();
  for (i = strlen(str); i > 0; i--)
  { 
    char c = *str++;
    if (c == INI_BSLASH)
    {
      str_t code;
      int base = 8;
      int code_size = 3; // default (normal) size of byte OCT code
      if (i <= 1)
        break;
      c = *str;
      if (c == INI_BSLASH_X)
      {
        str++;
        i--;
        base = 16;
        code_size = 2; // default (normal) size of byte HEX code
      }
      else if (c == INI_BSLASH)
      {
        str++;
        i--;
        str_add_char(&retv, c);
        continue;
      }
      if (i <= 1)
        break;
      if (code_size > (i - 1))
        code_size = i - 1;
      str_init_size_cstr(&code, code_size, str);
      c = (char) str_to_long_ex(&code, (long) INI_BSLASH, base);
      str_free(&code);
      str += code_size;
      i -= code_size;
    }
    str_add_char(&retv, c);
  }
  return retv;
}
//----------------------------------------------------------------------------
str_t ini_read_str(ini_t *f,
                   const char *section, const char *ident,
                   const char *default_value)
{
  char c1, c2;
  str_t str = str_sum("csc", '"', default_value, '"');
  str_t val = ini_read_value(f, section, ident, str_c(&str));
  str_free(&str);
  c1 = *str_c(&val);
  c2 = *str_last_char(&val);
  if ( (c1 == '\'' && c2 == '\'') ||
       (c1 == '"'  && c2 == '"' ) )
  {
    str = str_substr(&val, 1, str_size(&val) - 2);
    str_free(&val);
    val = ini_decode_str(str_c(&str));
    str_free(&str);
    return val;
  }
  str = ini_decode_str(str_c(&val));
  str_free(&val);
  return str;
}
//----------------------------------------------------------------------------
void ini_write_str(ini_t *f,
                   const char *section, const char *ident,
                   const char *value)
{
  str_t val = ini_code_str(value);
  str_t str = str_sum("cSc", '"', &val, '"');
  str_free(&val);
  ini_write_value(f, section, ident, str_c(&str)); 
  str_free(&str);
}
//----------------------------------------------------------------------------
#ifdef STR_EXTRA
str_t ini_read_bin(ini_t *f,
                   const char *section, const char *ident,
                   const char *default_value, int default_size)
{
  str_t def = str_bin2hex(default_value, default_size);
  str_t val = ini_read_value(f, section, ident, str_c(&def)); 
  str_t retv = str_hex2bin(str_c(&val));
  str_free(&val);
  str_free(&def);
  return retv;
}
//----------------------------------------------------------------------------
void ini_write_bin(ini_t *f,
                   const char *section, const char *ident,
                   const char *value, int size)
{
  str_t val = str_bin2hex(value, size);
  ini_write_value(f, section, ident, str_c(&val)); 
  str_free(&val);
}
#endif // STR_EXTRA
//----------------------------------------------------------------------------
long ini_read_long(ini_t *f,
                   const char *section, const char *ident,
                   long default_value)
{
  str_t def = str_long(default_value);
  str_t val = ini_read_value(f, section, ident, str_c(&def)); 
  long retv = str_size(&val) ?
              str_to_long_def(&val, default_value) : default_value;
  str_free(&val);
  str_free(&def);
  return retv;
}
//----------------------------------------------------------------------------
void ini_write_long(ini_t *f,
                    const char *section, const char *ident,
                    long value)
{
  str_t val = str_long(value);
  ini_write_value(f, section, ident, str_c(&val)); 
  str_free(&val);
}
//----------------------------------------------------------------------------
#ifdef STR_INT64_DEF
// read and write value as int64
STR_INT64 ini_read_int64(ini_t *f,
                         const char *section, const char *ident,
                         STR_INT64 default_value)
{
  str_t def = str_uint64_hex((STR_UINT64) default_value);
  str_t val = ini_read_value(f, section, ident, str_c(&def)); 
  STR_INT64 retv = str_size(&val) ?
                   str_to_int64(&val, default_value, 16) : default_value;
  str_free(&val);
  str_free(&def);
  return retv;
}
//----------------------------------------------------------------------------
void ini_write_int64(ini_t *f,
                     const char *section, const char *ident,
                     STR_INT64 value)
{
  str_t val = str_uint64_hex((STR_UINT64) value);
  ini_write_value(f, section, ident, str_c(&val)); 
  str_free(&val);
}
#endif // STR_INT64_DEF
//----------------------------------------------------------------------------
#ifdef STR_FLOAT
float ini_read_float(ini_t *f,
                     const char *section, const char *ident,
                     float default_value)
{
  str_t def = str_float(default_value);
  str_t val = ini_read_value(f, section, ident, str_c(&def)); 
  float retv = str_size(&val) ? (float) str_to_float(&val) : default_value;
  str_free(&val);
  str_free(&def);
  return retv;
}
//----------------------------------------------------------------------------
void ini_write_float(ini_t *f,
                     const char *section, const char *ident,
                     float value)
{
  str_t val = str_float(value);
  ini_write_value(f, section, ident, str_c(&val)); 
  str_free(&val);
}
//----------------------------------------------------------------------------
// read and write value as double
double ini_read_double(ini_t *f,
                       const char *section, const char *ident,
                       double default_value)
{
  str_t def = str_double(default_value);
  str_t val = ini_read_value(f, section, ident, str_c(&def)); 
  double retv = str_size(&val) ? str_to_double(&val) : default_value;
  str_free(&val);
  str_free(&def);
  return retv;
}
//----------------------------------------------------------------------------
void ini_write_double(ini_t *f,
                      const char *section, const char *ident,
                      double value)
{
  str_t val = str_double(value);
  ini_write_value(f, section, ident, str_c(&val)); 
  str_free(&val);
}
#endif // STR_FLOAT
//----------------------------------------------------------------------------
STR_BOOL ini_read_bool(ini_t *f,
                       const char *section, const char *ident,
                       STR_BOOL default_value)
{
  str_t def = str_bool(default_value);
  str_t val = ini_read_value(f, section, ident, str_c(&def)); 
  STR_BOOL retv = str_size(&val) ? str_to_bool(&val) : default_value;
  str_free(&val);
  str_free(&def);
  return retv;
}
//----------------------------------------------------------------------------
void ini_write_bool(ini_t *f,
                    const char *section, const char *ident,
                    STR_BOOL value)
{
  str_t val = str_bool(value);
  ini_write_value(f, section, ident, str_c(&val)); 
  str_free(&val);
}
//----------------------------------------------------------------------------

/*** end of "ini.c" ***/

