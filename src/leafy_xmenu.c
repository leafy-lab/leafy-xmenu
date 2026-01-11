#include "include/xcb_internal.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  LF_App_Context app = {0};
  app.width = 1366;
  app.height = 20;

  if (!xcb_init(&app)) {
    printf("ERROR");
  }

  xcb_create_menu_window(&app);

  return 0;
}
