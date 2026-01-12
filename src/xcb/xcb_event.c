#include "../include/xcb_internal.h"
#include <stdlib.h>

void xcb_event_loop(LF_App_Context *ctx) {
  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(ctx->connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_EXPOSE:
      // draw
      draw_input_box(ctx);
      xcb_flush(ctx->connection);
      break;
    case XCB_KEY_PRESS:
      // handle key press
      break;
    default:
      break;
    }
    free(event);
  }
}