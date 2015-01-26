/*
 * Unit test of `aclass::aini` class
 */


#include <math.h>
#include "aini.h"

#ifndef M_PI
#define M_PI 3.1415
#endif

int main()
{
  int g_err = 0;

  if (1)
  { // write
    int i, j;
    aclass::aini f("cfg.ini");

    f.write_value("", "global", "bad data");
    f.write_value("s0", "i0", "XXXXX");
    f.write_value("s2", "i2", "YYYYY");
    
    // write "matrix"
    for (i = 0; i < 3; i++)
    {
      std::string section = "s ";
      section[1] = '0' + i;
      for (j = 0; j < 3; j++)
      {
        char value[3];
        std::string ident = "i ";

        value[0] = '0' + i;
        value[1] = '0' + j;
        value[2] = 0;

        ident[1] = '0' + j;
        f.write_value(section, ident, value);
      }
    }
    
    f.write_value("", " global ", " 123 456 ");
    //f.write_str("", "hard_str", "\n1 23\\456\\789 ;# ");
    f.write_str("", "hard_str", std::string("\n1 23\\456\\789 ;# "));
    
    f.write_str  ("sec", "str1", "some string #1 ");
    f.write_str  ("sec", "str2", " some string #2");
    f.write_value("sec", "str3", "some string -3 ");
    
    f.write_bool("bool", "flag1", true);
    f.write_bool("bool", "flag2", false);

    f.write_value("end sectIon ", "value", "end");
  }

  if (1)
  { // "read"
    int i, j;
    std::string v;
    bool flg;
    aclass::aini f("cfg.ini");

    // read from global section
    v = f.read_value("", "global", "???");
    if (v != "123 456")
    { // error
      printf("error: read_value() = '%s'\n", v.c_str());
      g_err++;
    }
    v = f.read_str("", "hard_str", "???");
    if (v != "\n1 23\\456\\789 ;# ")
    { // error
      printf("0. error: read_str() = '%s'\n", v.c_str());
      g_err++;
    }

    // read from end section
    v = f.read_value("end sectIon ", "value", "???");
    if (v != "end")
    { // error
      printf("error: read_value() = '%s'\n", v.c_str());
      g_err++;
    }
    
    // read "matrix"
    for (i = 0; i < 3; i++)
    {
      std::string section = "s ";
      section[1] = '0' + i;
      for (j = 0; j < 3; j++)
      {
        int a_i;
        float a_f;
        double a_d;
        char value[3];
        std::string ident = "i ";
        value[0] = '0' + i;
        value[1] = '0' + j;
        value[2] = 0;

        ident[1] = '0' + j;
        v = f.read_value(section, ident, "???");
        a_i = f.read_long(section, ident, 999);
        a_f = f.read_float(section, ident, 999);
        a_d = f.read_double(section, ident, 999);
        //printf("[%s]: %s = '%s'\n", section.c_str(), ident.c_str(), v.c_str());
        //printf("[%s]: %s = '%i'\n", section.c_str(), ident.c_str(), a);
        
        // compare
        if (v != value || \
            a_i != (i * 10 + j) || a_d != a_f || a_i != a_f)
        { // error
          printf("error in read matrix ('%s')\n", v.c_str());
          g_err++;
        }
      }
    }
    
    printf("read_value()  [ok]\n");
    printf("read_long()   [ok]\n");
    printf("read_float()  [ok]\n");
    printf("read_double() [ok]\n");

    // read strings
    v = f.read_str("sec", "str1", "qqq");
    if (v != "some string #1 ")
    { // error
      printf("1. error: read_str() = '%s'\n", v.c_str());
      g_err++;
    }
    v = f.read_str("sec", "str2", "");
    if (v != " some string #2")
    { // error
      printf("2. error: read_str() = '%s'\n", v.c_str());
      g_err++;
    }
    v = f.read_str("sec", "str3", "qqq");
    if (v != "some string -3")
    { // error
      printf("3. error: read_str() = '%s'\n", v.c_str());
      g_err++;
    }
    printf("read_str() [ok]\n");

    // read bool
    flg = f.read_bool("bool", "flag1", false);
    if (flg != true)
    {
      printf("1. error: read_bool() = '%i'\n", (int) !!flg);
      g_err++;
    }
    else
      printf("read_bool() [ok]\n");
    flg = f.read_bool("bool", "flag2", true);
    if (flg != false)
    {
      printf("2. error: read_bool() = '%i'\n", (int) !!flg);
      g_err++;
    }
    else
      printf("read_bool() [ok]\n");
  }
  
  if (1)
  { // load
    int i, j;
    int err = 0;
    const char *str = 
      "[s1] ; line 1\n"
      "a1=11 ; line 2\n"
      "a2=12 ; line 3\n"
      "# line 4\n"
      "[s2] ; line 5\n"
      "a1=21 ; line 6\n"
      "a2=22 ; line 7\n";
    aclass::aini f;
    f.set_fname("matrix.ini");
    f.load(str);
  
    for (i = 1; i <= 2; i++)
    {
      std::string section = "s ";
      section[1] = '0' + i;
      for (j = 1; j <= 2; j++)
      {
        int a;
        std::string ident = "a ";

        ident[1] = '0' + j;
        a = f.read_long(section, ident, 999);
        //printf("[%s]: %s = '%i'\n", section.c_str(), ident.c_str(), a);
        
        // compare
        if (a != (i * 10 + j))
        { // error
          printf("error in ini_load() ([%s]: %s = '%i')\n",
                 section.c_str(), ident.c_str(), a);
          err++;
          g_err++;
        }
      }
    }
    
    f.write_str("", "first ident", "hello world");
    f.write_str("s2", "additional", "end of section");
    f.write_float ("math", "Pi_f", (float) M_PI);
    f.write_double("math", "Pi_d", M_PI);
    f.write_double("math", "Pi_d * 1e-12", M_PI * 1e-12);
    f.write_float ("math", "123.456_f", (float) 123.456);
    f.write_double("math", "123.456_d", 123.456);
    
    if (!f.save(true))
    {
      err++;
      g_err++;
    }
    if (err == 0)
      printf("ini_load() [ok]\n");
  }

#ifdef STR_INT64
  if (1)
  {
     aclass::aini f;
     STR_INT64 i = 0x87654321, j;
     j = i = (i << 32) + 0x00ABCDEF;
     f.set_fname("cfg.ini");
     
     f.write_int64("int64", "i64", i);
     i = f.read_int64("int64", "i64", 123);
     if (i != j)
     {
       printf("error in int64: %" STR_PRIx64 " != %" STR_PRIx64 "\n", i, j);
       g_err++;
     }
     else
       printf("read/write int64 [ok]\n");
  }

#ifdef STR_EXTRA
  if (1)
  {
     aclass::aini f;
     std::string bin;
     STR_INT64 *pi, i = 0x87654321;
     i = (i << 32) + 0x00ABCDEF;
     f.set_fname("cfg.ini");
     
     f.write_bin("int64", "bin", (const char*) &i, sizeof(i));
     bin = f.read_bin("int64", "bin", "qaqq", 1);
     pi = (STR_INT64 *) bin.c_str();
     if (i != *pi)
     {
       printf("error in bin: %" STR_PRIx64 " != %" STR_PRIx64 "\n", i, *pi);
       g_err++;
     }
     else
       printf("read/write bin [ok]\n");
  }
#endif // STR_EXTRA
#endif // STR_INT64

  if (1)
  {
    aclass::aini f("cfg.ini");
     
    if (!f.has_section(" int64 "))
    {
      printf("error in has_section()\n");
      g_err++;
    }
     
    if (f.has_section("int64_"))
    {
      printf("error in has_section()\n");
      g_err++;
    }
  
    if (!f.has_ident("int64", " i64"))
    {
      printf("error in has_ident()\n");
      g_err++;
    }
     
    if (f.has_ident("int64", "i64_"))
    {
      printf("error in has_ident()\n");
      g_err++;
    }
     
    if (f.has_ident("_int64", "i64"))
    {
      printf("error in has_ident()\n");
      g_err++;
    }

    printf("has_section()/has_ident() [ok]\n");
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

  return 0;
}

/*** end of "test_aini.cpp" ***/

