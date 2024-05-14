-include ../Makefile

UNAME_S = $(shell uname -s)
WORK_DIR  = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build
MAKE = make

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
END='\033[0m'

LOCK_DIR = ../../../.git/

THIRD_PARTY_LIB_DIR= $(shell find ./3rdparty -name "lib")

CC = g++
CPPFLAGS = -std=c++17 -O0 -g
#CPPFLAGS += -Wall
CPPFLAGS += -I$(WORK_DIR)/src

# NOTE: turn on debug here
CPPFLAGS += -D__DEBUG__

CPPFLAGS += -I "./inc"
CPPFLAGS += `pkg-config --cflags opencv4`

LDFLAGS = `pkg-config --libs opencv4`
LDFLAGS += -lm -lpthread -ldl -lrt

SRC = $(wildcard **/*.cpp) $(wildcard **/**/*.cpp)
OBJ = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SRC))))
BIN = MSN-07S

all: $(BIN) 
	$(call git_commit, "compile opencv!!")

run: all
	$(BUILD_DIR)/$(BIN)
	$(call git_commit, "run opencv!!")


$(BIN): $(OBJ)
	@echo -e + $(GREEN)LN$(END) $(BUILD_DIR)/$(BIN)
	@$(CC) -o $(BUILD_DIR)/$(BIN) $(OBJ) $(CPPFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@) 
	@echo -e + $(GREEN)CC$(END) $<
	@$(CC) -o $@ -c $< $(CPPFLAGS)

clean:
	@echo -e + $(BLUE)RM$(END) $(BIN)
	@rm $(BIN)

.PHONY: all run clean .git_commit .clean_index
