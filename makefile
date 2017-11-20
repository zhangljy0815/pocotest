src = $(wildcard *.cpp)
targets = $(patsubst %.cpp, %, $(src))

CC = g++
INC_PATH = -I$(POCO_HOME)include
LIB_PATH = -L$(POCO_HOME)lib
CPPFLAGS = -lPocoFoundation -lPocoUtil -lPocoXML -lPocoJSON 


all:$(targets)

$(targets):%:%.cpp
	$(CC) $< -o $@ $(INC_PATH) $(LIB_PATH) $(CPPFLAGS)

.PHONY:clean all
clean:
	-rm -rf $(targets) 
