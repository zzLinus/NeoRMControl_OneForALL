UNAME_S = $(shell uname -s)
WORK_DIR  = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build

CC = g++
CPPFLAGS = -std=c++20 -O0 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CPPFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
CPPFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
CPPFLAGS += -I$(WORK_DIR)/include
#CPPFLAGS += `pkg-config sdl --cflags`
#CPPFLAGS += -DDEBUG
# FIXME: imtui dependency linking
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
	mkdir -p $(BUILD_DIR)

run: all
	$(BUILD_DIR)/$(BIN)

$(BIN): $(OBJ)
	ar rcs $(BUILD_DIR)/$(BIN) $^

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@) && echo + CC $<
	$(CC) -o $@ -c $< $(CPPFLAGS)

clean:
	rm -rf $(BUILD_DIR)
