# make NAME=main
TARGET=$(NAME)

CCFLAGS=-g -O0 -pthread -pipe

GTK=`pkg-config --cflags --libs gtk+-3.0`

LDFLAGS=-pthread $(GTK) -export-dynamic -lm

OBJS= $(NAME).o 

all: $(OBJS)
	gcc -o $(TARGET) $(OBJS) $(LDFLAGS)
    
$(NAME).o: src/$(NAME).c
	gcc -c $(CCFLAGS) src/$(NAME).c $(GTK) -o $(NAME).o
    
clean:
	rm -f *.o $(TARGET)