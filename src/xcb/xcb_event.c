#include "../include/xcb_internal.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void xcb_event_loop(LF_App_Context *ctx) {
  xcb_generic_event_t *event;
  xcb_key_symbols_t *syms = xcb_key_symbols_alloc(ctx->connection);

  // Initial draw
  draw_ui(ctx);

  while ((event = xcb_wait_for_event(ctx->connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_EXPOSE:
      draw_ui(ctx);
      break;

    case XCB_KEY_PRESS: {
      xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
      xcb_keysym_t sym = xcb_key_symbols_get_keysym(syms, kp->detail, 0);

      if (sym == XK_Escape) {
        free(event);
        xcb_key_symbols_free(syms);
        return;
      } else if (sym == XK_Return) {
        // Handle selection
        if (ctx->input_len > 0) {
          // Process input (print for now)
          printf("Selected: %s\n", ctx->input_buffer);
        }
        free(event);
        xcb_key_symbols_free(syms);
        return;
      } else if (sym == XK_BackSpace) {
        if (ctx->input_len > 0) {
          ctx->input_buffer[--ctx->input_len] = '\0';
          draw_ui(ctx);
        }
      } else {
        // Handle regular character input
        char ch = 0;

        // Simple ASCII conversion
        if (sym >= XK_space && sym <= XK_asciitilde) {
          ch = (char)sym;

          // Handle shift for uppercase
          if (kp->state & XCB_MOD_MASK_SHIFT) {
            if (ch >= 'a' && ch <= 'z') {
              ch = toupper(ch);
            }
          }

          if (ctx->input_len < 255 && ch != 0) {
            ctx->input_buffer[ctx->input_len++] = ch;
            ctx->input_buffer[ctx->input_len] = '\0';
            draw_ui(ctx);
          }
        }
      }
      break;
    }

    default:
      break;
    }
    free(event);
  }

  xcb_key_symbols_free(syms);
}
