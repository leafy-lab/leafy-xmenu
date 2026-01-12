#ifndef XCB_INTERNAL_H
#define XCB_INTERNAL_H

#include <X11/keysym.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#define COLOR_BG 0x1e1e2e
#define COLOR_FG 0xcdd6f4
#define COLOR_SELECTED 0x89b4fa
#define COLOR_BORDER 0x45475a
#define COLOR_INPUT_BG 0x313244

typedef struct {
  xcb_connection_t *connection;
  xcb_screen_t *screen;
  xcb_window_t window;
  uint16_t width;
  uint16_t height;
  int running;
  xcb_gcontext_t gc;
  xcb_gcontext_t gc_text;
  xcb_font_t font;
  char input_buffer[256];
  int input_len;
} LF_App_Context;

int xcb_init(LF_App_Context *ctx);
void xcb_create_menu_window(LF_App_Context *ctx);
void xcb_menu_graphic_init(LF_App_Context *ctx);
void xcb_event_loop(LF_App_Context *ctx);
void xcb_cleanup(LF_App_Context *ctx);
void draw_ui(LF_App_Context *ctx);

#endif // XCB_INTERNAL_H
