ROOT=../../..
WIN32EX=$(ROOT)/../Win32Ex
OS=win32
include $(WIN32EX)/project/CommonMakefiles/common.Makefile
LDFLAGS=-nostartfiles -nodefaultlibs -nostdlib -ffreestanding -lWin32Ex -luser32 -lkernel32 -L $(WIN32EX)/lib
include $(ROOT)/project/src.Makefile
