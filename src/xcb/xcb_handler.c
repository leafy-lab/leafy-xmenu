#include "../include/xcb_internal.h"
#include <stdio.h>
#include <unistd.h>

int xcb_init(LF_App_Context *ctx) {
  ctx->connection = xcb_connect(NULL, NULL);
  if (xcb_connection_has_error(ctx->connection)) {
    fprintf(stderr, "leafy_xmenu: Failed to connect to X server");
    return -1;
  }

  const xcb_setup_t *setup = xcb_get_setup(ctx->connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  ctx->screen = iter.data;

  return 0;
}

void xcb_create_menu_window(LF_App_Context *ctx) {
  ctx->window = xcb_generate_id(ctx->connection);

  uint32_t values[] = {ctx->screen->black_pixel, // XCB_CW_BACK_PIXEL
                       1,                        // XCB_CW_OVERRIDE_REDIRECT
                       XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};

  xcb_create_window(
      ctx->connection, XCB_COPY_FROM_PARENT, ctx->window, ctx->screen->root, 0,
      20, ctx->width, ctx->height, 5, XCB_WINDOW_CLASS_INPUT_OUTPUT,
      ctx->screen->root_visual,
      XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK, values);

  xcb_map_window(ctx->connection, ctx->window);
  xcb_flush(ctx->connection);
}

void xcb_menu_graphic_init(LF_App_Context *ctx) {

  ctx->graphic = xcb_generate_id(ctx->connection);

  uint32_t gc_values[] = {
      ctx->screen->white_pixel,
      ctx->screen->black_pixel,
  };
  uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
  xcb_create_gc(ctx->connection, ctx->graphic, ctx->window, mask, gc_values);
  xcb_flush(ctx->connection);
}

// TODO: xcb_cleanup
