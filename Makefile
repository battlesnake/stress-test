progs := nr_sin nr_cos nr_sigmoid nr_tan nr_fail dotp alloc

O ?= 2

CFLAGS = -O$(O) -g -march=native -mtune=native -ftree-vectorize -funroll-loops -lm

# CFLAGS += -mfloat-abi=hard

.PHONY: all
all: $(addprefix prog-, $(progs))


.PHONY: clean
clean:
	rm -f -- prog-*

prog-%: %.c
	gcc $(CFLAGS) -o $@ $<
