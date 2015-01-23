
#include "str.h"

int g_retv = 0;

void print_ok(int ok)
{
  if (ok != 0)
    printf("     [ ok ]\n");
  else
  {
    printf("     [ false ]\n");
    g_retv = -1;
  }
}

int main()
{

  if (1)
  { // str_init(), str_c(), 
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
    str_t s1 = str_cstr("qqqWWWeeeRRR_123_Собака");
    str_t s2 = str_upper_case(&s1);
    str_t s3 = str_lower_case(&s2);
    
    printf("s2 = '%s' (str_upper_case)", str_c(&s2));
    print_ok(str_is_equal_cstr(&s2, "QQQWWWEEERRR_123_СОБАКА"));
    
    printf("s3 = '%s' (str_lower_case)", str_c(&s3));
    print_ok(str_is_equal_cstr(&s3, "qqqwwweeerrr_123_собака"));
    
    str_free(&s1);
    str_free(&s2);
    str_free(&s3);
  }
  
  if (1)
  { // str_read_from_file(), str_write_to_file
    
    str_t s1 = str_cstr("Мама мыла раму!\nВот ТАК!\n");
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
    if (!b) g_retv = -1;
    printf("\n");
    printf("str_to_bool('true') = %u\n", (int) b);
    str_free(&s);

    s = str_cstr("false");
    b = str_to_bool(&s);
    if (b) g_retv = -1;
    printf("str_to_bool('false') = %u\n", (int) b);
    str_free(&s);

    s = str_cstr("123");
    b = str_to_bool(&s);
    if (!b) g_retv = -1;
    printf("str_to_bool('123') = %u\n", (int) b);
    str_free(&s);

    s = str_cstr("00");
    b = str_to_bool(&s);
    if (b) g_retv = -1;
    printf("str_to_bool('00') = %u\n", (int) b);
    str_free(&s);
  }
  
#ifdef STR_DEBUG
  if (1)
  {
    if (str_malloc_num != 0 || str_malloc_bytes != 0)
      g_retv = -1;
    printf("\n");
    printf("str_malloc_num = %u\n", str_malloc_num);
    printf("str_malloc_bytes = %u\n", str_malloc_bytes);
  }
#endif // STR_DEBUG

  if (g_retv == 0)
  {
    printf("\n>>> sucsess");
    printf("\n    -------\n");
  }

  return g_retv;
}

