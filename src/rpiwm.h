#ifndef _RPIWM_H
#define _RPIWM_H

#include <stdlib.h>
#include <X11/Xlib.h>

#define RPI_COLOR "#BB1042"
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct _rpiwm_atoms_t {
    Atom xa_wm_protos;
    Atom xa_wm_delete;
    Atom xa_wm_state;
    Atom xa_wm_change_state;
} rpiwm_atoms_t;


typedef struct _rpiwm_t {
    Display *display;
    char    *display_options;
    int     screen;
    Window  root_window;
    Window  focus_window;
    int     x, y;

    rpiwm_atoms_t *atoms;
    XWindowAttributes attributes;

    XButtonEvent start;
} rpiwm_t;


static rpiwm_t *rpiwm;

void rpiwm_init();
void rpiwm_event_loop();
void rpiwm_grab_keys();
void rpiwm_key_handler(XKeyEvent *);
void rpiwm_set_desktop_bgcolor(char*);

#endif
