.PHONY: all analyze clean debug install release remove run

CC := gcc

CFLAGS := -std=c23 -pedantic -Wall -Wextra -Werror -Wformat=2 -Wformat-security -Wnull-dereference \
       -Wstack-protector -Warray-bounds=2 -Wimplicit-fallthrough=3 -Wtraditional-conversion \
       -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wstrict-overflow=2 \
       -Wstringop-overflow=4 -Winit-self -Wlogical-op -Wduplicated-cond -Wduplicated-branches \
       -Wfloat-equal -Wshadow=local -Wpointer-arith -Wcast-align=strict -Wstrict-aliasing=2 \
       -Wwrite-strings -Wdate-time -g3

SECURITY_FLAGS := -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fstack-clash-protection -fcf-protection

DEBUG_FLAGS := -O1 -DDEBUG -fsanitize=address -fsanitize=undefined

RELEASE_FLAGS := -O3 -DNDEBUG

SRCDIR := src
INCDIR := include
BUILDDIR := build
TESTDIR := test

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
TARGET := $(BUILDDIR)/dotman

CFLAGS += $(SECURITY_FLAGS)

all: debug

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

release: CFLAGS = -std=c23 -pedantic -Wall -Wextra -Werror $(SECURITY_FLAGS) $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

analyze:
	cppcheck --enable=all --std=c99 --platform=unix64 \
		--suppress=missingIncludeSystem \
		-I$(INCDIR) $(SRCDIR)/

memcheck: debug
	valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --verbose $(TARGET)

run: debug
	@$(TARGET)

install: release
	@sudo cp $(TARGET) /usr/local/bin/dm
	@sudo chmod +x /usr/local/bin/dm

remove:
	@sudo rm /usr/local/bin/dm

clean:
	rm -rf $(BUILDDIR)/*
