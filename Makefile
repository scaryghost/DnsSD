CPPC=g++
CPP_FLAGS=-std=c++11 -Wall -Werror

SRCS:= RecordType.cpp ServiceLocator.cpp SRVRecord.cpp  TXTRecord.cpp
SRCS:=$(addprefix src/,$(SRCS))
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))

INC_DIRS:=..
INC= $(addprefix -I,$(INC_DIRS))

LIB_NAME=libdnssd.a

all: $(LIB_NAME)

$(LIB_NAME): $(OBJS)
	$(AR) -cvru $@ $?

%.o: %.cpp
	$(CPPC) -c $(CPP_FLAGS) $(INC) $< -o $@

clean: cleandeps cleanobjs

cleanobjs:
	rm -f $(OBJS)
