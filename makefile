TARGET=proyecto

CCFLAGS=-g -O0 -Wall -pthread -pipe

GTK=`pkg-config --cflags --libs gtk+-3.0`

LDFLAGS=$(PTHREAD) $(GTK) -export-dynamic

OBJS= main.o

all: $(OBJS)
	gcc -o $(TARGET) $(OBJS) $(LDFLAGS)
    
main.o: src/main.c
	gcc -c $(CCFLAGS) src/main.c $(GTK) -o main.o
    
clean:
	rm -f *.o $(TARGET)