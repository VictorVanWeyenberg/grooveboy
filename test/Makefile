CC := gcc
CFLAGS := -I ../include

DEVKITPRO  := /opt/devkitpro
BIN2C      := $(DEVKITPRO)/tools/bin/bin2c

TEST_MAIN := tracker_tests.c
TEST_SRCS := $(filter-out $(TEST_MAIN),$(wildcard *.c))
TEST_OBJS := $(TEST_SRCS:.c=.o)

SRC_DIR    := ../src
BIN_DIR    := ../bin
BUILD_DIR  := ../build

TARGET     := grooveboy
MAIN_FILE  := $(SRC_DIR)/$(TARGET).c
SRC_FILES  := $(filter-out $(MAIN_FILE), $(wildcard $(SRC_DIR)/*.c))
BIN_FILES  := $(wildcard $(BIN_DIR)/*.bin)

SRC_OFILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
BIN_CFILES := $(patsubst $(BIN_DIR)/%.bin,$(BUILD_DIR)/%.c,$(BIN_FILES))
BIN_OFILES := $(BIN_CFILES:.c=.o)

OBJ_FILES  := $(BIN_OFILES) $(SRC_OFILES) $(TEST_OBJS)

test: grooveboy_tests
	./grooveboy_tests

clean:
	rm -rf grooveboy_tests $(OBJ_FILES)

grooveboy_tests: $(SRC_OFILES) $(BIN_OFILES) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_MAIN) $(BIN_OFILES) $(SRC_OFILES) $(TEST_OBJS) -o $@

$(BIN_OFILES): $(BUILD_DIR)/%.o: $(BUILD_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.c: $(BIN_DIR)/%.bin 
	$(BIN2C) $< $@ data
	sed -i "s/data/$(notdir $(basename $@))/g" $@

$(SRC_OFILES): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
