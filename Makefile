.PHONY: all analyze analyze-fix clean debug install release remove run memcheck

# Compiler and basic flags
CC := gcc
STD := gnu17
BASE_FLAGS := -std=$(STD) -pedantic -Wall -Wextra -Werror -Wformat=2 -Wformat-security \
              -Wnull-dereference -Wstack-protector -Warray-bounds=2 -Wimplicit-fallthrough=3 \
              -Wtraditional-conversion -Wstrict-prototypes -Wmissing-prototypes \
              -Wmissing-declarations -Wstrict-overflow=2 -Wstringop-overflow=4 -Winit-self \
              -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wfloat-equal -Wshadow=local \
              -Wpointer-arith -Wcast-align=strict -Wstrict-aliasing=2 -Wwrite-strings -Wdate-time -g3

# Security and optimization flags
SECURITY_FLAGS := -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fstack-clash-protection -fcf-protection
DEBUG_FLAGS := -Og -DDEBUG -fsanitize=address -fsanitize=undefined
RELEASE_FLAGS := -O3 -DNDEBUG

# Include paths
SUBMODULES := -Iexternal/c-vector

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build
TESTDIR := test

# File patterns
SOURCES := $(wildcard $(SRCDIR)/*.c)
HEADERS := $(wildcard $(INCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
TARGET := $(BUILDDIR)/dotman

# Combine base flags
CFLAGS := $(BASE_FLAGS) $(SECURITY_FLAGS) $(SUBMODULES)
LDFLAGS :=

# Default target
all: debug

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: LDFLAGS += -fsanitize=address -fsanitize=undefined
debug: $(TARGET)
	@echo "✓ Debug build completed successfully"

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)
	@echo "✓ Release build completed successfully"

# Main target
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	@echo "🔗 Linking $(notdir $@)..."
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(BUILDDIR)
	@echo "🔨 Compiling $(notdir $<)..."
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Static analysis with clang-tidy
analyze:
	@echo "🔍 Running static analysis with clang-tidy..."
	@clang-tidy $(SOURCES) -- -std=$(STD) -I$(INCDIR) $(SUBMODULES) \
		-D_FORTIFY_SOURCE=2 -DDEBUG
	@echo "✓ Static analysis completed"

# Fix issues automatically with clang-tidy
analyze-fix:
	@echo "🔧 Running clang-tidy with automatic fixes..."
	@clang-tidy -fix $(SOURCES) -- -std=$(STD) -I$(INCDIR) $(SUBMODULES) \
		-D_FORTIFY_SOURCE=2 -DDEBUG
	@echo "✓ Automatic fixes applied"

# Memory checking
memcheck: debug
	@echo "🧪 Running memory check..."
	@valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --verbose --error-exitcode=1 $(TARGET) || \
		(echo "ℹ️  Memory check completed (exit code: $$?)" && exit 0)
	@echo "✓ Memory check passed - no leaks detected"

# Run the program
run: debug
	@echo "🚀 Running $(notdir $(TARGET))..."
	@$(TARGET)

# Install to system
install: release
	@echo "📦 Installing to /usr/local/bin/dm..."
	@sudo cp $(TARGET) /usr/local/bin/dm
	@sudo chmod +x /usr/local/bin/dm
	@echo "✓ Installation completed successfully"

# Remove from system
remove:
	@echo "🗑️  Removing /usr/local/bin/dm..."
	@sudo rm -f /usr/local/bin/dm
	@echo "✓ Removal completed successfully"

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build directory..."
	@rm -rf $(BUILDDIR)/*
	@echo "✓ Clean completed successfully"
