OUT_NAME    := test_fsize
OUT_DIR     := .
#INC_DIRS   :=
CLEAN_FILES := "$(OUT_DIR)/$(OUT_NAME).exe"
#----------------------------------------------------------------------------
# 1-st way to select source files
#SRC_DIRS := ../.. .
#HDR_DIRS := ../..

# 2-nd way to select source files
SRCS := ../../fsize.c \
        test_fsize.c

HDRS := ../../fsize.h
#----------------------------------------------------------------------------
#INC_DIRS  := ../libs/include 
#INC_FLAGS := -I/usr/include/foo -I `wx-config --cxxflags`
#DEFS       := 
#OPTIM     := -Os -fomit-frame-pointer
OPTIM      := -g -O0 -std=gnu90
WARN      := -Wall

CFLAGS    := $(WARN) $(OPTIM) $(DEFS) $(CFLAGS) -pipe
CXXFLAGS  := $(CXXFLAGS) $(CFLAGS)
LDFLAGS   := -lm $(LDFLAGS)
#----------------------------------------------------------------------------
#_CC  := @gcc
#_CXX := @g++
#_LD  := @gcc

#_CC  := @clang
#_CXX := @clang++
#_LD  := @clang

#_CC  := @gcc-2.95
#_LD  := @gcc-2.95
#----------------------------------------------------------------------------
include ../Makefile.skel
#----------------------------------------------------------------------------

