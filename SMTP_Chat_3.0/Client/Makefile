INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR) -lm # mathlib flag added here 
OBJDIR = obj
_DEPS = client.h  ## add headers here 
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))
_OBJS = clientMain.o client.o  ## add obj here
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))
$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
## program name here 	
T04Client: $(OBJS) 
	$(CC) -o $@ $^ $(CFLAGS) -pthread   
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

