
#include <pebble.h>


#define KEY_SHOW_SECONDS 1
extern bool show_seconds;
const bool DEFAULT_SHOW_SECONDS = true;





void settings_load(Window *main_window) {
    // s_show_seconds = persist_exists(KEY_SHOW_SECONDS) ? persist_read_bool(KEY_SHOW_SECONDS) : DEFAULT_SHOW_SECONDS;
}

void settings_unload() {

}