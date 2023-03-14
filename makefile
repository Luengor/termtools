.SECONDEXPANSION:

# Vars
testd := tests
srcd := src

lib_objects = $(srcd)/screen.o $(srcd)/term.o $(srcd)/image.o
test_sources := $(wildcard $(testd)/*.c)
test_bins := $(patsubst $(testd)/%.c, %,$(test_sources))

CC := gcc
INC_DIRS := ${srcd} /usr/include/webp
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -lm -lwebp

# Lib
.PHONY: lib
lib: ${lib_objects}
${lib_objects}: %.o: %.c

# Tests
.PHONY: tests
tests: $(test_bins)

# This re-makes ALL tests every time 1 changes
# Bad, but the best I can do for now
$(test_bins): $(testd)/$$@.c $(lib_objects)
	$(CC) -o $@ $(CPPFLAGS) $(testd)/$@.c $(lib_objects) 

# Clean
.PHONY: clean
clean:
	rm ${srcd}/*.o
	rm $(test_bins)

