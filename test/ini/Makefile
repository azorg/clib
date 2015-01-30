OUT_NAME     := test_ini
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
        test_ini.c

HDRS := ../../str.h \
	      ../../ini.h
#----------------------------------------------------------------------------
DEFS     := -DSTR_DEBUG -DSTR_DEF_SECTOR_SIZE="2" \
            -D__STDC_FORMAT_MACROS
OPTIM    := -Os -fomit-frame-pointer
WARN     := -Wall

CFLAGS   := $(WARN) $(OPTIM) $(DEFS) $(CFLAGS) -pipe
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
LDFLAGS  :=
#----------------------------------------------------------------------------
#_CC  := @gcc
#_CXX := @g++
#_LD  := @gcc

#_CC  := @gcc-2.95
#_LD  := @gcc-2.95
#----------------------------------------------------------------------------
include ../Makefile.skel
#----------------------------------------------------------------------------

