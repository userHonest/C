# Main makefile

# create a subfolder named include
TARGET = Portscanner
INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

# create a subfolder named obj
OBJDIR = ./obj

## list all header files here ##
_DEPS = portscanner.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

## list all source files here, but given their .o file convention ##
_OBJS = main.c portscanner.c
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

## change output filename here ##
$(TARGET): $(OBJS)
		gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
		rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

