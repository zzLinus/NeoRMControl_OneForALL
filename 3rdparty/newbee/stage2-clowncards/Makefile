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
CPPFLAGS += -I$(WORK_DIR)/include

# NOTE: turn on debug here
CPPFLAGS += -D__DEBUG__

# FIXME: imtui dependency linking
CPPFLAGS += -I "./3rdparty/include"
CPPFLAGS += -L "./3rdparty/lib"

LDFLAGS += -lm -lpthread -ldl -lrt

#LDFLAGS = `pkg-config sdl --libs`

SRC = $(wildcard *.cc) $(wildcard **/*.cc)
OBJ = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SRC))))
BIN = MS-06

all: $(BIN) 
	$(call git_commit, "compile wormup!!")

run: all
	$(BUILD_DIR)/$(BIN)
	$(call git_commit, "run wormup!!")


$(BIN): $(OBJ)
	@echo -e + $(GREEN)LN$(END) $(BUILD_DIR)/$(BIN)
	@$(CC) -o $(BUILD_DIR)/$(BIN) $(OBJ) $(CPPFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@) 
	@echo -e + $(GREEN)CC$(END) $<
	@$(CC) -o $@ -c $< $(CPPFLAGS)

clean:
	@echo -e + $(BLUE)RM$(END) $(BIN)
	@rm $(BIN)

.PHONY: all run clean .git_commit .clean_index
