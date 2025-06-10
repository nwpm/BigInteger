# === Project Info ===
PROJECT_NAME = bigint

# === Paths ===
SRC_DIR = src

# === Compiler ===
CC     = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99

DEBUG_FLAGS   = -g -O0 -fsanitize=address
RELEASE_FLAGS = -O2

# === Build type ===
BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE), debug)
	CFLAGS    += $(DEBUG_FLAGS)
	BUILD_PATH = bin/debug
else ifeq ($(BUILD_TYPE), release)
	CLAGSS    += $(RELEASE_CFLAGS)
	BUILD_PATH = bin/release
else
	$(error Unknown build type: $(BUILD_TYPE))
endif

OBJ_DIR      = $(BUILD_PATH)/obj
LIB_TARGET   = $(BUILD_PATH)/lib$(PROJECT_NAME).a

OBJ_LIB = $(OBJ_DIR)/lib$(PROJECT_NAME).o

# ===Targets===
.PHONY: all debug release install uninstall clean

all: $(LIB_TARGET)

debug:
	$(MAKE) BUILD_TYPE=debug

release:
	$(MAKE) BUILD_TYPE=release

install: $(LIB_TARGET)
	install -d /usr/local/lib/ /usr/local/include/
	cp $(LIB_TARGET) /usr/local/lib/
	cp $(INCLUDE_DIR)/big_int.h /usr/local/include/

uninstall:
	rm -f /usr/local/lib/lib$(PROJECT_NAME).a
	rm -f /usr/local/include/big_int.h

clean :
	rm -rf $(BUILD_PATH)/*

# === Build Rules ===

$(LIB_TARGET) : $(OBJ_LIB) | $(BUILD_PATH)
	ar rcs $@ $<
	rm -r $(OBJ_DIR)

$(OBJ_LIB): $(SRC_DIR)/big_int.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(BUILD_PATH):
	mkdir -p $@






