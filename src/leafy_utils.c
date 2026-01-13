#include "include/xcb_internal.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>

void add_app_list(LF_App_Context *ctx, const char *appname) {
  if (ctx->app_count >= MAX_APPS)
    return;

  for (int i = 0; i < ctx->app_count; i++) {
    if (strcmp(ctx->app_names[i], appname) == 0)
      return;
  }
  strncpy(ctx->app_names[ctx->app_count], appname, MAX_NAME_LEN - 1);
  ctx->app_count++;

  ctx->filtered_count = ctx->app_count;
  memcpy(ctx->filtered_names, ctx->app_names, sizeof(ctx->app_names));
}

void populate_apps(LF_App_Context *ctx) {
  const char *path = "/usr/share/applications/";
  struct dirent *entry;
  DIR *dp = opendir(path);

  if (dp == NULL)
    return;

  while ((entry = readdir(dp))) {
    // Only look at .desktop files
    if (strstr(entry->d_name, ".desktop")) {
      char full_path[512];
      snprintf(full_path, sizeof(full_path), "%s%s", path, entry->d_name);

      FILE *f = fopen(full_path, "r");
      if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
          if (strncmp(line, "Name=", 5) == 0) {
            char *name = line + 5;
            name[strcspn(name, "\n")] = 0;

            add_app_list(ctx, name);
            break;
          }
        }
        fclose(f);
      }
    }
  }
  closedir(dp);
}
