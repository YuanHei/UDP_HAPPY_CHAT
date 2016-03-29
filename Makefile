ROOT_PATH=$(shell pwd)
CLIENT_BIN=chat_client
SERVER_BIN=chat_server
CLIENT=$(ROOT_PATH)/client
COMM=$(ROOT_PATH)/common
CONF=$(ROOT_PATH)/conf
LIB=$(ROOT_PATH)/lib
LOG=$(ROOT_PATH)/log
PLUGIN=$(ROOT_PATH)/plugin
SERVER=$(ROOT_PATH)/server
WINDOW=$(ROOT_PATH)/window
DATA_POLL=$(ROOT_PATH)/data_poll

INCLUDE=-I$(COMM) -I$(DATA_POLL) -I$(LIB)/include -I$(WINDOW) 
LIB_A_SO=-L$(LIB)/libs/linux-gcc-4.4.7 

CC=g++
FLAGS=$(INCLUDE)  #-D_DEBUG_
LDFLAGS=$(LIB_A_SO) -lpthread -ljson_linux-gcc-4.4.7_libmt -lncurses

SRC=$(shell ls -R | grep -E '.cpp')
OBJ=$(SRC:.cpp=.o)
SER_OBJ=$(shell echo $(OBJ) | sed 's/chat_client.o\|udp_client.o\|window.o//g')
CLI_OBJ=$(shell echo $(OBJ) | sed 's/chat_server.o\|udp_server.o\|data_pool.o//g')

.PHONY:all
all:$(CLIENT_BIN) $(SERVER_BIN)

.PHONY:debug
debug:
	echo $(SRC)
	echo $(OBJ)
	echo $(SER_OBJ)
	echo $(CLI_OBJ)

$(SERVER_BIN):$(SER_OBJ)
	echo 'building [$^] to [$@]'
	$(CC) -o $@ $^ $(LDFLAGS) 
	echo 'building ending'
$(CLIENT_BIN):$(CLI_OBJ)
	echo 'building [$^] to [$@]'
	$(CC) -o $@ $^ $(LDFLAGS)
	echo 'building ending'

%.o:$(CLIENT)/%.cpp
	echo 'compling [$<]'
	$(CC) -c $< $(FLAGS) 
	echo 'compling down'
%.o:$(SERVER)/%.cpp
	echo 'compling [$<]'
	$(CC) -c $< $(FLAGS) 
	echo 'compling down'
%.o:$(WINDOW)/%.cpp
	echo 'compling [$<]'
	$(CC) -c $< $(FLAGS) 
	echo 'compling down'
%.o:$(COMM)/%.cpp
	echo 'compling [$<]'
	$(CC) -c $< $(FLAGS) 
	echo 'compling down'
%.o:$(DATA_POLL)/%.cpp
	echo 'compling [$<]'
	$(CC) -c $< $(FLAGS) 
	echo 'compling down'

.PHONY:clean
clean:
	rm -rf *.o $(SERVER_BIN) $(CLIENT_BIN) output

.PHONY:output
output:
	mkdir -p output/server/log
	mkdir -p output/server/conf
	mkdir -p output/client
	cp $(SERVER_BIN) output/server
	cp $(CLIENT_BIN) output/client
	cp $(PLUGIN)/ctrl_server.sh output/server
	cp build.sh output/server
	cp $(CONF)/server.conf output/server/conf 
	cp ${LOG}/server.log output/server/log

