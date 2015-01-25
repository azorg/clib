OUT_NAME     := ini_test
OUT_DIR      := .
#INC_DIRS    :=
CLEAN_FILES := "$(OUT_DIR)/$(OUT_NAME).exe"

# 1-st way to select source files
#SRC_DIRS := . 

# 2-nd way to select source files
SRCS := ../../str.c \
        ../../ini.c \
	test_aini.cpp

DEFS    := -DSTR_DEBUG -DSTR_DEF_SECTOR_SIZE="3"
#-DINI_INLINE="static inline"
OPT     := -Os -fomit-frame-pointer
CFLAGS   := $(DEFS) $(OPT) -pipe -Wall
CXXFLAGS := $(DEFS) $(OPT) -pipe -Wall
LDFLAGS  :=

#_CC  := @gcc
#_CXX := @g++
_LD  := @g++

#_CC  := @gcc-2.95
#_LD  := @gcc-2.95
#----------------------------------------------------------------------------
include ../Makefile.skel
#----------------------------------------------------------------------------

