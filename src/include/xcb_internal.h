#ifndef XCB_INTERNAL_H
#define XCB_INTERNAL_H

#include <xcb/xcb.h>
#include <xcb/xproto.h>

typedef struct {
  xcb_connection_t *connection;
  xcb_screen_t *screen;
  xcb_window_t window;

  // State
  uint16_t width;
  uint16_t height;
  int running;
  // Graphic
  xcb_gcontext_t graphic;

} LF_App_Context;

typedef struct {
  xcb_generic_event_t *event;
} LF_App_Event;

int xcb_init(LF_App_Context *ctx);
void xcb_create_menu_window(LF_App_Context *ctx);
void xcb_menu_graphic_init(LF_App_Context *ctx);
void xcb_event_loop(LF_App_Context *ctx);
void xcb_cleanup(LF_App_Context *ctx);

#endif // XCB_INTERNAL_H
