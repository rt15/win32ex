#ifndef ZZ_TESTS_H
#define ZZ_TESTS_H

#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestTypes();
RT_B RT_CALL ZzTestMemory();
RT_B RT_CALL ZzTestRandom();
RT_B RT_CALL ZzTestHeap(RT_HEAP** lpHeap);
RT_B RT_CALL ZzTestIoDevice();
RT_B RT_CALL ZzTestPipe();
RT_B RT_CALL ZzTestFile();
RT_B RT_CALL ZzTestFileSystem();
RT_B RT_CALL ZzTestChar();
RT_B RT_CALL ZzTestBase64();
RT_B RT_CALL ZzTestEncoding();
RT_B RT_CALL ZzTestSmallFile(RT_HEAP** lpHeap);
RT_B RT_CALL ZzTestProperties(RT_HEAP** lpHeap);
RT_B RT_CALL ZzTestAtomic();
RT_B RT_CALL ZzTestThread();
RT_B RT_CALL ZzTestInitialization();
RT_B RT_CALL ZzTestEvent();
RT_B RT_CALL ZzTestUrl();
RT_B RT_CALL ZzTestSocket();
RT_B RT_CALL ZzTestNonBlockingSocket();
RT_B RT_CALL ZzTestList(RT_HEAP** lpHeap);
RT_B RT_CALL ZzTestEnvVar();
RT_B RT_CALL ZzTestEnvVars();
RT_B RT_CALL ZzTestCommandLineArgs();
RT_B RT_CALL ZzTestProcess(RT_HEAP** lpHeap);

#endif /* ZZ_TESTS_H */
