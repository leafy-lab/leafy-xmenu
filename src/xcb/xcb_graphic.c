#include "../include/xcb_internal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void draw_rect(LF_App_Context *ctx, int16_t x, int16_t y, uint16_t width,
               uint16_t height, uint32_t color, const char *app_name) {
  uint16_t border = 2;
  xcb_change_gc(ctx->connection, ctx->gc, XCB_GC_FOREGROUND, &color);
  xcb_rectangle_t outer_rect = {x, y, width, height};
  xcb_poly_rectangle(ctx->connection, ctx->window, ctx->gc, 1, &outer_rect);

  uint32_t inner_color = 0x282a36;
  xcb_change_gc(ctx->connection, ctx->gc, XCB_GC_FOREGROUND, &inner_color);
  xcb_rectangle_t inner_rect = {(int16_t)(x + border), (int16_t)(y + border),
                                (uint16_t)(width - 2 * border),
                                (uint16_t)(height - 2 * border)};
  xcb_poly_fill_rectangle(ctx->connection, ctx->window, ctx->gc, 1,
                          &inner_rect);

  if (app_name) {
    uint32_t text_color = 0xf8f8f2;
    xcb_change_gc(ctx->connection, ctx->gc_text, XCB_GC_FOREGROUND,
                  &text_color);

    int tx = x + (width / 2) - (strlen(app_name) * 4);
    int ty = y + (height / 2) + 5;

    xcb_image_text_8(ctx->connection, strlen(app_name), ctx->window,
                     ctx->gc_text, tx, ty, app_name);
  }

  xcb_flush(ctx->connection);
}

static void draw_rounded_rect(LF_App_Context *ctx, int16_t x, int16_t y,
                              uint16_t w, uint16_t h, uint32_t color,
                              int radius) {
  xcb_change_gc(ctx->connection, ctx->gc, XCB_GC_FOREGROUND, &color);

  if (radius <= 0) {
    xcb_rectangle_t rect = {x, y, w, h};
    xcb_poly_fill_rectangle(ctx->connection, ctx->window, ctx->gc, 1, &rect);
    return;
  }

  // Main body
  xcb_rectangle_t rects[3] = {
      {x + radius, y, w - 2 * radius, h},                  // Center horizontal
      {x, y + radius, radius, h - 2 * radius},             // Left edge
      {x + w - radius, y + radius, radius, h - 2 * radius} // Right edge
  };
  xcb_poly_fill_rectangle(ctx->connection, ctx->window, ctx->gc, 3, rects);

  xcb_rectangle_t corners[4] = {
      {x, y, radius, radius},                          // Top-left
      {x + w - radius, y, radius, radius},             // Top-right
      {x, y + h - radius, radius, radius},             // Bottom-left
      {x + w - radius, y + h - radius, radius, radius} // Bottom-right
  };
  xcb_poly_fill_rectangle(ctx->connection, ctx->window, ctx->gc, 4, corners);
}

static void draw_text(LF_App_Context *ctx, int16_t x, int16_t y,
                      const char *text, uint32_t color) {
  if (!text || strlen(text) == 0)
    return;

  // Update text color
  xcb_change_gc(ctx->connection, ctx->gc_text, XCB_GC_FOREGROUND, &color);
  xcb_image_text_8(ctx->connection, strlen(text), ctx->window, ctx->gc_text, x,
                   y, text);
}

void draw_ui(LF_App_Context *ctx) {
  int16_t radius = 12;
  int16_t padding = 20;
  int16_t border = 2;
  uint32_t color = COLOR_SELECTED;
  // Clear with background
  draw_rounded_rect(ctx, 0, 0, ctx->width, ctx->height, COLOR_BG, radius);

  // Draw border effect
  draw_rounded_rect(ctx, border, border, ctx->width - 2 * border,
                    ctx->height - 2 * border, COLOR_BG, radius);

  int16_t text_y = (ctx->height / 2) + 7; // +7 for proper baseline

  const char *prompt = "->";
  draw_text(ctx, padding, text_y, prompt, COLOR_SELECTED);

  // Draw input text
  int16_t text_x = padding + 30;
  if (ctx->input_len > 0) {
    draw_text(ctx, text_x, text_y, ctx->input_buffer, COLOR_FG);
  } else {
    draw_text(ctx, text_x, text_y, "Type to search...", 0x6272a4);
  }

  // Draw cursor
  if (ctx->input_len >= 0) {
    xcb_query_text_extents_cookie_t cookie;
    if (ctx->input_len > 0) {
      cookie =
          xcb_query_text_extents(ctx->connection, ctx->font, ctx->input_len,
                                 (xcb_char2b_t *)ctx->input_buffer);
      xcb_query_text_extents_reply_t *reply =
          xcb_query_text_extents_reply(ctx->connection, cookie, NULL);
      if (reply) {
        text_x += reply->overall_width;
        free(reply);
      }
    }

    int16_t cursor_h = 20;
    int16_t cursor_y = (ctx->height - cursor_h) / 2;
    xcb_change_gc(ctx->connection, ctx->gc, XCB_GC_FOREGROUND, &color);
    xcb_rectangle_t cursor = {text_x + 2, cursor_y, 2, cursor_h};
    xcb_poly_fill_rectangle(ctx->connection, ctx->window, ctx->gc, 1, &cursor);
  }

  xcb_flush(ctx->connection);
}
