all: ceres_test

CPPFLAGS += -I/usr/include/eigen3
CPPFLAGS += $(shell pkg-config opencv --cflags)
LOADLIBES += $(shell pkg-config opencv --libs)
LOADLIBES += -L/usr/local/lib -lceres
LOADLIBES += $(shell pkg-config libgflags --libs)
LOADLIBES += $(shell pkg-config libglog --libs)
LOADLIBES += -lblas -llapack -lcamd -lamd -lccolamd -lcolamd -lcholmod
LOADLIBES += -lcxsparse
LOADLIBES += -lpthread
LOADLIBES += -lgomp
LOADLIBES += -lm
CFLAGS += -Wall
CXXFLAGS += -Wall
CFLAGS += -O2
CXXFLAGS += -O2
