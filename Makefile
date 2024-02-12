# Name of executable
BINS = finddups

# Setting the flags
CFLAGS = -g -Wall -Wstrict-prototypes

# Link in support for debugging
LDFLAGS = -g

# Default target produced by entering "make" alone
.PHONY: default
default: $(BINS)

# Compile *.c into *.o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Removes *.o files, but leaves executable
.PHONY: clean
clean:
	rm -f core* src/*.o *~

# Link *.o files into an executable
finddups: src/traverse.o src/helpers.o src/finddups.o
	$(CC) $(LDFLAGS) $^ -o $@

# Removes all files that can be reconstructed through "make"
.PHONY: immaculate
immaculate: clean
	rm -f $(BINS)
