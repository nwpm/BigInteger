# === Project Info ===
PROJECT_NAME = bigint

# === Paths ===
SRC_DIR   = src
BIN_DIR   = bin
TEST_DIR  = test

BIGINT_SRC  = $(SRC_DIR)/bigint.c 

UNITY_DIR   = $(TEST_DIR)/Unity
UNITY_BUILD = build/Unity

TEST_SRC    = $(TEST_DIR)/test_bigint.c

# === Compiler ===
CC     = gcc
CFLAGS = -Wall -Wextra -pedantic -std=gnu99

DEBUG_FLAGS   = -g -O0 -fsanitize=address
RELEASE_FLAGS = -O2

# === Build type ===
BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE), debug)
	CFLAGS    += $(DEBUG_FLAGS)
	BUILD_DIR = build/$(PROJECT_NAME)/debug
else ifeq ($(BUILD_TYPE), release)
	CLAGSS    += $(RELEASE_CFLAGS)
	BUILD_DIR = build/$(PROJECT_NAME)/release
else
	$(error Unknown build type: $(BUILD_TYPE))
endif

LIB_TARGET = $(BUILD_DIR)/lib$(PROJECT_NAME).a
OBJ_LIB    = $(BUILD_DIR)/lib$(PROJECT_NAME).o

TEST_TARGET = $(BIN_DIR)/test_bigint
TEST_OBJ    = $(TEST_DIR)/test_bigint.o

# ===Targets===
.PHONY: all debug release tests unity install uninstall cllib clunity clall

all:
	$(MAKE) unity
	$(MAKE) BUILD_TYPE=debug

bigint: $(LIB_TARGET)

unity: $(UNITY_BUILD)
	cd $(UNITY_BUILD) && cmake ../../$(UNITY_DIR)
	cd $(UNITY_BUILD) && make
	cp test/Unity/src/unity.h $(SRC_DIR)
	cp test/Unity/src/unity_internals.h $(SRC_DIR) 
	find build/Unity -mindepth 1 ! -name 'libunity.a' -exec rm -rf {} +

tests: $(TEST_TARGET)

debug:
	$(MAKE) BUILD_TYPE=debug bigint

release:
	$(MAKE) BUILD_TYPE=release bigint

install: $(LIB_TARGET)
	install -d /usr/local/lib/ /usr/local/include/
	cp $(LIB_TARGET) /usr/local/lib/
	cp src/bigint.h /usr/local/include/

uninstall:
	rm -f /usr/local/lib/lib$(PROJECT_NAME).a
	rm -f /usr/local/include/bigint.h

cllib:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*

clunity:
	rm -rf $(UNITY_BUILD)/*
	rm $(SRC_DIR)/unity*.h

clall:
	$(MAKE) cllib
	$(MAKE) clunity

# === Build Rules ===

$(TEST_TARGET): $(TEST_OBJ)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -L$(BUILD_DIR) -L$(UNITY_BUILD) $< -o $@ -lbigint -lunity
	rm $(TEST_OBJ)

$(TEST_OBJ) : $(TEST_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_TARGET) : $(OBJ_LIB) | $(BUILD_DIR)
	ar rcs $@ $<
	rm -r $(OBJ_LIB)

$(OBJ_LIB): $(BIGINT_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(UNITY_BUILD):
	mkdir -p $@




