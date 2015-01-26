OUT_NAME     := test_str
OUT_DIR      := .
#INC_DIRS    :=
CLEAN_FILES := test_file?.txt "$(OUT_DIR)/$(OUT_NAME).exe"

# 1-st way to select source files
#SRC_DIRS := ../.. .
#HDR_DIRS := ../..

# 2-nd way to select source files
SRCS := ../../str.c \
        test_str.c

HDRS := ../../str.h

DEFS    := -DSTR_DEBUG -DSTR_DEBUG_EXTRA -DSTR_DEF_SECTOR_SIZE="7"
#-DSTR_INLINE="static inline"
OPT     := -g -O0 -std=gnu90
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

