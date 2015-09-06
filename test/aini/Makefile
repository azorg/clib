OUT_NAME     := test_aini
OUT_DIR      := .
#INC_DIRS    :=
CLEAN_FILES := "$(OUT_DIR)/$(OUT_NAME).exe"
#----------------------------------------------------------------------------
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
#----------------------------------------------------------------------------
#INC_DIRS  := ../libs/include 
#INC_FLAGS := -I/usr/include/foo
DEFS     := -DSTR_DEBUG -DSTR_DEF_SECTOR_SIZE="3" \
            -D__STDC_FORMAT_MACROS
OPTIM    := -Os -fomit-frame-pointer
WARN     := -Wall

CFLAGS    := $(WARN) $(OPTIM) $(DEFS) $(CFLAGS) -pipe
CXXFLAGS  := $(CXXFLAGS) $(CFLAGS)
LDFLAGS   := -lm $(LDFLAGS)
#----------------------------------------------------------------------------
#_CC  := @gcc
#_CXX := @g++
_LD  := @g++

#_CC  := @clang
#_CXX := @clang++
#_LD  := @clang++

#_CC  := @gcc-2.95
#_LD  := @gcc-2.95
#----------------------------------------------------------------------------
include ../Makefile.skel
#----------------------------------------------------------------------------
