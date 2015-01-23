OUT_NAME     := test_str
OUT_DIR      := .
#INC_DIRS    :=
CLEAN_FILES := test_file?.txt

# 1-st way to select source files
#SRC_DIRS := .

# 2-nd way to select source files
SRCS := ../../str.c \
        test_str.c

DEFS    := -DSTR_DEBUG -DSTR_DEBUG_EXTRA -DSTR_DEF_SECTOR_SIZE="8"
#-DSTR_INLINE="static inline"
OPT    := -g -O0
#OPT     := -Os -fomit-frame-pointer
CFLAGS  := $(DEFS) $(OPT) -Wall -pipe
LDFLAGS :=

#_CC  := @gcc
#_CXX := @g++
#_LD  := @gcc

#_CC  := @g++
#_CXX := @g++
#_LD  := @g++

#_CC  := @gcc-2.95
#_LD  := @gcc-2.95
#----------------------------------------------------------------------------
include ../Makefile.skel
#----------------------------------------------------------------------------

