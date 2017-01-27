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
  RT_UN unWritten;

  unWritten = 0;
  RtCopyString(lpName, lpBigBuffer, BIG_BUFFER_SIZE, &unWritten);
  RtCopyString(_R("=\""), &lpBigBuffer[unWritten], BIG_BUFFER_SIZE - unWritten, &unWritten);
  RtGetEnvironmentVariable(lpName, &lpBigBuffer[unWritten], BIG_BUFFER_SIZE - unWritten, &unWritten);
  RtCopyString(_R("\"\n"), &lpBigBuffer[unWritten], BIG_BUFFER_SIZE - unWritten, &unWritten);
  RtWriteStringToConsoleWithSize(lpBigBuffer, unWritten);
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_CHAR lpBuffer[BUFFER_SIZE];
  RT_CHAR lpPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unReadCharactersCount;
  RT_CHAR nRead;
  RT_UN unWritten;
  RT_N nI;

  unWritten = 0;
  RtCopyString(_R("Temp directory: \""), lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten);
  RtGetTempDirectory(&lpPath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
  RtCopyString(_R("\".\n"), &lpPath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
  RtWriteStringToConsoleWithSize(lpPath, unWritten);

  unWritten = 0;
  RtCopyString(_R("File name: \""), lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten);

#ifdef RT_DEFINE_WINDOWS
  RtExtractFileName(_R("c:\\foo\\bar.txt"), RtGetStringSize(_R("c:\\foo\\bar.txt")), &lpPath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
#else
  RtExtractFileName(_R("/foo/bar.txt"), RtGetStringSize(_R("/foo/bar.txt")), &lpPath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
#endif
  RtCopyString(_R("\"\n"), &lpPath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
  RtWriteStringToConsoleWithSize(lpPath, unWritten);

  ZzDisplayEnvironmentVariable(_R("SystemRoot"));
  ZzDisplayEnvironmentVariable(_R("HOME"));

  unWritten = 0;
  RtGetApplicationDataDirectory(_R("Foo"), lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten);
  RtWriteStringsToConsole(3, _R("Config dir=\""), lpPath, _R("\"\n"));

  RtWriteStringToConsole(_R("Héllo, world!\n"));
  RtWriteStringsToConsole(1, _R("Hello, world!\n"));
  RtWriteStringsToConsole(4, _R("Hello"), _R(", "), _R("world!"), _R("\n"));
  RtWriteStringToConsole(_R("RtReadCharFromConsole\n"));
  nRead = RtReadCharFromConsole();
  RtWriteStringToConsoleWithSize(&nRead, 1);

  unWritten = 0;
  RtConvertIntegerToString((RT_UCHAR8)nRead, lpBuffer, BUFFER_SIZE, &unWritten);
  RtWriteStringsToConsole(3, _R(" "), lpBuffer, _R("\n"));

  for (nI = 0; nI < 2; nI++)
  {
    RtWriteStringToConsole(_R("\nReading line...\n"));
    unReadCharactersCount = RtReadLineFromConsole(lpBuffer, BUFFER_SIZE);

    RtWriteStringsToConsole(3, _R("O"), lpBuffer, _R("O\n"));

    RtConvertUIntegerToString(unReadCharactersCount, lpBuffer, BUFFER_SIZE, &unWritten);
    RtWriteStringsToConsole(3, _R("Read = "), lpBuffer, _R("\n"));
  }

  RtWriteStringToConsole(_R("\nDone.\n"));
  RtReadCharFromConsole();

  return 0;
}
