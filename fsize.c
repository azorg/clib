/*
 * File size standard determinator
 * Version: 0.2b
 * File: "fsize.h"
 * (C) 2007-2015 Alex Zorg <azorg@mail.com>
 */

//----------------------------------------------------------------------------
#include "fsize.h"
//----------------------------------------------------------------------------
long fsize_name(const char *fname)
{
  long size;
  FILE *file = fopen(fname, "rb");

  if (file != (FILE*) NULL)
  {
    fseek(file, 0, SEEK_END);
    size = ftell(file);
  }
  else
    size = -1; // error

  fclose(file);
  return size;
}
//----------------------------------------------------------------------------
long fsize(FILE *file)
{
  long size, pos;
  pos = ftell(file); // save position
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fseek(file, pos, SEEK_SET); // restore position
  return size;
}
//----------------------------------------------------------------------------

/*** end of "fsize.c" file ***/

