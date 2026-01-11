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
} LF_App_Context;

int xcb_init(LF_App_Context *ctx);
void xcb_create_menu_window(LF_App_Context *ctx);
void xcb_cleanup(LF_App_Context *ctx);

#endif // XCB_INTERNAL_H
