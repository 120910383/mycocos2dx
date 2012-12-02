@echo off
color 0b
title build cocos2dx use nmake
rem written by zhanghao
rem 2012-12-02

%~d0

if "clean" == "%1" (
echo **********************cleaning tests**************************
cd %~dp0
cd .\tests\proj.win32
nmake clean

echo **********************cleaning cocos2dx**************************
cd %~dp0
cd .\cocos2dx\proj.win32
nmake clean
) else (
echo **********************building cocos2dx**************************
cd %~dp0
cd .\cocos2dx\proj.win32
nmake all

echo **********************building tests**************************
cd %~dp0
cd .\tests\proj.win32
nmake all

echo **********************run tests**************************
cd %~dp0
cd .\Debug.win32
tests.exe
)

pause