#include "../include/xcb_internal.h"
#include <stdlib.h>

void xcb_event_loop(LF_App_Context *ctx) {
  xcb_generic_event_t *event;
  xcb_key_symbols_t *syms = xcb_key_symbols_alloc(ctx->connection);

  while ((event = xcb_wait_for_event(ctx->connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_EXPOSE:
      // draw
      draw_input_box(ctx);
      xcb_flush(ctx->connection);
      break;
    case XCB_KEY_PRESS:
      xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
      xcb_keysym_t sym = xcb_key_symbols_get_keysym(syms, kp->detail, 0);

      if (sym == XK_Escape)
      {
        xcb_ungrab_keyboard(ctx->connection, XCB_CURRENT_TIME);
        xcb_destroy_window(ctx->connection, ctx->window);
        xcb_flush(ctx->connection);
        free(event);
        xcb_key_symbols_free(syms);
        return;
      }
      break;
    default:
      break;
    }
    free(event);
  }
}