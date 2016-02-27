OS := $(shell uname)
ARCH := $(shell uname -m)
LEAP_SDK := $(HOME)/develop/LeapSDK

ifeq ($(ARCH), x86_64)
  LEAP_LIBRARY := $(LEAP_SDK)/lib/x64/libLeap.so -Wl,-rpath,../lib/x64
else
  LEAP_LIBRARY := $(LEAP_SDK)/lib/x86/libLeap.so -Wl,-rpath,../lib/x86
endif

CPPFLAGS += $(shell pkg-config opencv --cflags)
LOADLIBES += $(shell pkg-config opencv --libs)

ImageViewer: ImageViewer.cpp
	$(CXX) -Wall -g -I$(LEAP_SDK)/include ImageViewer.cpp -o ImageViewer $(LEAP_LIBRARY) $(LOADLIBES)

ImageCapture: ImageCapture.cpp
	$(CXX) -Wall -g -I$(LEAP_SDK)/include ImageCapture.cpp -o ImageCapture $(LEAP_LIBRARY)

clean:
	rm -rf ImageViewer ImageCapture 
