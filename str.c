/*
 * Project: C string type
 * Version: 0.2b
 * File: "str.c"
 * (C) 2007-2015 Alex Zorg <azorg@mail.ru>
 */

//----------------------------------------------------------------------------
#include "str.h"
//----------------------------------------------------------------------------
char _str_delim[] = STR_TRIM_DELIMITERS; // default trim delimiters
#ifdef STR_EXCEPTION_STRING
char _str_exstr[] = STR_EXCEPTION_STRING; // exception string
#endif // STR_EXCEPTION_STRING
int  _str_def_sector = STR_DEF_SECTOR_SIZE; // sector default value
char _str_loc_al[]   = STR_LOCAL_AL; // local alphabet low case
char _str_loc_au[]   = STR_LOCAL_AU; // local alphabet upper case
//----------------------------------------------------------------------------
#ifdef STR_DEBUG
unsigned _str_malloc_num   = 0;
unsigned _str_malloc_bytes = 0;
#endif // STR_DEBUG
//----------------------------------------------------------------------------
void _str_create_size(str_t *s, int size) // size > 0
{
  s->size = size;
  size = (size + s->sector) / s->sector; // number of sectors "[(A+B-1+1)/B]"
  s->ptr = _str_malloc(size * s->sector);
}
//----------------------------------------------------------------------------
void _str_create_cstr(str_t *s, const char *src) // src != NULL
{
  unsigned int seg;
  if ( (s->size = strlen(src)) == 0 )
  {
    s->ptr = (char*) NULL;
    return;
  }
  seg = (s->size + s->sector) / s->sector; // segments "[(A+B-1+1)/B]"
  s->ptr = _str_malloc(seg * s->sector);
  memcpy((void*) s->ptr, (const void*) src, s->size + 1);
}
//----------------------------------------------------------------------------
void str_init_size_cstr(str_t *s, int size, const char *src)
{
  s->sector = _str_def_sector;
  if (size <= 0)
  {
    s->size = 0;
    s->ptr = (char*) NULL;
    return;
  }
  _str_create_size(s, size);
  if (src != (const char *) NULL)
  {
    int i;
    for (i = 0; i < size; i++)
    {
      char c = *src++;
      s->ptr[i] = c;
      if (c == '\0')
      break;
    }
  }
  s->ptr[size] = '\0';
}
//----------------------------------------------------------------------------
void str_init_cstr(str_t *s, const char *src)
{
  s->sector = _str_def_sector;
  if (src == (const char *) NULL)
  {
    s->size = 0;
    s->ptr = (char*) NULL;
    return;
  }
  _str_create_cstr(s, src);
}
//----------------------------------------------------------------------------
void str_init_str(str_t *s, const str_t *src)
{
  s->sector = src->sector; // legacy sector size
  if (src->size != 0)
  {
    _str_create_size(s, src->size);
    memcpy((void*) s->ptr, (const void*) src->ptr, s->size + 1);
  }
  else s->size = 0;
}
//----------------------------------------------------------------------------
void str_init_char(str_t *s, char src)
{
  s->sector = _str_def_sector;
  _str_create_size(s, 1);
  s->ptr[0] = src;
  s->ptr[1] = '\0';
}
//----------------------------------------------------------------------------
void str_init_vsprintf(str_t *s, const char *fmt, va_list ap)
{
  str_t buf; // temp buffer for vsnprintf()
  int str_size;
  va_list ap_copy;

  // prepare temp buffer, set dafault sector size
  s->sector  = _str_def_sector;
  buf.sector = _str_def_sector;
  str_size   = _str_def_sector - 1;
  if (str_size < 1) str_size = 1;
  _str_create_size(&buf, str_size);

  // optimistic try vsnprintf() first time
  va_copy(ap_copy, ap);
  str_size = vsnprintf((char*) buf.ptr, buf.size, fmt, ap_copy);
  va_end(ap_copy);
#ifdef STR_DEBUG_EXTRA
  buf.ptr[buf.size] = '\0';
  fprintf(stderr, "str.c: 1.vsnprintf(%i)=%i '%s'\n",
          buf.size, str_size, buf.ptr);
#endif // STR_DEBUG_EXTRA

  while (str_size < 0)
  { // it's may be Windows (or HP-UX?)
    str_set_size(&buf, buf.size + _str_def_sector); // increment buf size
    
    // try vsnprintf() again
    va_copy(ap_copy, ap);
    str_size = vsnprintf((char*) buf.ptr, buf.size, fmt, ap_copy);
    va_end(ap_copy);
#ifdef STR_DEBUG_EXTRA
    buf.ptr[buf.size] = '\0';
    fprintf(stderr, "str.c: 2.vsnprintf(%i)=%i '%s'\n",
            buf.size, str_size, buf.ptr);
#endif // STR_DEBUG_EXTRA
  }

  if (str_size != 0)
  { // set valid string size
    _str_create_size(s, str_size);

    if (str_size < buf.size)
    { // vsnprintf() OK => copy to destination string
      memcpy((void*)s->ptr, (const void*)buf.ptr, str_size + 1);
    }
    else
    { // last try vsnprintf()
      va_copy(ap_copy, ap);
      str_size = vsnprintf((char*)s->ptr, s->size + 1, fmt, ap_copy);
      va_end(ap_copy);

#ifdef STR_DEBUG_EXTRA
      fprintf(stderr, "str.c: 3.vsnprintf(%i)=%i '%s'\n",
        s->size + 1, str_size, s->ptr);
      if (str_size != s->size)
        fprintf(stderr, "str.c: ERROR in str_init_vsnprintf()!\n");
#endif // STR_DEBUG_EXTRA
    }
  }
  else // str_size == 0
  { // empty string
    s->size = 0;
  }

  // free buffer
  str_free(&buf);
}
//----------------------------------------------------------------------------
void str_init_bool(str_t *s, STR_BOOL src)
{
  s->sector = _str_def_sector;
  if (src)
    _str_create_cstr(s, STR_BOOL_TRUE_STR);
  else
    _str_create_cstr(s, STR_BOOL_FALSE_STR);
}
//----------------------------------------------------------------------------
str_t str_hex(unsigned long value, int digits)
{
  str_t s;
  str_init_zero(&s);
  if (digits > (int) (sizeof(long) << 1) ||
      digits < 0) digits = (int) (sizeof(long) << 1);
  if (digits != 0)
  {
    char c, *p;
    str_set_size(&s, digits);
    p = s.ptr + digits - 1;
    do {
      c = (((char) value) & 0x0F) + '0';
      if (c > '9') c += 'A' - '9' - 1;
      *p-- = c;
      value >>= 4;
    } while (--digits != 0);
  }
  return s;
}
//----------------------------------------------------------------------------
str_t str_bin(unsigned long value, int digits)
{
  str_t s;
  str_init_zero(&s);
  if (digits > (int) (sizeof(long) << 3) ||
      digits < 0) digits = (int) (sizeof(long) << 3);
  if (digits != 0)
  {
    char c, *p;
    str_set_size(&s, digits);
    p = s.ptr + digits - 1;
    do {
      c = (((char) value) & 1) + '0';
      *p-- = c;
      value >>= 1;
    } while (--digits != 0);
  }
  return s;
}
//----------------------------------------------------------------------------
str_t str_oct(unsigned long value, int digits)
{
  str_t s;
  str_init_zero(&s);
  if (digits > (int) (((sizeof(long) << 3) + 2) / 3) ||
      digits < 0) digits = (int) (((sizeof(long) << 3) + 2) / 3);
  if (digits != 0)
  {
    char c, *p;
    str_set_size(&s, digits);
    p = s.ptr + digits - 1;
    do {
      c = (((char) value) & 0x07) + '0';
      *p-- = c;
      value >>= 3;
    } while (--digits != 0);
  }
  return s;
}
//----------------------------------------------------------------------------
str_t str_dec(unsigned long value, int digits)
{
  str_t s;
  str_t fmt = str_sum("sis", "%0", digits, "lu");
  str_init_sprintf(&s, str_c(&fmt), value);
  str_free(&fmt);
  return s;
}
//----------------------------------------------------------------------------
STR_BOOL str_is_equal(const str_t *s1, const str_t *s2)
{
  if (s1->size != s2->size) return STR_BOOL_FALSE;
  if (s1->size == 0)        return STR_BOOL_TRUE;
  if (strcmp(s1->ptr, s2->ptr) == 0) return STR_BOOL_TRUE;
  else                               return STR_BOOL_FALSE;
}
//----------------------------------------------------------------------------
STR_BOOL str_is_equal_cstr(const str_t *s1, const char *s2)
{
  if (s1->size == 0)
  {
    if (strlen(s2) == 0) return STR_BOOL_TRUE;
    else                 return STR_BOOL_FALSE;
  }
  if (strcmp(s1->ptr, s2) == 0) return STR_BOOL_TRUE;
  else                          return STR_BOOL_FALSE;
}
//----------------------------------------------------------------------------
long str_to_long_cstyle(const str_t *s, long def_val, unsigned char base)
{
  unsigned int i;
  unsigned char c, *p, flg = 0;
  long ret = 0;
  str_t t = str_trim(s); // trimed string
  if ((i = t.size) == 0) goto err;
  if (base < 2 || base > 36) goto err;
  p = (unsigned char*) t.ptr;
  if (*p == '-')
  {
    i--; p++; flg = 1;
  }
  else if (*p == '+')
  {
    i--; p++; // flg = 0;
  }
  else if (*p == '0')
  {
    if (--i == 0)
    {
      str_free(&t);
      return 0;
    }
    p++;
    base = 8; // OCT (ANSI C)
    if (*p == 'x' || *p == 'X')
    { // HEX
      base = 16;
      i--; p++;
    }
    else if (*p == 'b' || *p == 'B')
    { // BIN (extension)
      base = 2;
      i--; p++;
    }
    else if (*p == 'o' || *p == 'O')
    { // OCT (extension)
      base = 8;
      i--; p++;
    }
    else if (*p == 'd' || *p == 'D')
    { // DEC (extension)
      base = 10;
      i--; p++;
    }
  }
  if (i == 0) goto err;
  do {
    c = *p++;
    if (base > 10)
    {
      if (c >= 'A' && c <= 'Z') c -= 'A' - '9' - 1;
      else if (c >= 'a' && c <= 'z') c -= 'a' - '9' - 1;
    }
    c -= '0';
    if (c >= base) goto err; // illegal char
    ret *= (long) base;
    ret += (long) c;
  } while (--i != 0);
  str_free(&t);
  return (flg == 0) ? ret : -ret;
err:
  str_free(&t);
  return def_val;
}
//----------------------------------------------------------------------------
#ifdef STR_INT64_DEF
STR_INT64 str_to_int64(const str_t *s, STR_INT64 def_val, unsigned char base)
{
  unsigned int i;
  unsigned char c, *p, flg = 0;
  STR_INT64 ret = 0;
  str_t t = str_trim(s); // trimed string
  if ((i = t.size) == 0) goto err;
  if (base < 2 || base > 36) goto err;
  p = (unsigned char*) t.ptr;
  if (*p == '-'){ p++; i--; flg = 1; }
  else if (*p == '+'){ p++; i--; }
  if (i == 0) goto err;
  do {
    c = *p++;
    if (base > 10)
    {
      if (c >= 'A' && c <= 'Z') c -= 'A' - '9' - 1;
      else if (c >= 'a' && c <= 'z') c -= 'a' - '9' - 1;
    }
    c -= '0';
    if (c >= base) goto err; // illegal char
    ret *= (STR_INT64) base;
    ret += (STR_INT64) c;
  } while (--i != 0);
  str_free(&t);
  return (flg == 0) ? ret : -ret;
err:
  str_free(&t);
  return def_val;
}
#endif // STR_INT64_DEF
//----------------------------------------------------------------------------
long str_to_long_ex(const str_t *s, long def_val, unsigned char base)
{
  unsigned int i;
  unsigned char c, *p, flg = 0;
  long ret = 0;
  str_t t = str_trim(s); // trimed string
  if ((i = t.size) == 0) goto err;
  if (base < 2 || base > 36) goto err;
  p = (unsigned char*) t.ptr;
  if (*p == '-'){ p++; i--; flg = 1; }
  else if (*p == '+'){ p++; i--; }
  if (i == 0) goto err;
  do {
    c = *p++;
    if (base > 10)
    {
      if (c >= 'A' && c <= 'Z') c -= 'A' - '9' - 1;
      else if (c >= 'a' && c <= 'z') c -= 'a' - '9' - 1;
    }
    c -= '0';
    if (c >= base) goto err; // illegal char
    ret *= (long) base;
    ret += (long) c;
  } while (--i != 0);
  str_free(&t);
  return (flg == 0) ? ret : -ret;
err:
  str_free(&t);
  return def_val;
}
//----------------------------------------------------------------------------
STR_BOOL str_to_bool(const str_t *s)
{
  if (str_is_equal_cstr(s, STR_BOOL_TRUE_STR))
    return STR_BOOL_TRUE;

  if (str_is_equal_cstr(s, STR_BOOL_FALSE_STR))
    return STR_BOOL_FALSE;

  if (str_to_long(s) != 0)
    return STR_BOOL_TRUE;

  return STR_BOOL_FALSE;
}
//----------------------------------------------------------------------------
void str_assign(str_t *dst, const str_t *src)
{
  unsigned int b_cur = 0, b_new = 0;
  if (dst->size != 0) b_cur = (dst->size + dst->sector) / dst->sector;
  if (src->size != 0) b_new = (src->size + dst->sector) / dst->sector;
  if (b_cur == b_new)
  {
    if (b_cur != 0)
    {
      memcpy((void*) dst->ptr, (const void*) src->ptr, src->size + 1);
      dst->size = src->size;
    }
  }
  else
  {
    str_free(dst);
    if (src->size != 0)
    {
      _str_create_size(dst, src->size);
      memcpy((void*) dst->ptr, (const void*) src->ptr, src->size + 1);
    }
    else dst->size = 0;
  }
}
//----------------------------------------------------------------------------
void str_assign_cstr(str_t *dst, const char *src)
{
  unsigned int src_size = 0, b_cur = 0, b_new = 0;
  if (src != NULL) src_size = strlen(src);
  if (dst->size != 0) b_cur = (dst->size + dst->sector) / dst->sector;
  if (src_size  != 0) b_new = (src_size  + dst->sector) / dst->sector;
  if (b_cur == b_new)
  {
    if (b_cur != 0)
    {
      memcpy((void*) dst->ptr, (const void*) src, src_size + 1);
      dst->size = src_size;
    }
  }
  else
  {
    str_free(dst);
    if (src_size != 0)
    {
      _str_create_size(dst, src_size);
      memcpy((void*) dst->ptr, (const void*) src, src_size + 1);
    }
    else dst->size = 0;
  }
}
//----------------------------------------------------------------------------
void str_add(str_t *s, const str_t *add)
{
  unsigned int new_size = s->size + add->size;
  unsigned int b_cur = 0, b_new = 0;
  if (s->size  != 0) b_cur = (s->size  + s->sector) / s->sector;
  if (new_size != 0) b_new = (new_size + s->sector) / s->sector;
  if (b_cur == b_new)
  {
    if (b_new != 0 && add->size != 0)
    {
      memcpy((void*) &s->ptr[s->size], (const void*) add->ptr, add->size + 1);
      s->size = new_size;
    }
  }
  else
  {
    char *new_ptr = _str_malloc(b_new * s->sector);
    memcpy((void*) new_ptr, (const void*) s->ptr, s->size);
    memcpy((void*) &new_ptr[s->size], (const void*) add->ptr, add->size + 1);
    str_free(s);
    s->size = new_size;
    s->ptr  = new_ptr;
  }
}
//----------------------------------------------------------------------------
void str_add_cstr(str_t *s, const char *add)
{
  unsigned int new_size, b_cur = 0, b_new = 0;
  unsigned int add_size = 0;
  if (add != NULL) add_size = strlen(add);
  new_size = s->size + add_size;
  if (s->size  != 0) b_cur = (s->size  + s->sector) / s->sector;
  if (new_size != 0) b_new = (new_size + s->sector) / s->sector;
  if (b_cur == b_new)
  {
    if (b_new != 0 && add_size != 0)
    {
      memcpy((void*) &s->ptr[s->size], (const void*) add, add_size + 1);
      s->size = new_size;
    }
  }
  else
  {
    char *new_ptr = _str_malloc(b_new * s->sector);
    memcpy((void*) new_ptr, (const void*) s->ptr, s->size);
    memcpy((void*) &new_ptr[s->size], (const void*) add, add_size + 1);
    str_free(s);
    s->size = new_size;
    s->ptr  = new_ptr;
  }
}
//----------------------------------------------------------------------------
void str_add_char(str_t *s, char add)
{
  unsigned int b_cur = 0, b_new;
  if (s->size != 0) b_cur = (s->size     + s->sector) / s->sector;
                    b_new = (s->size + 1 + s->sector) / s->sector;
  if (b_cur == b_new)
  {
    s->ptr[s->size++] = add;
    s->ptr[s->size  ] = '\0';
  }
  else
  {
    char *new_ptr = _str_malloc(b_new * s->sector);
    if (s->size != 0)
    {
      memcpy((void*) new_ptr, (const void*) s->ptr, s->size);
      str_free(s);
    }
    new_ptr[s->size++] = add;
    new_ptr[s->size  ] = '\0';
    s->ptr = new_ptr;
  }
}
//----------------------------------------------------------------------------
// concatenation
// fmt: S - str_t*, s - char*, c - char, i - int, i - uint, x - hex
//      f - float, d - double, I - int64, U - uint64, X - uint64-HEX, ...
str_t str_sum(const char *fmt, ...)
{
  va_list ap;
  int i, num, sum_size = 0;
  struct str_sum_list { str_t s, *p; };
  struct str_sum_list buf[STR_SUM_MAX_ITEM];
  struct str_sum_list *lst = buf;
  str_t s; // return value
  str_init_zero(&s);

  // check argument
  if (fmt == (char*) NULL) return s;
  num = strlen(fmt);
  if (num == 0) return s;

  // allocate temp buffer
  if (num > STR_SUM_MAX_ITEM)
    lst = (struct str_sum_list*)
          _str_malloc_mem(sizeof(struct str_sum_list) * num);

  va_start(ap, fmt);
  for (i = 0; i < num; i++)
  {
    char c = fmt[i];
    if (c == 'S') lst[i].p = va_arg(ap, str_t*);
    else
    {
      str_t *p = lst[i].p = &lst[i].s;
      if      (c == 's')
        str_init_cstr(p, va_arg(ap, char*));
      else if (c == 'c')
        str_init_char(p, va_arg(ap, int));
      else if (c == 'i')
        str_init_int(p, va_arg(ap, int));
      else if (c == 'u')
        str_init_uint(p, va_arg(ap, unsigned int));
      else if (c == 'x')
        str_init_int_hex(p, va_arg(ap, int));
#ifdef STR_INT64_DEF
      else if (c == 'I')
        str_init_int64(p, va_arg(ap, STR_INT64));
      else if (c == 'U')
        str_init_uint64(p, va_arg(ap, STR_UINT64));
      else if (c == 'X')
        str_init_uint64_hex(p, va_arg(ap, STR_UINT64));
#endif // STR_INT64_DEF
#ifdef STR_FLOAT
      else if (c == 'f')
        str_init_float(p, (float) va_arg(ap, double));
      else if (c == 'd')
        str_init_double(p, va_arg(ap, double));
#endif // STR_FLOAT
      else if (c == 'b')
        str_init_bool(p, va_arg(ap, int));
      else
        str_init_zero(p); // unknown type
    }
    sum_size += lst[i].p->size;
  }
  va_end(ap);

  // copy to output buffer
  if (sum_size != 0)
  {
    char *p;
    str_set_size(&s, sum_size);
    p = s.ptr;
    for (i = 0; i < num; i++)
    {
      if (lst[i].p->size != 0)
        memcpy((void*) p, (const void*) lst[i].p->ptr, lst[i].p->size);
      p += lst[i].p->size;
    }
  }

  // free list
  for (i = 0; i < num; i++)
    if (lst[i].p == &lst[i].s)
      str_free(lst[i].p);

  // free buffer
  if (num > STR_SUM_MAX_ITEM)
    _str_free_mem((void*) lst);

  return s;
}
//----------------------------------------------------------------------------
// accessing character by specified index
char *str_at(const str_t *s, int index)
{
  static char c;
  if (index < 0) index += s->size; // like Python
  if (index >= 0 && index < s->size)
    return s->ptr + index;
  c = STR_ERROR_CHAR;
  return &c;
}
//----------------------------------------------------------------------------
// convert to "C" string
const char *str_c(const str_t *s)
{
  static char c;
  if (s->size != 0)
    return s->ptr;
  c = '\0';
  return &c;
}
//----------------------------------------------------------------------------
str_t str_substr(const str_t *s, int index, int count)
{
  str_t r;
  if (index < 0) index += s->size; // like Python
  if (index >= 0 && index < s->size && count != 0)
  {
    if (count < 0 || count > (s->size - index))
      count = s->size - index;

    r.sector = _str_def_sector;
    _str_create_size(&r, count);
    memcpy((void*) r.ptr, (const void*) (s->ptr + index), count);
    r.ptr[count] = '\0';
  }
  else
    str_init_zero(&r);
  return r;
}
//----------------------------------------------------------------------------
int str_strpos(const str_t *s, const str_t *substr, int index)
{
  char *p;
  if (index < 0) index += s->size; // like Python
  if (index < 0 || index >= s->size) return -1;
  p = strstr(s->ptr + index, str_c(substr));
  if (p == (char*) NULL) return -1;
  return (int)(p - s->ptr);
}
//----------------------------------------------------------------------------
int str_chrpos(const str_t *s, char subchr, int index)
{
  char *p;
  if (index < 0) index += s->size; // like Python
  if (index < 0 || index >= s->size) return -1;
  p = strchr(s->ptr + index, subchr);
  if (p == (char*) NULL) return -1;
  return (int)(p - s->ptr);
}
//----------------------------------------------------------------------------
int str_find_last(const str_t *s, const str_t *substr)
{
  int i, j;
  if (substr->size == 0) return -1;
  i = 0;
  j = -1;
  while ( (i = str_strpos(s, substr, i)) != -1 )
  {
    j = i;
    i += substr->size;
  }
  return j;
}
//----------------------------------------------------------------------------
int str_find_one_of(
  const str_t *s,   // find in this string
  const char *cstr, // find first one of char from this C string
  int index,        // begin from this index
  int dir,          // direction <0 or >=0
  int count)        // maximum count of search (<0 - unlimited)
{
  int cstr_size = strlen(cstr);
  if (index < 0) index += s->size; // like Python
  if (index < 0) return -1;
  if (count < 0) count = s->size - index;
  dir = (dir >= 0) ? 1 : -1;
  while (index < s->size && count-- > 0)
  {
    int i;
    for (i = 0; i < cstr_size; i++) // number of variants
      if (s->ptr[index] == cstr[i]) return index;
    index += dir;
  }
  return -1;
}
//----------------------------------------------------------------------------
int str_skip_for(
  const str_t *s,   // find in this string
  const char *cstr, // skip any char from this C string
  int index,        // begin from this index
  int dir,          // direction <0 or >=0
  int count)        // maximum count of search (<0 - unlimited)
{
  int cstr_size = strlen(cstr);
  if (index < 0) index += s->size; // like Python
  if (index < 0 || index >= s->size) return -1;
  if (dir >= 0)
  {
    int max_count = s->size - index;
    if (count < 0 || count > max_count) count = max_count;
    dir = 1;
  }
  else
  {
    int max_count = index + 1;
    if (count < 0 || count > max_count) count = max_count;
    dir = -1;
  }
  while (count-- > 0)
  {
    int i;
    char c1 = s->ptr[index];
    for (i = 0; i < cstr_size; i++) // number of variants
    {
      char c2 = cstr[i];
      if (c1 == c2) break;
    }
    if (i == cstr_size) return index;
    index += dir;
  }
  return -1;
}
//----------------------------------------------------------------------------
str_t str_lower_case(const str_t *s)
{
  str_t r;
  char *p;
  if (s->size != 0)
  {
    int i = s->size;
    char c, *src = s->ptr, *dst;
    r.sector = _str_def_sector;
    _str_create_size(&r, s->size);
    dst = r.ptr;
    while (i-- != 0)
    {
      c = *src++;
      if (c >= 'A' && c < 'Z')
        c += 'a'-'A'; // convert ASCII symbol
      else
      {
        p = strchr(_str_loc_au, (unsigned) c); // find in upper case
        if (p != (char*) NULL) // convert KOI8-R/CP1251/CP866 symbol
          c = _str_loc_al[(int)(p - _str_loc_au)];
      }
      *dst++ = c;
    }
    *dst = '\0';
  }
  else
    str_init_zero(&r);
  return r;
}
//----------------------------------------------------------------------------
str_t str_upper_case(const str_t *s)
{
  str_t r;
  char *p;
  if (s->size != 0)
  {
    int i = s->size;
    char c, *src = s->ptr, *dst;
    r.sector = _str_def_sector;
    _str_create_size(&r, s->size);
    dst = r.ptr;
    while (i-- != 0)
    {
      c = *src++;
      if (c >= 'a' && c < 'z')
        c -= 'a' - 'A'; // convert ASCII symbol
      else
      {
        p = strchr(_str_loc_al, (unsigned) c); // find in low case
        if (p != NULL) // convert KOI8-R/CP1251/CP866 symbol
          c = _str_loc_au[(int)(p - _str_loc_al)];
      }
      *dst++ = c;
    }
    *dst = '\0';
  }
  else
    str_init_zero(&r);
  return r;
}
//----------------------------------------------------------------------------
str_t str_trim_ex(const str_t *s, const char *delim)
{
  int i, j;
  i = str_skip_for(s, delim, 0, 0, -1); // skip delim. from begin
  if (i == -1) return str_zero();
  j = str_skip_for(s, delim, s->size - 1, -1, -1); // skip delim. from end
  if (++j == 0) return str_zero();
  return str_substr(s, i, j - i);
}
//----------------------------------------------------------------------------
str_t str_trim_left_ex(const str_t *s, const char *delim)
{
  int i = str_skip_for(s, delim, 0, 0, -1); // skip delim. from begin
  if (i == -1) return str_zero();
  return str_substr(s, i, -1);
}
//----------------------------------------------------------------------------
str_t str_trim_right_ex(const str_t *s, const char *delim)
{
  int i = str_skip_for(s, delim, s->size - 1, -1, 1); // skip delim. from end
  if (++i == 0) return str_zero();
  return str_substr(s, 0, i);
}
//----------------------------------------------------------------------------
void str_swap(str_t *s1, str_t *s2)
{
  str_t tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}
//----------------------------------------------------------------------------
void str_set_size(str_t *s, int new_size)
{
  int b_cur = 0, b_new = 0;
  if (new_size < 0) return; // ?!
  if (s->size  != 0) b_cur = (s->size  + s->sector) / s->sector;
  if (new_size != 0) b_new = (new_size + s->sector) / s->sector;
  if (b_cur == b_new)
  {
    if (b_cur == 0) return;
  }
  else // b_cur != b_new
  {
    char *new_ptr;
    if (b_new == 0)
    {
      str_free(s);
      s->size = 0;
      return;
    }
    new_ptr = _str_malloc(b_new * s->sector);
    if (s->size != 0)
      memcpy((void*) new_ptr, (const void*) s->ptr,
             (new_size > s->size ? s->size : new_size));
    str_free(s);
    s->ptr = new_ptr;
  }
  s->ptr[s->size = new_size] = '\0';
}
//----------------------------------------------------------------------------
void str_insert(str_t *s, const str_t *str, int index)
{
  int new_size, b_cur = 0, b_new = 0;
  if (index < 0) index += s->size; // like Python
  if (index < 0 || index > s->size) return; // do nothing
  new_size = s->size + str->size;
  if (s->size  != 0) b_cur = (s->size  + s->sector) / s->sector;
  if (new_size != 0) b_new = (new_size + s->sector) / s->sector;
  if (b_cur == b_new)
  {
    if (b_new != 0 && str->size != 0)
    {
      int i = s->size - index;
      char *src = s->ptr + s->size, *dst = s->ptr + new_size;
      while (i-- != 0) *(--dst) = *(--src);
      memcpy(s->ptr + index, str->ptr, str->size);
      s->ptr[s->size = new_size] = '\0';
    }
  }
  else
  {
    char *new_ptr = _str_malloc(b_new * s->sector);
    if (index != 0)
      memcpy(new_ptr, s->ptr, index);
    memcpy(new_ptr + index, str->ptr, str->size);
    if (s->size != index)
      memcpy(new_ptr + index + str->size, s->ptr + index, s->size - index);
    str_free(s);
    s->ptr = new_ptr;
    s->ptr[s->size = new_size] = '\0';
  }
}
//----------------------------------------------------------------------------
void str_insert_cstr(str_t *s, const char *cstr, int index)
{
  str_t str;
  str_init_cstr(&str, cstr);
  str_insert(s, &str, index);
  str_free(&str);
}
//----------------------------------------------------------------------------
void str_insert_char(str_t *s, char chr, int index)
{
  str_t str;
  str_init_char(&str, chr);
  str_insert(s, &str, index);
  str_free(&str);
}
//----------------------------------------------------------------------------
void str_delete(str_t *s, int index, int count)
{
  if (index < 0) index += s->size; // like Python
  if (index >= 0 && index < s->size && count != 0)
  {
    int new_size, b_new = 0, b_cur, tail;
    if (count < 0 || count > s->size - index) count = s->size - index;
    new_size = s->size - count;
                       b_cur = (s->size  + s->sector) / s->sector;
    if (new_size != 0) b_new = (new_size + s->sector) / s->sector;
    tail = s->size - index - count;
    if (b_cur == b_new)
    {
      if (tail != 0)
      {
        char *src = s->ptr + index + count;
        char *dst = s->ptr + index;
        do
          *dst++ = *src++;
        while (--tail != 0);
      }
    }
    else
    {
      char *new_ptr = _str_malloc(b_new * s->sector);
      if (index != 0)
        memcpy(new_ptr, s->ptr, index);
      if (tail != 0)
      {
        char *src = s->ptr + index + count;
        char *dst = new_ptr + index;
        do
          *dst++ = *src++;
        while (--tail != 0);
      }
      str_free(s);
      s->ptr = new_ptr;
    }
    s->ptr[s->size = new_size] = '\0';
  }
}
//----------------------------------------------------------------------------
int str_replace(str_t *s, const str_t *old_substr, const str_t *new_substr)
{ // You can optimize this function
  int i = str_strpos(s, old_substr, 0);
  if (i != -1)
  {
    str_delete(s, i, old_substr->size);
    str_insert(s, new_substr, i);
  }
  return i;
}
//----------------------------------------------------------------------------
void str_fill(str_t *s, char chr, int index, int count)
{
  if (index < 0) index += s->size; // like Python
  if (index >= 0 && index < s->size && count != 0)
  {
    if (count < 0 || count > s->size - index) count = s->size - index;
    memset(s->ptr + index, chr, count);
  }
}
//----------------------------------------------------------------------------
void str_set_sector(str_t *s, int new_sector)
{
  if (new_sector < 0) return; // do nothing
  if (s->size != 0)
  {
    int b_cur = (s->size +  s->sector) / s->sector;
    int b_new = (s->size + new_sector) / new_sector;
    b_new *= new_sector;
    if ((b_cur * s->sector) != b_new)
    {
      char *new_ptr = _str_malloc(b_new);
      memcpy(new_ptr, s->ptr, s->size + 1);
      str_free(s);
      s->ptr = new_ptr;
    }
  }
  s->sector = new_sector;
}
//----------------------------------------------------------------------------
void str_rol(str_t *s, int count) // left scrolling
{
  if (s->size != 0 && count > 0)
  {
    char *src, *dst;
    int tail;
    if (count > s->size) count = s->size;
    tail = s->size - count;
    src = s->ptr + count;
    dst = s->ptr;
    while (tail-- != 0) *dst++ = *src++;
    while (count-- > 0) *dst++ = ' ';
  }
}
//----------------------------------------------------------------------------
void str_ror(str_t *s, int count) // right scrolling
{
  if (s->size != 0 && count > 0)
  {
    char *src, *dst;
    int tail;
    if (count > s->size) count = s->size;
    tail = s->size - count;
    src = s->ptr + s->size - count;
    dst = s->ptr + s->size;
    while (tail-- != 0) *(--dst) = *(--src);
    while (count-- > 0) *(--dst) = ' ';
  }
}
//----------------------------------------------------------------------------
str_t str_mul(const str_t *str, int times)
{
  str_t s;
  str_init_zero(&s);
  while (times-- > 0)
    str_add(&s, str);
  return s;
}
//----------------------------------------------------------------------------
#ifdef STR_EXTRA
// convert binary array to hex string (like "01:23:AB:CD")
str_t str_bin2hex(const char *bin, int size)
{
  str_t out;
  if (bin == (const char *) NULL || size <= 0)
    str_init_zero(&out); // return empty string
  else
  {
    char *dst;
    str_init_size_cstr(&out, size * 3 - 1, (const char*) NULL);
    dst = out.ptr;

    while (size-- != 0)
    {
      unsigned char val = (unsigned char) *bin++;
      char c = (char)(val >> 4) + '0';
      if (c > '9') c += 'A' - '9' - 1;
      *dst++ = c;
      c = (char)(val & 0x0F) + '0';
      if (c > '9') c += 'A' - '9' - 1;
      *dst++ = c;
      *dst++ = ':';
    }
    *--dst = '\0'; // terminate output string
  }
  return out;
}
//----------------------------------------------------------------------------
// convert hex string (like "01:23:AB:CD") to binary array
str_t str_hex2bin(const char *str)
{
  int size;
  str_t out;
  char *dst;

  if (str == (const char *) NULL)
    goto return_empty_string;

  size = strlen(str) + 1;
  if ((size % 3) != 0)
    goto return_empty_string;

  size /= 3;
  if (size == 0)
    goto return_empty_string;

  str_init_size_cstr(&out, size, (const char*) NULL);
  dst = out.ptr;

  while (size-- != 0)
  {
    char l, h = *str++;
    if (     h >= 'A' && h <= 'Z') h -= 'A' - '9' - 1;
    else if (h >= 'a' && h <= 'z') h -= 'a' - '9' - 1;
    h -= '0';
    h <<= 4;
    l = *str++;
    if (     l >= 'A' && l <= 'Z') l -= 'A' - '9' - 1;
    else if (l >= 'a' && l <= 'z') l -= 'a' - '9' - 1;
    l -= '0';
    *dst++ = h | l;
    h = *str++;
    if (h != ':' && h != '\0')
    { // syntax error
      str_free(&out);
      goto return_empty_string;
    }
  }
  return out;

return_empty_string:
  return str_zero();
}
#endif // STR_EXTRA
//----------------------------------------------------------------------------
int str_read_from_file(str_t *s, const char *fname, int add_flag)
{
  int i;
  FILE *file = fopen(fname, "r");
  if (file == (FILE*) NULL) return 0; // can't open for read
  if (!add_flag) str_clear(s);
  while ((i = fgetc(file)) != EOF)
    str_add_char(s, (char) i);
  fclose(file);
  return 1;
}
//----------------------------------------------------------------------------
int str_write_to_file(const str_t *s, const char *fname, int add_flag)
{
  char *p;
  int count = s->size;
  FILE *file;
  if (add_flag)
    file = fopen(fname, "a+");
  else
    file = fopen(fname, "w");
  if (file == (FILE*) NULL) return 0; // can't open for write
  for (p = s->ptr; count != 0; count--)
    if (fputc(*p++, file) == EOF) break;
  fclose(file);
  return (count == 0) ? 1 : 0;
}
//----------------------------------------------------------------------------
int str_read_from_bin_file(str_t *s, const char *fname, int add_flag)
{
  int old_size, count, size;
#ifndef STR_FAST_FILE_IO
  int i;
#endif // !STR_FAST_FILE_IO
  FILE *file = fopen(fname, "rb");
  if (file == (FILE*) NULL) return 0; // can't open for read
  if (!add_flag) str_clear(s);
#ifdef STR_FAST_FILE_IO
  if (fseek(file, 0L, SEEK_END) < 0) goto error;
  count = size = (int) ftell(file); // get file size
  if (size < 0) goto error;
  rewind(file);
  str_set_size(s, (old_size = s->size) + size);
  if (size != 0)
    count = fread((void*) &s->ptr[old_size], sizeof(char), size, file);
  if (s->size != 0)
    s->ptr[old_size + count] = '\0';
  if (count == size)
  {
    fclose(file);
    return 1;
  }
error:
  fclose(file);
  return 0;
#else // !STR_FAST_FILE_IO
  while ((i = fgetc(file)) != EOF)
    str_add_char(s, (char) i);
  fclose(file);
  return 1;
#endif // !STR_FAST_FILE_IO
}
//----------------------------------------------------------------------------
int str_write_to_bin_file(const str_t *s, const char *fname, int add_flag)
{
  int count = s->size;
#ifndef STR_FAST_FILE_IO
  char *p;
#endif // !STR_FAST_FILE_IO
  FILE* file;
  if (add_flag)
    file = fopen(fname, "ab+");
  else
    file = fopen(fname, "wb");
  if (file == (FILE*) NULL) return 0; // can't open for write
#ifdef STR_FAST_FILE_IO
  if (count != 0)
    count -= fwrite((const void *) s->ptr, sizeof(char), (size_t) count, file);
#else // !STR_FAST_FILE_IO
  for (p = s->ptr; count != 0; count--)
    if (fputc(*p++, file) == EOF) break;
#endif // !STR_FAST_FILE_IO
  fclose(file);
  return (count == 0) ? 1 : 0;
}
//----------------------------------------------------------------------------

/*** end of "str.c" ***/

