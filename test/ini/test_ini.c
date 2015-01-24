/*
 * Unit test of `aini_t` component
 */


#include <math.h>
#include "str.h"
#include "ini.h"

#ifndef M_PI
#define M_PI 3.1415
#endif

int main()
{
  int g_err = 0; // number of errors

  if (1)
  { // "initile_t" - write
    int i, j;
    ini_t f;
    //ini_init(&f);
    //ini_set_fname(&f, "cfg.ini");
    ini_init_fname(&f, "cfg.ini");

    ini_write_value(&f, "", "global", "bad data");
    ini_write_value(&f, "s0", "i0", "XXXXX");
    ini_write_value(&f, "s2", "i2", "YYYYY");
    
    // write "matrix"
    for (i = 0; i < 3; i++)
    {
      str_t section = str_cstr("s ");
      *str_at(&section, 1) = '0' + i;
      for (j = 0; j < 3; j++)
      {
	char value[3];
	str_t ident = str_cstr("i ");

        value[0] = '0' + i;
        value[1] = '0' + j;
        value[2] = 0;

        *str_at(&ident, 1) = '0' + j;
        ini_write_value(&f, str_c(&section), str_c(&ident), value);
        
	str_free(&ident);
      }
      str_free(&section);
    }
    
    ini_write_value(&f, "", " global ", " 123 456 ");
    ini_write_str(&f, "", "hard_str", "\n1 23\\456\\789 ;# ");
    
    ini_write_str  (&f, "sec", "str1", "some string #1 ");
    ini_write_str  (&f, "sec", "str2", " some string #2");
    ini_write_value(&f, "sec", "str3", "some string -3 ");
    
    ini_write_bool(&f, "bool", "flag1", STR_BOOL_TRUE);
    ini_write_bool(&f, "bool", "flag2", STR_BOOL_FALSE);

    ini_write_value(&f, "end sectIon ", "value", "end");
    
    ini_free(&f);
  }

  if (1)
  { // "initile_t" - read
    int i, j;
    str_t v;
    STR_BOOL flg;
    ini_t f;
    ini_init_fname(&f, "cfg.ini");

    // read from global section
    v = ini_read_value(&f, "", "global", "???");
    if (!str_is_equal_cstr(&v, "123 456"))
    { // error
      printf("error: ini_read_value() = '%s'\n", str_c(&v));
      g_err++;
    }
    str_free(&v);
    v = ini_read_str(&f, "", "hard_str", "???");
    if (!str_is_equal_cstr(&v, "\n1 23\\456\\789 ;# "))
    { // error
      printf("error: ini_read_str() = '%s'\n", str_c(&v));
      g_err++;
    }
    str_free(&v);

    // read from end section
    v = ini_read_value(&f, "end sectIon ", "value", "???");
    if (!str_is_equal_cstr(&v, "end"))
    { // error
      printf("error: ini_read_value() = '%s'\n", str_c(&v));
      g_err++;
    }
    str_free(&v);
    
    // read "matrix"
    for (i = 0; i < 3; i++)
    {
      str_t section = str_cstr("s ");
      *str_at(&section, 1) = '0' + i;
      for (j = 0; j < 3; j++)
      {
	int a_i;
	float a_f;
	double a_d;
	char value[3];
	str_t ident = str_cstr("i ");
        value[0] = '0' + i;
        value[1] = '0' + j;
        value[2] = 0;

        *str_at(&ident, 1) = '0' + j;
	v = ini_read_value(&f, str_c(&section), str_c(&ident), "???");
	a_i = ini_read_long(&f, str_c(&section), str_c(&ident), 999);
	a_f = ini_read_float(&f, str_c(&section), str_c(&ident), 999);
	a_d = ini_read_double(&f, str_c(&section), str_c(&ident), 999);
	//printf("[%s]: %s = '%s'\n", str_c(&section), str_c(&ident), str_c(&v));
	//printf("[%s]: %s = '%i'\n", str_c(&section), str_c(&ident), a);
        
	// compare
	if (!str_is_equal_cstr(&v, value) || \
	    a_i != (i * 10 + j) || a_d != a_f || a_i != a_f)
	{ // error
          g_err++;
	}

	str_free(&v);
	str_free(&ident);
      }
      str_free(&section);
    }
    printf("ini_read_value()  [ok]\n");
    printf("ini_read_long()   [ok]\n");
    printf("ini_read_float()  [ok]\n");
    printf("ini_read_double() [ok]\n");

    // read strings
    v = ini_read_str(&f, "sec", "str1", "qqq");
    if (!str_is_equal_cstr(&v, "some string #1 "))
    { // error
      printf("ini_read_str() = '%s'\n", str_c(&v));
      g_err++;
    }
    str_free(&v);
    v = ini_read_str(&f, "sec", "str2", "");
    if (!str_is_equal_cstr(&v, " some string #2"))
    { // error
      printf("ini_read_str() = '%s'\n", str_c(&v));
      g_err++;
    }
    str_free(&v);
    v = ini_read_str(&f, "sec", "str3", "qqq");
    if (!str_is_equal_cstr(&v, "some string -3"))
    { // error
      printf("ini_read_str() = '%s'\n", str_c(&v));
      g_err++;
    }
    str_free(&v);
    printf("ini_read_str() [ok]\n");

    // read bool
    flg = ini_read_bool(&f, "bool", "flag1", STR_BOOL_FALSE);
    if (flg != STR_BOOL_TRUE)
      g_err++;
    else
      printf("ini_read_bool() [ok]\n");
    flg = ini_read_bool(&f, "bool", "flag2", STR_BOOL_TRUE);
    if (flg != STR_BOOL_FALSE)
      g_err++;
    else
      printf("ini_read_bool() [ok]\n");
    
    ini_free(&f);
  }
  
  if (1)
  { // "initile_t" - ini_load(...)
    int i, j;
    int err = 0;
    char *str = 
      "[s1] ; line 1\n"
      "a1=11 ; line 2\n"
      "a2=12 ; line 3\n"
      "# line 4\n"
      "[s2] ; line 5\n"
      "a1=21 ; line 6\n"
      "a2=22 ; line 7\n";
    ini_t f;
    ini_init_fname(&f, "matrix.ini");
    ini_load(&f, str);
  
    for (i = 1; i <= 2; i++)
    {
      str_t section = str_cstr("s ");
      *str_at(&section, 1) = '0' + i;
      for (j = 1; j <= 2; j++)
      {
	int a;
	str_t ident = str_cstr("a ");

        *str_at(&ident, 1) = '0' + j;
	a = ini_read_long(&f, str_c(&section), str_c(&ident), 999);
	//printf("[%s]: %s = '%i'\n", str_c(&section), str_c(&ident), a);
        
	// compare
	if (a != (i * 10 + j))
	{ // error
	  printf("error in ini_load() ([%s]: %s = '%i')\n", str_c(&section), str_c(&ident), a);
	  err++;
          g_err++;
	}

	str_free(&ident);
      }
      str_free(&section);
    }
    
    ini_write_str(&f, "", "first ident", "hello world");
    ini_write_str(&f, "s2", "additional", "end of section");
    ini_write_float (&f, "math", "Pi_f", M_PI);
    ini_write_double(&f, "math", "Pi_d", M_PI);
    ini_write_double(&f, "math", "Pi_d * 1e-12", M_PI * 1e-12);
    ini_write_float (&f, "math", "123.456_f", 123.456);
    ini_write_double(&f, "math", "123.456_d", 123.456);
    
    if (!ini_save(&f))
    {
      err++;
      g_err++;
    }
    ini_free(&f);
    if (err == 0)
      printf("ini_load() [ok]\n");
  }

#ifdef STR_INT64
  if (1)
  {
     ini_t f;
     STR_INT64 i = 0x87654321, j;
     j = i = (i << 32) + 0x00ABCDEF;
     ini_init_fname(&f, "cfg.ini");
     
     ini_write_int64(&f, "int64", "i64", i);
     i = ini_read_int64(&f, "int64", "i64", 123);
     if (i != j)
     {
       printf("error in int64: %LX != %LX)\n", i, j);
       g_err++;
     }
     else
       printf("read/write int64 [ok]\n");
     ini_free(&f);
  }

#ifdef STR_EXTRA
  if (1)
  {
     ini_t f;
     str_t bin;
     STR_INT64 *pi, i = 0x87654321;
     i = (i << 32) + 0x00ABCDEF;
     ini_init_fname(&f, "cfg.ini");
     
     ini_write_bin(&f, "int64", "bin", (const char*) &i, sizeof(i));
     bin = ini_read_bin(&f, "int64", "bin", "qaqq", 1);
     pi = (STR_INT64 *) str_c(&bin);
     if (i != *pi)
     {
       printf("error in bin: %LX != %LX)\n", i, *pi);
       g_err++;
     }
     else
       printf("read/write bin [ok]\n");
     str_free(&bin);
     ini_free(&f);
  }
#endif // STR_EXTRA
#endif // STR_INT64

  if (1)
  {
    ini_t f;
    ini_init_fname(&f, "cfg.ini");
     
    if (!ini_has_section(&f, " int64 "))
    {
      printf("error in ini_has_section()\n");
      g_err++;
    }
     
    if (ini_has_section(&f, "int64_"))
    {
      printf("error in ini_has_section()\n");
      g_err++;
    }
  
    if (!ini_has_ident(&f, "int64", " i64"))
    {
      printf("error in ini_has_ident()\n");
      g_err++;
    }
     
    if (ini_has_ident(&f, "int64", "i64_"))
    {
      printf("error in ini_has_ident()\n");
      g_err++;
    }
     
    if (ini_has_ident(&f, "_int64", "i64"))
    {
      printf("error in ini_has_ident()\n");
      g_err++;
    }

    printf("ini_has_section()/ini_has_ident() [ok]\n");
    ini_free(&f);
  }

#ifdef STR_DEBUG
  if(1)
  {
    printf("\n");
    printf("str_malloc_num   = %u\n", str_malloc_num());
    printf("str_malloc_bytes = %u\n", str_malloc_bytes());
    if (str_malloc_num() != 0 || str_malloc_bytes() != 0)
      g_err++;
  }
#endif // STR_DEBUG

  if (g_err == 0)
  {
    printf("\n>>> sucsess");
    printf("\n    -------\n");
  }
  else
    printf("\n>>> errors = %i\n", g_err);

  return g_err;
}

/*** end of "test_ini.c" ***/
