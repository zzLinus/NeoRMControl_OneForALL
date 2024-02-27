UNAME_S = $(shell uname -s)
WORK_DIR  = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build

CC = g++
CPPFLAGS = -std=c++14 -O0 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CPPFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
CPPFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
CPPFLAGS += -I$(WORK_DIR)/include
#CPPFLAGS += `pkg-config sdl --cflags`
#CPPFLAGS += -DDEBUG
# FIXME: imtui dependency linking
CPPFLAGS += -I "../imtui/build/dist/include"
CPPFLAGS += -I "../imtui/build/dist/include/imgui-for-imtui"
CPPFLAGS += -L "../imtui/build/dist/lib/"

LDFLAGS += -limtui -limtui-ncurses -limgui-for-imtui -lncurses

#LDFLAGS = `pkg-config sdl --libs`

SRC = $(wildcard src/*.cc) $(wildcard src/**/*.cc)
OBJ = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SRC))))
BIN = rx78-2

.PHONY: all clean

all: dirs $(BIN)

dirs:
	mkdir -p $(BUILD_DIR)

run: all
	$(BUILD_DIR)/$(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BUILD_DIR)/$(BIN) $^ $(CPPFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@) && echo + CC $<
	$(CC) -o $@ -c $< $(CPPFLAGS)

clean:
	rm -rf $(BUILD_DIR)/$(BIN) $(OBJ)
