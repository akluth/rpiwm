SRCS = $(shell find -name '*.c')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = gcc
LD = gcc
XROOT    = /usr/X11R6
XINCLPATH = -I$(XROOT)/include
XLDPATH   = -L$(XROOT)/lib
CFLAGS = -Wall -I include -lX11

rpiwm: $(OBJS)
	$(LD) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(XINCLPATH) -c -o $@ $^

clean:
	rm $(OBJS)

.PHONY: clean
