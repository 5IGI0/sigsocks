EXEC_EXT ?= bin
TARGET_EXEC ?= $(shell $(CC) -dumpmachine)
BUILDS_DIR = ./build
BUILD_DIR = $(BUILDS_DIR)/build-$(TARGET_EXEC)
SRC_DIRS = ./src

SRCS = $(shell find $(SRC_DIRS) -name *.c -or -name *.s)
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

CFLAGS ?= -Wall -Wextra $(ADDITIONAL_CFLAGS)

$(BUILDS_DIR)/$(TARGET_EXEC).$(EXEC_EXT): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(CFLAGS)
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILDS_DIR)/build-*/ $(BUILDS_DIR)/*.$(EXEC_EXT) $(BUILDS_DIR)/*.exe

MKDIR_P ?= mkdir -p