# nmake makefile
# cocos2dx工程，生成动态链接库dll
# by zhanghao 2012-12-02

###################################### 第一部分:宏定义
# 生成文件
PROJECT_NAME = cocos2dx
OUT_DIR = ..\..\Debug.win32
OBJ_DIR = .\Debug.win32
TARGET = $(PROJECT_NAME).dll

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
CCFLAGS = /Od /Gm /EHsc /RTC1 /MDd /W4 /nologo /c /ZI /TP /Fd"$(OBJ_DIR)/vcx0.pdb"
CXXFLAGS = /Od /Gm /EHsc /RTC1 /MDd /W4 /nologo /c /ZI /TP /Fd"$(OBJ_DIR)/vcx0.pdb"

# 编译预定义宏
DEFINES = /D "WIN32" \
          /D "_DEBUG" \
          /D "_WINDOWS" \
          /D "_USRDLL" \
          /D "COCOS2DX_EXPORTS" \
          /D "_CRT_SECURE_NO_WARNINGS" \
          /D "_WINDLL" \
          /D "_UNICODE" \
          /D "UNICODE"

# 编译包含路径
INCLUDES = /I "../include" \
           /I "../third_party/OGLES" \
           /I "../third_party/vld" \
           /I "../third_party/libpng" \
           /I "../third_party/zlib" \
           /I "../third_party/GLFW" \
           /I "../platform" \
           /I "../platform/win32"

# 链接工具
LK = link

# 链接选项
LKFLAGS = /NOLOGO /MANIFEST:NO /DEBUG /DLL /PDB:"$(OUT_DIR)/$(PROJECT_NAME).pdb" /OUT:"$(OUT_DIR)/$(TARGET)"

# 链接文件包含路径
LIBPATH = /LIBPATH:"$(OUT_DIR)"
          
# 运行时库冲突，手动禁用指定运行库
NODEFAULTLIB = /NODEFAULTLIB:"libc.lib" \
          /NODEFAULTLIB:"libcmt.lib" \
          /NODEFAULTLIB:"msvcrt.lib" \
          /NODEFAULTLIB:"libcd.lib" \
          /NODEFAULTLIB:"libcmtd.lib"

# 链接库文件
LIBRARY = libzlib.lib \
          libpng.lib \
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
OBJECTS = ..\base_nodes\CCNode.obj \
		  ..\cocoa\CCAffineTransform.obj \
		  ..\cocoa\CCAutoreleasePool.obj \
		  ..\cocoa\CCGeometry.obj \
		  ..\cocoa\CCObject.obj \
		  ..\cocoa\CCZone.obj \
		  ..\label_nodes\CCLabelTTF.obj \
		  ..\layers_scenes_transitions_nodes\CCLayer.obj \
		  ..\layers_scenes_transitions_nodes\CCScene.obj \
		  ..\menu_nodes\CCMenu.obj \
		  ..\menu_nodes\CCMenuItem.obj \
		  ..\platform\CCFileUtils.obj \
		  ..\platform\CCImage.obj \
		  ..\platform\win32\CCApplication.obj \
		  ..\platform\win32\CCEGLView.obj \
		  ..\sprite_nodes\CCSprite.obj \
		  ..\support\CCPointExtension.obj \
		  ..\support\TransformUtils.obj \
		  ..\textures\CCTexture2D.obj \
		  ..\textures\CCTextureCache.obj \
		  ..\touch_dispatcher\CCTouchDispatcher.obj \
		  ..\touch_dispatcher\CCTouchHandler.obj \
		  ..\CCConfiguration.obj \
		  ..\CCDirector.obj \
		  ..\cocos2d.obj

###################################### 第二部分:命令块
all : $(OUT_DIR)\$(TARGET)

$(OUT_DIR)\$(TARGET):$(OBJECTS)
	@echo copying needed files...
	@if not exist "$(OUT_DIR)" mkdir "$(OUT_DIR)"
	@$(CP) "$(ProjectDir)..\third_party\libraries\*.*" "$(OUT_DIR)"
	@echo linking...
	@$(LK) $(LKFLAGS) $(LIBPATH) $(LIBRARY) $(NODEFAULTLIB) $(OBJECTS)

.cpp.obj:
	@echo compiling...
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) /Fo"$@" $< 

clean:
	@echo deleting objs...
	@$(DEL) $(OBJECTS) $(OBJ_DIR)\vcx0.idb $(OBJ_DIR)\vcx0.pdb
	@echo deleting targets...
	@$(DEL) $(OUT_DIR)\$(PROJECT_NAME).pdb $(OUT_DIR)\$(PROJECT_NAME).exp \
	$(OUT_DIR)\$(PROJECT_NAME).ilk $(OUT_DIR)\$(PROJECT_NAME).lib $(OUT_DIR)\$(PROJECT_NAME).dll
