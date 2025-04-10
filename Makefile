CC=gcc
INCLUDE_FLAGS=-I.
CFLAGS=-g -Werror
EXTRA_CFLAGS=

SRCS = main.c

OBJ = $(SRCS:.c=.o)

EXEC=socket_listener

.PHONY = clean all

all : $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) $^ $(LINK_FLAGS) -o $@

%.o : %.c
	$(COV_TOOL) $(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDE_FLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ) $(EXEC)
