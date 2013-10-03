CPPC=g++
CPP_FLAGS=-std=c++11 -Wall -Werror

SRC_NAMES:= RecordType.cpp SRVRecord.cpp  TXTRecord.cpp
SRCS:=$(addprefix src/,$(SRC_NAMES))
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))
DEPS:=$(OBJS:.o=.d)
-include $(DEPS)

INC_DIRS:=..
INC= $(addprefix -I,$(INC_DIRS))

LIB_NAMES=resolv
LIB= $(addprefix -l,$(LIB_NAMES))

all: $(OBJS)

%.o: %.cpp
	$(CPPC) -c $(CPP_FLAGS) $(INC) $< -o $@
	$(CPPC) -MM $(CPP_FLAGS) $(INC) $< > $*.d

clean: cleandeps cleanobjs

cleandeps:
	rm -f $(DEPS)

cleanobjs:
	rm -f $(OBJS)
