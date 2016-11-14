#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>

/*
typedef struct _Ldc_User
{
  RT_CHAR8* _lpName;
  RT_UN32 _nDate;
}
Ldc_User;

Rt_SerializableType charType = {RT_SERIALIZABLE_TYPE_TYPE_PRIMITIVE, RT_NULL, RT_SERIALIZABLE_TYPE_PRIMITIVE_SCHAR};
Rt_SerializableType uIntType = {RT_SERIALIZABLE_TYPE_TYPE_PRIMITIVE, RT_NULL, RT_SERIALIZABLE_TYPE_PRIMITIVE_UN32};
Rt_SerializableType stringType = {RT_SERIALIZABLE_TYPE_TYPE_DYNAMIC_ARRAY, &charType, 0};
Rt_SerializableField userFields[2] = {{"name", &stringType}, {"date", &uIntType}};
Rt_SerializableType userType = {RT_SERIALIZABLE_TYPE_TYPE_STRUCTURE, &userFields, 2};
Rt_SerializableField fileFields[2] = {{"girl", &userType}, {"boy", &userType}};

Ldc_User lpUsers[2] = {{"Céline", 12}, {"Laurent", 13}};

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_CHAR a;
  RT_N32 i;
  Rt_String str;
  int* lpArray;
  int* newItem;
  RT_UN32 unewItemIndex;
  RT_UN32 unArraySize;
  int deletedItem;


  Rt_PropertiesFileSerializer propertiesFileSerializer;

  PropertiesFileSerializer_Create(&propertiesFileSerializer, _R("test.ini"), fileFields, 2);

  PropertiesFileSerializer_Serialize(&propertiesFileSerializer, lpUsers);

  Array_Create((void**)&lpArray, sizeof(int), 10);
  for (i = 0; i < 6; i++)
    lpArray[i] = i;

  Array_SetSize((void**)&lpArray, 20);
  i = Array_GetSize(lpArray);

  Array_NewItem((void**)&lpArray, (void**)&newItem);

  newItemIndex = Array_NewItemIndex((void**)&lpArray, &newItemIndex);

  nArraySize = Array_GetSize(lpArray);
  for (i = 0; i < nArraySize; i++)
    lpArray[i] = i;

  Array_DeleteItem((void**)&lpArray, 5);
  deletedItem = lpArray[5];
  newItem = Array_GetLastItem(lpArray);
  Array_DeleteLastItem((void**)&lpArray);
  newItem = Array_GetLastItem(lpArray);

  for (i = 0; i < nArgC; i++)
    Shell_WriteLine(lpArgV[i]);

  Shell_Write(_R("Hééllo world !\n"));
  Shell_WriteCount(_R("Hello world !\n"), 14);

  String_Create(&str, _R("tutu"));

  Shell_Write(str);

  a = Shell_GetChar();
  if (a == _R('é'))
    Shell_Write(_R("A !\n"));
  Shell_Pause();

  return 0;
}

*/

#define BUFFER_SIZE 100
#define BIG_BUFFER_SIZE 200

void RT_CALL ZzDisplayEnvironmentVariable(RT_CHAR* lpName)
{
  RT_CHAR lpBigBuffer[BIG_BUFFER_SIZE];
  RT_N nWritten;

  nWritten = 0;
  RtCopyString(lpName, lpBigBuffer, BIG_BUFFER_SIZE, &nWritten);
  RtCopyString(_R("=\""), &lpBigBuffer[nWritten], BIG_BUFFER_SIZE - nWritten, &nWritten);
  RtGetEnvironmentVariable(lpName, &lpBigBuffer[nWritten], BIG_BUFFER_SIZE - nWritten, &nWritten);
  RtCopyString(_R("\"\n"), &lpBigBuffer[nWritten], BIG_BUFFER_SIZE - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpBigBuffer, nWritten);
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_CHAR lpBuffer[BUFFER_SIZE];
  RT_CHAR lpPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nReadCharactersCount;
  RT_CHAR nRead;
  RT_N nWritten;
  RT_N nI;

  nWritten = 0;
  RtCopyString(_R("Temp directory: \""), lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten);
  RtGetTempDirectory(&lpPath[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten);
  RtCopyString(_R("\".\n"), &lpPath[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpPath, nWritten);

  nWritten = 0;
  RtCopyString(_R("File name: \""), lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten);

#ifdef RT_DEFINE_WINDOWS
  RtExtractFileName(_R("c:\\foo\\bar.txt"), RtGetStringSize(_R("c:\\foo\\bar.txt")), &lpPath[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten);
#else
  RtExtractFileName(_R("/foo/bar.txt"), RtGetStringSize(_R("/foo/bar.txt")), &lpPath[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten);
#endif
  RtCopyString(_R("\"\n"), &lpPath[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpPath, nWritten);

  ZzDisplayEnvironmentVariable(_R("SystemRoot"));
  ZzDisplayEnvironmentVariable(_R("HOME"));

  nWritten = 0;
  RtGetApplicationDataDirectory(_R("Foo"), lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten);
  RtWriteStringsToConsole(3, _R("Config dir=\""), lpPath, _R("\"\n"));

  RtWriteStringToConsole(_R("Héllo, world!\n"));
  RtWriteStringsToConsole(1, _R("Hello, world!\n"));
  RtWriteStringsToConsole(4, _R("Hello"), _R(", "), _R("world!"), _R("\n"));
  RtWriteStringToConsole(_R("RtReadCharFromConsole\n"));
  nRead = RtReadCharFromConsole();
  RtWriteStringToConsoleWithSize(&nRead, 1);

  nWritten = 0;
  RtConvertNumberToString((RT_UCHAR8)nRead, lpBuffer, BUFFER_SIZE, &nWritten);
  RtWriteStringsToConsole(3, _R(" "), lpBuffer, _R("\n"));

  for (nI = 0; nI < 2; nI++)
  {
    RtWriteStringToConsole(_R("\nReading line...\n"));
    nReadCharactersCount = RtReadLineFromConsole(lpBuffer, BUFFER_SIZE);

    RtWriteStringsToConsole(3, _R("O"), lpBuffer, _R("O\n"));

    RtConvertNumberToString(nReadCharactersCount, lpBuffer, BUFFER_SIZE, &nWritten);
    RtWriteStringsToConsole(3, _R("Read = "), lpBuffer, _R("\n"));
  }

  RtWriteStringToConsole(_R("\nDone.\n"));
  RtReadCharFromConsole();

  return 0;
}
