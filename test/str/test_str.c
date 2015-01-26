/*
 * Unit test of `str_t` component
 */

#include "str.h"

int g_err = 0; // number of errors

void print_ok(int ok)
{
  if (ok != 0)
    printf("     [ ok ]\n");
  else
  {
    printf("     [ false ]\n");
    g_err++;
  }
}

int main()
{
  int def_sec;
  for (def_sec = 1; def_sec <= 16; def_sec++)
  {
    str_set_def_sector(def_sec);
    printf("\nstr_set_def_sector(%i) ------------------------------ ", def_sec);
    print_ok(str_get_def_sector() == def_sec);

    if (1)
    { // str_sprintf("%s", ""), str_c()
      str_t s;
      s = str_sprintf("%s", "");
      printf("str_sprintf('s', '') = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, ""));
      str_free(&s);

      s = str_sprintf("%s", "Hello!");
      printf("str_sprintf('s', 'Hello!') = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "Hello!"));
      str_free(&s);
    }

#ifdef STR_INT64
    if (1)
    { // int64
      str_t s;
      STR_UINT64 i64;
      STR_INT64  u64;
      printf("sezeof(STR_INT64) = %i\n", (int) sizeof(STR_INT64));

      i64 = 1234567890123456789LL;
      s = str_int64(i64);
      printf("str_int64(1234567890123456789) = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "1234567890123456789"));
      str_free(&s);

      u64 = 18446700000000000000ULL;
      s = str_uint64(u64);
      printf("str_int64(18446700000000000000ULL) = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "18446700000000000000"));
      str_free(&s);

      i64 = 0x12345678aBcDEf0FLL; // int64
      s = str_int64_hex(i64);
      printf("str_int64_hex(0x12345678aBcDEf0FLL) = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "12345678ABCDEF0F"));
      str_free(&s);
    }
#endif

    if (1)
    { // str_to_long_cstyle(), str_bin(), str_hex(), str_bin(), str_dec(), ...
      long li;
			str_t s, q, w;

      s = str_cstr("0xDEADBEEF");
      li = str_to_long_cstyle(&s, 321, 10);
      printf("str_to_long_cstyle('0xDEADBEEF') = '%lX'", li);
      print_ok(li == 0xDEADBEEF);
      q = str_hex((unsigned long) li, 8);
      printf("str_hex(0x%lX) = '%s'", li, str_c(&q));
      str_delete(&s, 0, 2); // delete "0x" 
      print_ok(str_is_equal(&s, &q));
      str_free(&s);
      str_free(&q);
      
      s = str_cstr("BedaBeda");
      li = str_to_long_cstyle(&s, 321, 16);
      printf("str_to_long_cstyle('BedaBeda') = '%lX'", li);
      print_ok(li == 0xBEDABEDA);
      q = str_hex((unsigned long) li, 8);
      printf("str_hex(0x%lX) = '%s'", li, str_c(&q));
      w = str_upper_case(&s);
      print_ok(str_is_equal(&q, &w));
      str_free(&w);
      str_free(&s);
      str_free(&q);

      s = str_cstr("123456789");
      li = str_to_long_cstyle(&s, 321, 10);
      printf("str_to_long_cstyle('123456789') = '%li'", li);
      print_ok(li == 123456789);
      q = str_dec((unsigned long) li, 9);
      printf("str_dec(%li) = '%s'", li, str_c(&q));
      print_ok(str_is_equal(&q, &s));
      str_free(&s);
      str_free(&q);
			
      s = str_cstr("01234567");
      li = str_to_long_cstyle(&s, 321, 2);
      printf("str_to_long_cstyle('01234567') = '0%lo'", li);
      print_ok(li == 01234567);
      q = str_oct((unsigned long) li, 8);
      printf("str_oct(0%lo) = '%s'", li, str_c(&q));
      print_ok(str_is_equal(&q, &s));
      str_free(&s);
      str_free(&q);
			
      s = str_cstr("1111100110101");
      li = str_to_long_cstyle(&s, 0xF0F0, 2);
      q  = str_bin((unsigned long) li, 13);
      printf("str_to_long_cstyle('1111100110101', 2) = '%s'", str_c(&q));
      print_ok(str_is_equal(&s, &q));
      str_free(&s);
      str_free(&q);
      
      s = str_cstr("0b10100110001110000111100000111110");
      li = str_to_long_cstyle(&s, 0xF0F0, 8);
      q  = str_bin((unsigned long) li, 32);
      printf("str_to_long_cstyle('0b10100110001110000111100000111110') = '%s'",
				     str_c(&q));
      str_delete(&s, 0, 2); // delete "0b" 
      print_ok(str_is_equal(&s, &q));
      str_free(&s);
      str_free(&q);
    }

    if (1)
    { // str_int(), str_c()
      str_t s;
      s = str_int(12345);
      printf("str_int(12345) = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "12345"));
      print_ok(str_is_equal_cstr(&s, str_c(&s)));
      str_free(&s);

      str_init_cstr(&s, "Hello world!");
      printf("str_cstr('Hello world!') = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "Hello world!"));
      str_free(&s);
      
      str_init_size_cstr(&s, 10, "Hello!");
      printf("str_size_cstr(10, 'Hello!)' = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "Hello!"));
      str_free(&s);
      
      str_init_size_cstr(&s, 3, "OneOne");
      printf("str_size_cstr(3, 'OneOne') = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "One"));
      str_free(&s);
    }
  
    if (1)
    { // str_add(), str_add_char, str_add_cstr() 
      str_t s1 = str_cstr("abc");
      str_t s2 = str_cstr("ABC");
      char *s3 = "def";
      char s4 = 'G';
      str_add(&s1, &s2);
      str_add_cstr(&s1, s3);
      str_add_char(&s1, s4);
      printf("str_addX(): 'abc' + 'ABC' + 'def' + 'G' = '%s'", str_c(&s1));
      print_ok(str_is_equal_cstr(&s1, "abcABCdefG"));
      str_free(&s1);
      str_free(&s2);
    }

    if (1)
    {
      str_t s = str_sprintf("%i-%s-%.2f", 123, "456", 78.9);
      printf("str_sprintf() = '%s'", str_c(&s));
      print_ok(str_is_equal_cstr(&s, "123-456-78.90"));
      str_free(&s);
    }
    
    if (1)
    { // str_cstr(), str_sum(), str_swap()
      str_t s1 = str_int(123);
      str_t s2 = str_cstr("456");
      str_t s3 = str_sum("fSsScisdcx", 1., &s1, "+", &s2, '=', 1579, " :-) ",
			 3.14, ' ', 0xDEADBEEF);
      
      printf("str_sum(...) = '%s'", str_c(&s3));
      str_swap(&s1, &s3);
      print_ok(str_is_equal_cstr(&s1,
	       "1123+456=1579 :-) 3.1400000000000001 DEADBEEF"));
      print_ok(str_is_equal_cstr(&s3, "123"));
      
      str_free(&s1);
      str_free(&s2);
      str_free(&s3);
    }

    if (1)
    { // str_insert(), str_delete()
      str_t s1 = str_cstr("123456789");
      str_t s2 = str_cstr("xyz");
      
      str_insert(&s1, &s2, 3);
      printf("str_insert()");
      print_ok(str_is_equal_cstr(&s1, "123xyz456789"));
      
      str_insert_char(&s1, '_', 1);
      printf("str_insert_char() s1 = '%s'", str_c(&s1));
      print_ok(str_is_equal_cstr(&s1, "1_23xyz456789"));
      
      str_delete(&s1, 1, 1);
      str_delete(&s1, 3, 3);
      str_delete(&s1, 6, -1);
      printf("str_delete() s1 = '%s'", str_c(&s1));
      print_ok(str_is_equal_cstr(&s1, "123456"));
      
      str_free(&s1);
      str_free(&s2);
    }

    if (1)
    { // str_upper_case()
      str_t s1 = str_cstr("qqqWWWeeeRRR_123_óÏÂÁËÁ");
      str_t s2 = str_upper_case(&s1);
      str_t s3 = str_lower_case(&s2);
      
      printf("s2 = '%s' (str_upper_case)", str_c(&s2));
      print_ok(str_is_equal_cstr(&s2, "QQQWWWEEERRR_123_óïâáëá"));
      
      printf("s3 = '%s' (str_lower_case)", str_c(&s3));
      print_ok(str_is_equal_cstr(&s3, "qqqwwweeerrr_123_ÓÏÂÁËÁ"));
      
      str_free(&s1);
      str_free(&s2);
      str_free(&s3);
    }
    
    if (1)
    { // str_read_from_file(), str_write_to_file
      
      str_t s1 = str_cstr("íÁÍÁ ÍÙÌÁ ÒÁÍÕ!\n÷ÏÔ ôáë!\n");
      str_t s2 = str_zero();
      str_t fname = str_cstr("test_file1.txt");

      str_write_to_file(&s1, str_c(&fname), 0);
      str_read_from_file(&s2, str_c(&fname), 0);

      printf("s2 = %s  ", str_c(&s2));
      print_ok(str_is_equal(&s1, &s2));
      
      str_assign_cstr(&fname, "test_file2.txt");
      str_write_to_bin_file(&s1, str_c(&fname), 0);
      str_read_from_bin_file(&s2, str_c(&fname), 0);

      printf("s2 = %s  ", str_c(&s2));
      print_ok(str_is_equal(&s1, &s2));
      
      str_free(&s1);
      str_free(&s2);
      str_free(&fname);
    }
    
#ifdef STR_EXTRA
    if (1)
    {
      long long *pv64, v64 = 0x12345678;
      v64 = (v64 << 32) + 0x00ABCDEF;
      str_t s1 = str_bin2hex((const char *) &v64, sizeof(v64));
      str_t s2 = str_hex2bin(str_c(&s1));
      pv64 = (long long *) str_c(&s2);
      printf("str_hex2bin(), str_bin2hex() [%s]: ", str_c(&s1));
      print_ok(v64 == *pv64);
      str_free(&s1);
      str_free(&s2);
    }
#endif // STR_EXTRA

    if (1)
    {
      str_t s = str_cstr("true");
      STR_BOOL b = str_to_bool(&s);
      if (!b) g_err++;
      printf("\n");
      printf("str_to_bool('true') = %u\n", (int) b);
      str_free(&s);

      s = str_cstr("false");
      b = str_to_bool(&s);
      if (b) g_err++;
      printf("str_to_bool('false') = %u\n", (int) b);
      str_free(&s);

      s = str_cstr("123");
      b = str_to_bool(&s);
      if (!b) g_err++;
      printf("str_to_bool('123') = %u\n", (int) b);
      str_free(&s);

      s = str_cstr("00");
      b = str_to_bool(&s);
      if (b) g_err++;
      printf("str_to_bool('00') = %u\n", (int) b);
      str_free(&s);
    }
    //break; //!!!
  } // for(..
    
#ifdef STR_DEBUG
  if (1)
  {
    if (_str_malloc_num != 0 || _str_malloc_bytes != 0)
      g_err++;
    printf("\n");
    printf("str_malloc_num   = %u\n", str_malloc_num());
    printf("str_malloc_bytes = %u\n", str_malloc_bytes());
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

