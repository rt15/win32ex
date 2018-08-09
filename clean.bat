@echo off

:: http://stevelorek.com/how-to-shrink-a-git-repository.html

rmdir /S /Q Win32Ex\doxygen\html

rmdir /S /Q Win32Ex\obj\Release
rmdir /S /Q Win32Ex\obj\Debug
del /S /Q Win32Ex\lib\*.a
del /S /Q Win32Ex\lib\*.lib
del /Q Win32Ex\project\*.layout
del /Q Win32Ex\project\*.depend
del /Q Win32Ex\project\*.cscope_file_list
del /Q Win32Ex\project\*.ncb
del /Q Win32Ex\project\*.user
del /Q /A:H Win32Ex\project\*.suo

rmdir /S /Q Win32ExGui\obj\Release
rmdir /S /Q Win32ExGui\obj\Debug
del /S /Q Win32ExGui\lib\*.a
del /S /Q Win32ExGui\lib\*.lib
del /Q Win32ExGui\project\*.layout
del /Q Win32ExGui\project\*.depend
del /Q Win32ExGui\project\*.cscope_file_list
del /Q Win32ExGui\project\*.ncb
del /Q Win32ExGui\project\*.user
del /Q /A:H Win32ExGui\project\*.suo

rmdir /S /Q Samples\obj\Release
rmdir /S /Q Samples\obj\Debug
del /Q Samples\project\*.layout
del /Q Samples\project\*.depend
del /Q Samples\project\*.cscope_file_list
del /Q Samples\project\*.ncb
del /Q Samples\project\*.user
del /Q /A:H Samples\project\*.suo

del /Q Samples\bin\ClimbingContest*
del /Q Samples\bin\Climbing*
del /Q Samples\bin\HelloWorld*
del /Q Samples\bin\LuaEx*
del /Q Samples\bin\libLuaEx*
del /Q Samples\bin\PicturesNamer*
del /Q Samples\bin\ReplaceInFile*
del /Q Samples\bin\Tests*
del /Q Samples\bin\TestsWithCrt*
del /Q Samples\bin\XdbmScriptsLister*

