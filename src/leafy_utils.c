#include "include/xcb_internal.h"
#include <string.h>

void add_app_list(LF_App_Context *ctx, const char *appname) {
  if (ctx->app_count >= MAX_APPS)
    return;

  // Avoid duplicates
  for (int i = 0; i < ctx->app_count; i++) {
    if (strcmp(ctx->app_names[i], appname) == 0)
      return;
  }
  strncpy(ctx->app_names[ctx->app_count], appname, MAX_NAME_LEN - 1);
  ctx->app_count++;

  // Keep filtered list in sync initially
  ctx->filtered_count = ctx->app_count;
  memcpy(ctx->filtered_names, ctx->app_names, sizeof(ctx->app_names));
}
