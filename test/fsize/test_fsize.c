/*
 * Unit test of `fsize` module
 */

//----------------------------------------------------------------------------
#include "fsize.h"
//----------------------------------------------------------------------------
#define FILE_NAME "test_fsize.c"
#define FILE_SIZE 1160 // FIXME: magic constant!
//----------------------------------------------------------------------------
int main()
{
  const char *fname = FILE_NAME;
  FILE *file;

  long size = fsize_name(fname);
  if (size == -1)
  {
    fprintf(stderr, "error: can't open file, fsize_name() test FAIL\n");
    return -1;
  }

  printf("fsize_name('%s') = %li\n", fname, size);

  if (size == FILE_SIZE)
    printf("fsize_name() test OK\n");
  else
    printf("fsize_name() test FAIL\n");

  file = fopen(FILE_NAME, "rb");
  if (file == (FILE*) NULL)
  {
    fprintf(stderr, "error: can't open file, test FAIL\n");
    return -1;
  }

  size = fsize(file);
  if (size == FILE_SIZE)
    printf("fsize() test OK\n");
  else
    printf("fsize() test FAIL\n");

  fclose(file);

  return 0;
}
//----------------------------------------------------------------------------

/*** end of "test_fsize.c" ***/
