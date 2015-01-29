OUT_NAME     := test_aini
OUT_DIR      := .
#INC_DIRS    :=
CLEAN_FILES := "$(OUT_DIR)/$(OUT_NAME).exe"

# 1-st way to select source files
#SRC_DIRS := ../.. .
#HDR_DIRS := ../..

# 2-nd way to select source files
SRCS := ../../str.c \
        ../../ini.c \
        test_aini.cpp

HDRS := ../../str.h \
        ../../ini.h \
        ../../aini.h

DEFS    := -DSTR_DEBUG -DSTR_DEF_SECTOR_SIZE="3" 

#           -D__USE_MINGW_ANSI_STDIO 1
#           -DINI_INLINE="static inline"

OPT     := -Os -fomit-frame-pointer
WARN    := -Wall

CFLAGS   := $(DEFS) $(OPT) -pipe $(WARN)
CXXFLAGS := $(DEFS) $(OPT) -pipe $(WARN) $(CXXFLAGS)
LDFLAGS  := -lm

#_CC  := @gcc
#_CXX := @g++
_LD  := @g++

#_CC  := @gcc-2.95
#_LD  := @gcc-2.95
#----------------------------------------------------------------------------
include ../Makefile.skel
#----------------------------------------------------------------------------
