CFLAGS=-g
LDFLAGS=-lbsd

.PHONY: clean

kdumpd: kdumpd.o kdumpsubs.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean:
	rm *.o
	rm kdumpd
