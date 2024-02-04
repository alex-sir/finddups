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
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Removes *.o files, but leaves executable
.PHONY: clean
clean:
	rm -f core* *.o *~

# Link *.o files into an executable
finddups: traverse.o helpers.o finddups.o
	$(CC) $(LDFLAGS) $^ -o $@

# Removes all files that can be reconstructed through "make"
.PHONY: immaculate
immaculate: clean
	rm -f $(BINS)
