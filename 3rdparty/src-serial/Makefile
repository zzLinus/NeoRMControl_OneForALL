UNAME_S = $(shell uname -s)
WORK_DIR  = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
END='\033[0m'

CC = g++
CPPFLAGS = -std=c++17 -O0 -g 
CPPFLAGS += -I$(WORK_DIR)/include

CPPFLAGS += -I "./3rdparty/include"
CPPFLAGS += -L "./3rdparty/lib"

LDFLAGS += -lm -lpthread -ldl -lrt -lserial

#LDFLAGS = `pkg-config sdl --libs`

SRC = $(wildcard src/*.cc) $(wildcard src/**/*.cc) $(wildcard src/**/**/*.cc)
OBJ = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SRC))))
BIN = libserial.a

.PHONY: all clean

all: dirs $(BIN)

dirs:
	@echo -e + $(BLUE)MKDIR$(END) $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)

run: all
	$(BUILD_DIR)/$(BIN)

$(BIN): $(OBJ)
	@echo -e + $(GREEN)AR$(END) $(BUILD_DIR)/$(BIN)
	@ar rcs $(BUILD_DIR)/$(BIN) $^

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	@echo -e + $(GREEN)CC$(END) $<
	@$(CC) -o $@ -c $< $(CPPFLAGS)

clean:
	rm -rf $(BUILD_DIR)
