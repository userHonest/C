INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR) -lm # mathlib flag added here 
OBJDIR = obj
_DEPS = clientHandle.h  ## add headers here 
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))
_OBJS = main.o clientHandle.o  ## add obj here
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))
$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
## program name here 	
Task05_Client: $(OBJS) 
	$(CC) -o $@ $^ $(CFLAGS) -pthread   
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

