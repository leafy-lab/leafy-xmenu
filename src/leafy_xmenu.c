#include "include/xcb_internal.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  LF_App_Context app = {0};
  app.width = 1366;
  app.height = 20;

  if (xcb_init(&app) < 0) {
    fprintf(stderr, "Failed to init XCB\n");
    return 1;
  }

  app.app_count = 0;
  app.scroll_index = 0;

  populate_apps(&app);

  xcb_create_menu_window(&app);
  xcb_menu_graphic_init(&app);
  refresh_screen(&app);

  xcb_event_loop(&app);
  return 0;
}
