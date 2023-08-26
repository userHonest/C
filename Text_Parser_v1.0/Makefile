INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)  
OBJDIR = obj
_DEPS =   threadData.h## add headers here 
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))
_OBJS = main.o threadData.o  ## add obj here, main.o is the same as main.c
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))
$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
## program name here 	
examTask04: $(OBJS) 
	$(CC) -o $@ $^ $(CFLAGS) -pthread  
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

