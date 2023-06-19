
TARGET = Client
INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -pthread
CFLAGS += -I$(INCLDIR)

OBJDIR = ./obj

## list all header files here ##	
_DEPS = client.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(strip $(_DEPS)))

## list all source files here, but given their .o file convention ##
_SRCS =  clientMain.c clientFunc.c
OBJS =  $(OBJDIR)/* 
#OBJS += $(patsubst %,$(OBJDIR)/%,$(notdir $(strip $(SRCS:.c=.o))))
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(strip $(_SRCS)))
$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

## change output filename here ##
$(TARGET): $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -pthread

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
