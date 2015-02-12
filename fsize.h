/*
 * File size standard determinator
 * Version: 0.2b
 * File: "fsize.h"
 * (C) 2007-2015 Alex Zorg <azorg@mail.com>
 */

#ifndef FSIZE_H
#define FSIZE_H
//----------------------------------------------------------------------------
#include <stdio.h>
//----------------------------------------------------------------------------
// get file size by name
// (return -1 if open error)
long fsize_name(const char *fname);
//----------------------------------------------------------------------------
// get file size by FILE* stream pointer (position save and restore)
long fsize(FILE *file);
//----------------------------------------------------------------------------
#endif // FSIZE_H

/*** end of "fsize.h" file ***/

