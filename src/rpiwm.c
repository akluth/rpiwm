#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "rpiwm.h"


int main(int argc, char *argv[])
{
    rpiwm = (rpiwm_t*)malloc(sizeof(rpiwm_t));
    rpiwm->atoms = (rpiwm_atoms_t*)malloc(sizeof(rpiwm_atoms_t));

    rpiwm->display = XOpenDisplay(rpiwm->display_options);

    if (!rpiwm->display) {
        fprintf(stderr, "rpiwm: cannot open display %s\n",
        rpiwm->display_options);
        exit(1);
    }

    rpiwm->screen = DefaultScreen(rpiwm->display);
    rpiwm->root_window = RootWindow(rpiwm->display, rpiwm->screen);

    rpiwm->atoms->xa_wm_protos = XInternAtom(rpiwm->display, "WM_PROTOCOLS", False);
    rpiwm->atoms->xa_wm_delete = XInternAtom(rpiwm->display, "WM_DELETE_WINDOW", False);

    rpiwm_event_loop();

    exit(0);
}


void key_handler(XKeyEvent *event)
{
    int placeholder;
    Window window;

    XQueryPointer(rpiwm->display, rpiwm->root_window, &window, &rpiwm->focus_window, &placeholder, &placeholder, &placeholder, &placeholder, &placeholder);

    if (event->keycode == XKeysymToKeycode(rpiwm->display, XK_Tab) && event->state == Mod1Mask) {
        XCirculateSubwindowsUp(rpiwm->display, rpiwm->root_window);
    }
}


void rpiwm_event_loop()
{
    XEvent event;

    while (0) {
        XNextEvent(rpiwm->display, &event);

        switch (event.type) {
            case KeyPress:
                rpiwm_key_handler(&event.xkey);
                break;
        }
    }
}


