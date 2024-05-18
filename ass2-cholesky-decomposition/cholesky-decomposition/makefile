CC         := gcc-13
LINKER     := $(CC)
CFLAGS     := -O2 -Wall -march=armv8.5-a
LDFLAGS    := -lm

UTIL       := matrix.o \
              dclock.o \
              base_chol.o

TEST_OBJS  := test_chol.o $(FILE).o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all:
	make test_chol.x

test_chol.x: $(TEST_OBJS) $(UTIL) parameters.h
	$(LINKER) $(TEST_OBJS) $(UTIL) $(LDFLAGS) \
		$(BLAS_LIB) -o $@

run:
	rm -f $(FILE).o
	make all
	export OMP_NUM_THREADS=1
	export GOTO_NUM_THREADS=1
	echo "version = '$(FILE)';" > output_$(FILE).txt
	./test_chol.x >> output_$(FILE).txt

clean:
	rm -f *.o *~ core *.x

cleanall:
	rm -f *.o *~ core *.x output*.m *.eps *.png
