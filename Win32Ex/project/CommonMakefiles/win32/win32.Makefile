ifeq ($(MAKECMDGOALS),release)
CC=gcc -O2 -m32 -Wall -ansi -pedantic -fno-strict-aliasing
else
CC=gcc -g -m32 -Wall -ansi -pedantic -fno-strict-aliasing
endif

Path:=$(WIN32EX)/tools;$(Path)
export Path

RM_DIR=rm -rf
RM_FILE=rm -f
LD_STATIC=ar -ru
LD_EXE=$(CC)
MK_DIR=mkdir -p

OBJ_EXT=.o
STATIC_EXT=.a
SHARED_EXT=.dll
EXE_EXT=.exe
