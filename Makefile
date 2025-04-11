CC=gcc
INCLUDE_FLAGS=-I.
CFLAGS=-g -Werror
EXTRA_CFLAGS=

SRCS = cov-listener.c

OBJ = $(SRCS:.c=.o)

EXEC=cov-listener

.PHONY = clean all

all : $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) $^ $(LINK_FLAGS) -o $@

%.o : %.c
	$(COV_TOOL) $(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDE_FLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ) $(EXEC)
