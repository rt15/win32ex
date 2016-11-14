OUTPUT_SUFFIX=d
OUTPUT_DIR=$(ROOT)/obj/Debug/src
ifeq ($(MAKECMDGOALS),release)
OUTPUT_SUFFIX=
OUTPUT_DIR=$(ROOT)/obj/Release/src
endif

ifeq "$(OS)" "win32"
include $(WIN32EX)/project/CommonMakefiles/win32/win32.Makefile
endif
ifeq "$(OS)" "linux32"
include $(WIN32EX)/project/CommonMakefiles/linux32/linux32.Makefile
endif
