#include "../include/xcb_internal.h"

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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
        if (ctx->filtered_count > 0) {
          const char *cmd = ctx->filtered_execs[ctx->scroll_index];
          if (fork() == 0) {
            // Child process
            setsid(); // Detach from terminal
            char *args[] = {"/bin/sh", "-c", (char *)cmd, NULL};
            execvp(args[0], args);
            exit(0);
          }

          // Close menu
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
