#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "rpiwm.h"


int main(int argc, char *argv[])
{
    rpiwm_init();

    exit(0);
}


void rpiwm_init()
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

    rpiwm_set_desktop_bgcolor(RPI_COLOR);
    rpiwm_grab_keys();
    rpiwm_event_loop();
}


void rpiwm_set_desktop_bgcolor(char *hex)
{
    XColor color;
    XParseColor(rpiwm->display, DefaultColormap(rpiwm->display, rpiwm->screen), hex, &color);
    XAllocColor(rpiwm->display, DefaultColormap(rpiwm->display, rpiwm->screen), &color);

    XSetWindowBackground(rpiwm->display, rpiwm->root_window, color.pixel);
    XClearWindow(rpiwm->display, rpiwm->root_window);
}


void rpiwm_grab_keys()
{
    XGrabKey(rpiwm->display, XKeysymToKeycode(rpiwm->display, XK_Tab), Mod1Mask, rpiwm->root_window, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(rpiwm->display, XKeysymToKeycode(rpiwm->display, XK_F11), Mod1Mask, rpiwm->root_window, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(rpiwm->display, XKeysymToKeycode(rpiwm->display, XK_F12), Mod1Mask, rpiwm->root_window, True, GrabModeAsync, GrabModeAsync);
    XGrabButton(rpiwm->display, 1, Mod1Mask, rpiwm->root_window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
}


/**
 * Handle all pressed keys
 **/
void rpiwm_key_handler(XKeyEvent *event)
{
    int placeholder;
    Window window;

    XQueryPointer(rpiwm->display, rpiwm->root_window, &window, &rpiwm->focus_window, &placeholder, &placeholder, &placeholder, &placeholder, (unsigned int*)&placeholder);

    // Alt+Tab = Cycle
    //TODO: Meta+Tab = Cycle
    if (event->keycode == XKeysymToKeycode(rpiwm->display, XK_Tab) && event->state == Mod1Mask) {
        XCirculateSubwindowsUp(rpiwm->display, rpiwm->root_window);
    }

    // Alt+F12: Exit WM
    if (event->keycode == XKeysymToKeycode(rpiwm->display, XK_F12) && event->state == Mod1Mask) {
        exit(0);
    }
}


void rpiwm_button_pressed_handler(XButtonEvent *event)
{
    if (event->subwindow != None) {
        XGrabPointer(rpiwm->display, event->subwindow, True, PointerMotionMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

        XGetWindowAttributes(rpiwm->display, event->subwindow, &rpiwm->attributes);
        rpiwm->start = *event;
    }
}


void rpiwm_button_release_handler()
{
    rpiwm->start.subwindow = None;
    XUngrabPointer(rpiwm->display, CurrentTime);
}


void rpiwm_motion_notify_handler(XButtonEvent *event)
{
    if (rpiwm->start.subwindow != None) {
        int xdiff = event->x_root - rpiwm->start.x_root;
        int ydiff = event->y_root - rpiwm->start.y_root;

        XMoveResizeWindow(rpiwm->display, rpiwm->start.subwindow,
                          rpiwm->attributes.x + (rpiwm->start.button ==1 ? xdiff : 0),
                          rpiwm->attributes.y + (rpiwm->start.button ==1 ? ydiff : 0),
                          MAX(1, rpiwm->attributes.width + (rpiwm->start.button ==3 ? xdiff : 0)),
                          MAX(1, rpiwm->attributes.height + (rpiwm->start.button ==3 ? ydiff : 0)));
    }
}


void rpiwm_event_loop()
{
    XEvent event;

    rpiwm->start.subwindow = None;

    while (1) {
        XNextEvent(rpiwm->display, &event);

        switch (event.type) {
            case KeyPress:
                rpiwm_key_handler(&event.xkey);
                break;
            case ButtonPress:
                rpiwm_button_pressed_handler(&event.xbutton);
                break;
            case ButtonRelease:
                rpiwm_button_release_handler();
                break;
            case MotionNotify:
                rpiwm_motion_notify_handler(&event.xbutton);
                break;
        }
    }
}

