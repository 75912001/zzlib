#-lpthread 是多线程库支持

EXE=libel.so

#请在用户目录下创建endless文件夹,用来存放so

TAG_DIR=$(SERVER_HOME)/endless

CC=g++ -std=c++0x

#FLAG=-Wall -DENABLE_TRACE_LOG -DSHOW_ALL_LOG -DSHOW_ALL_LOG_HEX -fPIC -shared -O0 -g -o
FLAG=-Wall -Wconversion -Wno-sign-conversion -fPIC -shared -O0 -g -o
LOCAL_INCLUDE=../include

INCLUDES=-I$(LOCAL_INCLUDE) -I/usr/include/libxml2

LINK=-lxml2 -lpthread -lz

LOCAL_SRCS=$(wildcard ./*.cpp)
LOCAL_SRCS_NET=$(wildcard ./lib_net/*.cpp)
LOCAL_SRCS_PROTO=$(wildcard ./lib_proto/*.cpp)
LOCAL_SRCS_WEBSOCKET=$(wildcard ./lib_websocket/*.cpp)

$(EXE):
	$(CC) $(LOCAL_SRCS) $(LOCAL_SRCS_NET) $(LOCAL_SRCS_PROTO) $(LOCAL_SRCS_WEBSOCKET) $(INCLUDES) $(LINK) $(FLAG) $@
	mkdir -p $(TAG_DIR)
	cp -f $@ $(TAG_DIR)
	mkdir -p $(EL_LIB_INCLUDE)
	cp -rf $(LOCAL_INCLUDE) $(EL_LIB_INCLUDE)
	rm $@
install:
# 将头文件放入指定的目录中
# 将SO放入指定的目录中
clean:
	rm $(EXE) *.o -f
#ln -s $(LOCAL_INCLUDE) $(TAG_DIR)lib_include
