#include<X11/X.h>
#include<X11/Xlib.h>
#include<stdio.h>
#include<stdlib.h>

#include"functions.h"

void mouse(int x, int y)
{
	Display *display;
	Window window,root;
	int screen;
	display=XOpenDisplay(NULL);
	screen=DefaultScreen(display);
	root=RootWindow(display, screen);
	
	XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
	//printf("working");
	XFlush(display);
	//fflush(stdout);
	XCloseDisplay(display);
	printf("*******************display %p****************************\n",display);
	//free(display);
	return;
}
