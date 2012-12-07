# make makefile
# cocos2dx工程，生成动态链接库so
# by zhanghao 2012-12-03

###################################### 第一部分:宏定义
# 生成文件
PROJECT_NAME = cocos2dx
OUT_DIR = ../../Debug.linux
OBJ_DIR = ./Debug.linux
TARGET = $(PROJECT_NAME).so

# shell命令
CP = cp
DEL = rm -f

# 编译工具
CC = gcc
CXX = g++

# 编译选项
# Wall----显示所有编译警告
# Wno-unused-parameter----关闭从未用过的参数警告
# g----生成调试信息
# O2----二级优化，进一步编译优化
# fPIC----生成动态链接库函数符号可重定向，默认shared是包含fPIC选项，安全起见，显式调用
CCFLAGS = -Wall -g -O2 -fPIC
CXXFLAGS = -Wall -g -O2 -fPIC

# 编译预定义宏
DEFINES = -DLINUX

# 编译包含路径
INCLUDES = -I../include \
           -I../platform

# 链接工具
LK = g++

# 链接选项
LKFLAGS = -shared -o

# 链接文件包含路径
STATICLIBS_DIR = 


# 链接库文件
STATICLIBS = 
SHAREDLIBS = 

# 编译生成的对象文件
OBJECTS = ../base_nodes/CCNode.o \
          ../cocoa/CCAffineTransform.o \
          ../cocoa/CCAutoreleasePool.o \
          ../cocoa/CCGeometry.o \
          ../cocoa/CCObject.o \
          ../cocoa/CCZone.o \
          ../label_nodes/CCLabelTTF.o \
          ../layers_scenes_transitions_nodes/CCLayer.o \
          ../layers_scenes_transitions_nodes/CCScene.o \
          ../menu_nodes/CCMenu.o \
          ../menu_nodes/CCMenuItem.o \
          ../sprite_nodes/CCSprite.o \
          ../support/CCPointExtension.o \
          ../support/TransformUtils.o \
          ../textures/CCTexture2D.o \
          ../textures/CCTextureCache.o \
          ../touch_dispatcher/CCTouchDispatcher.o \
          ../touch_dispatcher/CCTouchHandler.o \
          ../CCConfiguration.o \
          ../CCDirector.o \
          ../cocos2d.o

###################################### 第二部分:命令块
all: $(OUT_DIR)/$(TARGET)

$(OUT_DIR)/$(TARGET): $(OBJECTS)
	$(LK) $(OBJECTS) $(LKFLAGS) $(OUT_DIR)/$(TARGET) $(SHAREDLIBS) $(STATICLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

clean:
	$(DEL) $(OBJECTS) $(OUT_DIR)/$(TARGET)

