#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include "functions.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void mouseClick(int a, int b)
{
Display *display = XOpenDisplay(NULL);

XEvent event;

if(display == NULL)
{
fprintf(stderr, "Error!!!\n");
exit(EXIT_FAILURE);
}

memset(&event, 0x00, sizeof(event));

event.type = ButtonPress;
event.xbutton.button = 1;
event.xbutton.x=a;
event.xbutton.y=b;
event.xbutton.same_screen = True;

XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

event.xbutton.subwindow = event.xbutton.window;

while(event.xbutton.subwindow)
{
event.xbutton.window = event.xbutton.subwindow;

XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
}

if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error!!!\n");

XFlush(display);

usleep(100000);

event.type = ButtonRelease;
event.xbutton.state = 0x100;

if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error !!!\n");

XFlush(display);

XCloseDisplay(display);
}
