#include "../include/xcb_internal.h"

void draw_input_box(LF_App_Context *ctx) {
  xcb_rectangle_t rect = {
      .x = 0, .y = 0, .width = ctx->width, .height = ctx->height};

  /* background */
  xcb_poly_fill_rectangle(ctx->connection, ctx->window, ctx->graphic, 1, &rect);
}
