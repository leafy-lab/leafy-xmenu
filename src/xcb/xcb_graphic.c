#include "../include/xcb_internal.h"
#include <string.h>

static void draw_rounded_rect(LF_App_Context *ctx, int16_t x, int16_t y,
                              uint16_t w, uint16_t h, uint32_t color) {
  xcb_change_gc(ctx->connection, ctx->gc, XCB_GC_FOREGROUND, &color);
  xcb_rectangle_t rect = {x, y, w, h};
  xcb_poly_fill_rectangle(ctx->connection, ctx->window, ctx->gc, 1, &rect);
}

static void draw_text(LF_App_Context *ctx, int16_t x, int16_t y,
                      const char *text, uint32_t color) {
  if (!text || strlen(text) == 0)
    return;

  xcb_change_gc(ctx->connection, ctx->gc_text, XCB_GC_FOREGROUND, &color);

  xcb_image_text_8(ctx->connection, strlen(text), ctx->window, ctx->gc_text, x,
                   y, text);
}

void draw_ui(LF_App_Context *ctx) {
  // Clear background
  draw_rounded_rect(ctx, 0, 0, ctx->width, ctx->height, COLOR_BG);

  int16_t padding = 15;
  int16_t input_x = padding;
  int16_t input_y = 8;
  uint16_t input_w = ctx->width - (padding * 2);
  uint16_t input_h = ctx->height - 16;
  uint32_t color = COLOR_SELECTED;
  // Input background
  draw_rounded_rect(ctx, input_x, input_y, input_w, input_h, COLOR_INPUT_BG);

  // Draw prompt
  const char *prompt = ">";
  draw_text(ctx, padding + 8, ctx->height / 2 + 5, prompt, COLOR_SELECTED);

  // Draw input text
  if (ctx->input_len > 0) {
    draw_text(ctx, padding + 25, ctx->height / 2 + 5, ctx->input_buffer,
              COLOR_FG);
  }

  // Draw cursor
  int16_t cursor_x = padding + 25 + (ctx->input_len * 8);
  xcb_change_gc(ctx->connection, ctx->gc, XCB_GC_FOREGROUND, &color);
  xcb_rectangle_t cursor = {cursor_x, input_y + 6, 2, input_h - 12};
  xcb_poly_fill_rectangle(ctx->connection, ctx->window, ctx->gc, 1, &cursor);

  xcb_flush(ctx->connection);
}
