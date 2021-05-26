CCFLAGS=-g -O0 -pthread -pipe

GTK=`pkg-config --cflags --libs gtk+-3.0`

LDFLAGS=-pthread $(GTK) -export-dynamic -lm

OBJS= menu.o exe/floyd_ui exe/knapsack_ui exe/series_ui exe/replacement

all: $(OBJS)
	gcc -o menu menu.o $(LDFLAGS)
	rm -f menu.o 
    
menu.o: src/menu.c
	gcc -c $(CCFLAGS) src/menu.c $(GTK) -o menu.o

# Compile and load floyd_ui
exe/floyd_ui: floyd_ui.o
	gcc -o exe/floyd_ui floyd_ui.o $(LDFLAGS)
	rm -f floyd_ui.o 

floyd_ui.o: src/floyd.c
	gcc -c $(CCFLAGS) src/floyd.c $(GTK) -o floyd_ui.o  

# Compile and load knapsack_ui
exe/knapsack_ui: knapsack_ui.o
	gcc -o exe/knapsack_ui knapsack_ui.o $(LDFLAGS)
	rm -f knapsack_ui.o 

knapsack_ui.o: src/knapsack_ui.c
	gcc -c $(CCFLAGS) src/knapsack_ui.c $(GTK) -o knapsack_ui.o 

# Compile and series_ui 
exe/series_ui: series_ui.o
	gcc -o exe/series_ui series_ui.o $(LDFLAGS)
	rm -f series_ui.o 

series_ui.o: src/series_ui.c
	gcc -c $(CCFLAGS) src/series_ui.c $(GTK) -o series_ui.o  

# Compile and load replacement_ui 
exe/replacement: replacement.o
	gcc -o exe/replacement replacement.o $(LDFLAGS)
	rm -f replacement.o 

replacement.o: src/replacement.c
	gcc -c $(CCFLAGS) src/replacement.c $(GTK) -o replacement.o
    
clean:
	rm -f *.o *~ 