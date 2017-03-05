#include <RtWin32Ex.h>

void RT_CALL ZzDisplayFlags()
{
#ifdef RT_DEFINE_DEBUG
  RtWriteStringToConsole(_R("Debug build.\n"));
#else
  RtWriteStringToConsole(_R("Release build.\n"));
#endif

#ifdef RT_DEFINE_32
  RtWriteStringToConsole(_R("Architecture = 32 bits.\n"));
#else
  RtWriteStringToConsole(_R("Architecture = 64 bits.\n");
#endif

#ifdef RT_DEFINE_WINDOWS
  RtWriteStringToConsole(_R("Operating system = Windows.\n"));
#else
  RtWriteStringToConsole(_R("Operating system = Linux.\n"));
#endif

#ifdef RT_DEFINE_VC
  RtWriteStringToConsole(_R("Compiler = Visual C++.\n"));
#else
  RtWriteStringToConsole(_R("Compiler = GCC.\n"));
#endif

#ifdef RT_DEFINE_USE_CRT
  RtWriteStringToConsole(_R("Use CRT = true.\n"));
#else
  RtWriteStringToConsole(_R("Use CRT = false.\n"));
#endif
}

RT_B RT_CALL ZzManualTests()
{
  ZzDisplayFlags();
  return RT_SUCCESS;
}