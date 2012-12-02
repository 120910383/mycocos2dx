# nmake makefile
# tests工程，生成可执行程序exe
# by zhanghao 2012-12-02

###################################### 第一部分:宏定义
# 生成文件
PROJECT_NAME = tests
OUT_DIR = ..\..\Debug.win32
OBJ_DIR = .\Debug.win32
TARGET = $(PROJECT_NAME).exe

# shell命令
CP = xcopy /y /q
DEL = del /f /q

# 编译工具
CC = cl
CXX = cl

# 编译选项
# Od----禁用优化
# Gm----启用最小重新生成
# EHsc----启用C++异常
# RTC1----启用基本运行时错误检查:如未初始化变量使用等
# MDd----指定运行时库位多线程调试DLL
# W4----设置警告等级为4级
# nologo----不显示cl版权信息
# c----只编译不链接
# ZI----生成“编辑并继续”的程序数据库
# TP----指定编译为C++代码
# Fo----生成obj位置
# Fd----生成pdb位置
CCFLAGS = /Od /Gm /EHsc /RTC1 /MDd /W3 /nologo /c /ZI /TP /Fd"$(OBJ_DIR)/vcx0.pdb"
CXXFLAGS = /Od /Gm /EHsc /RTC1 /MDd /W3 /nologo /c /ZI /TP /Fd"$(OBJ_DIR)/vcx0.pdb"

# 编译预定义宏
DEFINES = /D "WIN32" \
          /D "_DEBUG" \
          /D "_WINDOWS" \
          /D "_UNICODE" \
          /D "UNICODE"

# 编译包含路径
INCLUDES = /I "../../cocos2dx/include" \
           /I "..\..\cocos2dx\third_party\OGLES" \
           /I "..\..\cocos2dx\third_party\GLFW" \
           /I "..\..\cocos2dx\platform" \
           /I "..\..\cocos2dx\platform\win32" \
           /I "..\tests"

# 链接工具
LK = link

# 链接选项
LKFLAGS = /NOLOGO /MANIFEST /MANIFESTFILE:"$(OUT_DIR)\$(TARGET).manifest" \
          /SUBSYSTEM:WINDOWS /MACHINE:X86 /DEBUG /PDB:"$(OUT_DIR)/$(PROJECT_NAME).pdb" /OUT:"$(OUT_DIR)/$(TARGET)"

# 链接文件包含路径
LIBPATH = /LIBPATH:"$(OUT_DIR)"

# 链接库文件
LIBRARY = cocos2dx.lib \
          libgles_cm.lib \
          kernel32.lib \
          user32.lib \
          gdi32.lib \
          winspool.lib \
          comdlg32.lib \
          advapi32.lib \
          shell32.lib \
          ole32.lib \
          oleaut32.lib \
          uuid.lib \
          odbc32.lib \
          odbccp32.lib

# 编译生成对象文件
OBJECTS = .\main.obj \
          ..\tests\AppDelegate.obj \
          ..\tests\TestBasicScene.obj \
          ..\tests\TestControllerScene.obj \
          ..\tests\LabelTest\LabelTest.obj \
          ..\tests\OpenGLTest\OpenGLTest.obj \
          ..\tests\SpriteTest\SpriteTest.obj \

###################################### 第二部分:命令块
all : $(OUT_DIR)\$(TARGET)

$(OUT_DIR)\$(TARGET):$(OBJECTS)
	@echo linking...
	@$(LK) $(LKFLAGS) $(LIBPATH) $(LIBRARY) $(OBJECTS)

.cpp.obj:
	@echo compiling...
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) /Fo"$@" $< 

clean:
	@echo deleting objs...
	@$(DEL) $(OBJECTS) $(OBJ_DIR)\vcx0.idb $(OBJ_DIR)\vcx0.pdb 2>nul
	@echo deleting targets...
	@$(DEL) $(OUT_DIR)\$(TARGET) $(OUT_DIR)\$(TARGET).manifest \
		$(OUT_DIR)\$(PROJECT_NAME).pdb $(OUT_DIR)\$(PROJECT_NAME).ilk 2>nul
	@echo deleting completed.
