#For cflags
CFLAGS = `pkg-config opencv cvblob --cflags --libs`

all: test

mouse.o: mouse.cpp
	gcc -c mouse.cpp -L/usr/include/X11/ -lX11
	
mouseclick.o: mouseclick.cpp
	gcc -c mouseclick.cpp -L/usr/include/X11/ -lX11

red.o: red.cpp
	g++ -ggdb $(CFLAGS) -c red.cpp 

test: red.o mouse.o mouseclick.o
	g++ $(CFLAGS) red.o mouse.o mouseclick.o -o test -L/usr/include/X11/ -lX11

clean:
	rm -rf *o test
