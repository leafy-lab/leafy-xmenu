#ifndef XCB_INTERNAL_H
#define XCB_INTERNAL_H

#include <X11/keysym.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#define COLOR_BG 0x282a36
#define COLOR_FG 0xf8f8f2
#define COLOR_SELECTED 0xbd93f9
#define COLOR_BORDER 0x6272a4
#define COLOR_INPUT_BG 0x44475a
#define COLOR_SHADOW 0x000000

#define MAX_APPS 356
#define MAX_NAME_LEN 128

#define MIN(a, b) ((a) < (b) ? (a) : (b))

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

  char app_names[MAX_APPS][MAX_NAME_LEN];
  int app_count;
  char filtered_names[MAX_APPS][MAX_NAME_LEN];
  int filtered_count;
  int scroll_index;
} LF_App_Context;

int xcb_init(LF_App_Context *ctx);
void xcb_create_menu_window(LF_App_Context *ctx);
void xcb_menu_graphic_init(LF_App_Context *ctx);
void xcb_event_loop(LF_App_Context *ctx);
void xcb_cleanup(LF_App_Context *ctx);
void draw_ui(LF_App_Context *ctx);
void draw_rect(LF_App_Context *ctx, int16_t x, int16_t y, uint16_t width,
               uint16_t height, uint32_t color, const char *app_name);
void refresh_screen(LF_App_Context *ctx);

void add_app_list(LF_App_Context *ctx, const char *appname);
void populate_apps(LF_App_Context *ctx);

#endif // XCB_INTERNAL_H
