#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include "include/xcb_internal.h"

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_app_list(LF_App_Context *ctx, const char *appname, const char *exec) {
  if (ctx->app_count >= MAX_APPS)
    return;

  strncpy(ctx->app_names[ctx->app_count], appname, MAX_NAME_LEN - 1);

  char *clean_exec = strdup(exec);
  if (clean_exec) {
    char *arg_pos = strchr(clean_exec, '%');
    if (arg_pos)
      *arg_pos = '\0';

    size_t len = strlen(clean_exec);
    while (len > 0 && isspace((unsigned char)clean_exec[len - 1])) {
      clean_exec[--len] = '\0';
    }

    strncpy(ctx->app_execs[ctx->app_count], clean_exec, MAX_NAME_LEN - 1);
    free(clean_exec);
  }

  ctx->app_count++;
}

void populate_apps(LF_App_Context *ctx) {
  const char *path = "/usr/share/applications/";
  struct dirent *entry;
  DIR *dp = opendir(path);
  if (!dp)
    return;

  while ((entry = readdir(dp))) {
    if (strstr(entry->d_name, ".desktop")) {
      char full_path[1024];
      snprintf(full_path, sizeof(full_path), "%s%s", path, entry->d_name);
      FILE *f = fopen(full_path, "r");
      if (!f)
        continue;

      char line[1024];
      char name[MAX_NAME_LEN] = {0};
      char exec[MAX_NAME_LEN] = {0};

      while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Name=", 5) == 0 && name[0] == '\0') {
          strncpy(name, line + 5, MAX_NAME_LEN - 1);
          name[strcspn(name, "\r\n")] = 0;
        } else if (strncmp(line, "Exec=", 5) == 0 && exec[0] == '\0') {
          strncpy(exec, line + 5, MAX_NAME_LEN - 1);
          exec[strcspn(exec, "\r\n")] = 0;
        }
      }
      if (name[0] && exec[0]) {
        add_app_list(ctx, name, exec);
      }
      fclose(f);
    }
  }
  closedir(dp);

  ctx->filtered_count = ctx->app_count;
  for (int i = 0; i < ctx->app_count; i++) {
    strncpy(ctx->filtered_names[i], ctx->app_names[i], MAX_NAME_LEN - 1);
    strncpy(ctx->filtered_execs[i], ctx->app_execs[i], MAX_NAME_LEN - 1);
  }
}
