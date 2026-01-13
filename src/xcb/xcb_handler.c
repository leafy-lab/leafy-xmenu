#include "../include/xcb_internal.h"
#include <stdlib.h>
#include <string.h>

int xcb_init(LF_App_Context *ctx) {
  ctx->connection = xcb_connect(NULL, NULL);
  if (xcb_connection_has_error(ctx->connection)) {
    return -1;
  }

  const xcb_setup_t *setup = xcb_get_setup(ctx->connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  ctx->screen = iter.data;

  ctx->width = 600;
  ctx->height = 150;
  ctx->running = 1;
  ctx->input_len = 0;
  memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));

  return 0;
}

void xcb_create_menu_window(LF_App_Context *ctx) {
  ctx->window = xcb_generate_id(ctx->connection);

  uint32_t values[] = {COLOR_BG, // XCB_CW_BACK_PIXEL
                       1,        // XCB_CW_OVERRIDE_REDIRECT
                       XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};

  int16_t x_pos = (ctx->screen->width_in_pixels - ctx->width) / 2;
  int16_t y_pos = ctx->screen->height_in_pixels / 4;
  xcb_create_window(
      ctx->connection, XCB_COPY_FROM_PARENT, ctx->window, ctx->screen->root,
      x_pos, y_pos, ctx->width, ctx->height,
      2, // border
      XCB_WINDOW_CLASS_INPUT_OUTPUT, ctx->screen->root_visual,
      XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK, values);

  xcb_map_window(ctx->connection, ctx->window);

  // Grab keyboard
  xcb_grab_keyboard(ctx->connection, 1, ctx->window, XCB_CURRENT_TIME,
                    XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);

  xcb_flush(ctx->connection);
}

void xcb_menu_graphic_init(LF_App_Context *ctx) {
  // Create main graphics context
  ctx->gc = xcb_generate_id(ctx->connection);
  uint32_t gc_values[] = {COLOR_FG};
  xcb_create_gc(ctx->connection, ctx->gc, ctx->window, XCB_GC_FOREGROUND,
                gc_values);

  // Load font fallback to fixed
  const char *fonts[] = {
      "-*-dejavu sans mono-medium-r-normal--14-*-*-*-*-*-*-*",
      "-*-liberation mono-medium-r-normal--14-*-*-*-*-*-*-*",
      "-*-courier-medium-r-normal--14-*-*-*-*-*-*-*", "fixed"};

  ctx->font = xcb_generate_id(ctx->connection);
  xcb_generic_error_t *error = NULL;

  for (int i = 0; i < 4; i++) {
    xcb_void_cookie_t cookie = xcb_open_font_checked(
        ctx->connection, ctx->font, strlen(fonts[i]), fonts[i]);
    error = xcb_request_check(ctx->connection, cookie);
    if (!error) {
      break;
    }
    free(error);
  }

  ctx->gc_text = xcb_generate_id(ctx->connection);
  uint32_t text_gc_values[] = {COLOR_FG, COLOR_BG, ctx->font};
  uint32_t text_mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
  xcb_create_gc(ctx->connection, ctx->gc_text, ctx->window, text_mask,
                text_gc_values);

  xcb_flush(ctx->connection);
}

void xcb_cleanup(LF_App_Context *ctx) {
  if (ctx->gc_text)
    xcb_free_gc(ctx->connection, ctx->gc_text);
  if (ctx->gc)
    xcb_free_gc(ctx->connection, ctx->gc);
  if (ctx->font)
    xcb_close_font(ctx->connection, ctx->font);
  if (ctx->window)
    xcb_destroy_window(ctx->connection, ctx->window);
  if (ctx->connection)
    xcb_disconnect(ctx->connection);
}
