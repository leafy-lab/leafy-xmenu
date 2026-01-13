#include "../include/xcb_internal.h"

#include <stdio.h>
#include <stdlib.h>

void xcb_event_loop(LF_App_Context *ctx) {
  xcb_generic_event_t *event;
  xcb_key_symbols_t *syms = xcb_key_symbols_alloc(ctx->connection);

  // Initial draw
  refresh_screen(ctx);

  while ((event = xcb_wait_for_event(ctx->connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_EXPOSE:
      refresh_screen(ctx);
      break;

    case XCB_KEY_PRESS: {
      xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
      xcb_keysym_t sym = xcb_key_symbols_get_keysym(syms, kp->detail, 0);

      if (sym == XK_Escape) {
        free(event);
        xcb_key_symbols_free(syms);
        return;
      } else if (sym == XK_Return) {
        if (ctx->app_count > 0) {
          int selected_idx = ctx->scroll_index;
          const char *selected_app = ctx->app_names[selected_idx];
          printf("Selected: %s\n", selected_app);

          free(event);
          xcb_key_symbols_free(syms);
          return;
        }
      } else if (sym == XK_BackSpace) {
        if (ctx->input_len > 0) {
          ctx->input_buffer[--ctx->input_len] = '\0';
          refresh_screen(ctx);
        }
      } else if (sym == XK_Down) {
        if (ctx->app_count > 2) {
          if (ctx->scroll_index < ctx->app_count - 2) {
            ctx->scroll_index++;
            refresh_screen(ctx);
          }
        }
      } else if (sym == XK_Up) {
        if (ctx->scroll_index > 0) {
          ctx->scroll_index--;
          refresh_screen(ctx);
        }
      } else {
        char ch = 0;
        if (sym >= XK_space && sym <= XK_asciitilde) {
          ch = (char)sym;
          if (kp->state & XCB_MOD_MASK_SHIFT) {
            if (ch >= 'a' && ch <= 'z') {
              ch -= 32;
            }
          }
          if (ctx->input_len < 255 && ch != 0) {
            ctx->input_buffer[ctx->input_len++] = ch;
            ctx->input_buffer[ctx->input_len] = '\0';
            refresh_screen(ctx);
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
