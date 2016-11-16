
#include <pebble.h>
#include "watchface.h"
#include "enums.h"


#define KEY_DISPLAY_TYPE 0
#define KEY_JEEP_MODEL 1
#define KEY_SHOW_SECONDS 2
#define KEY_JEEP_COLOR 3
#define KEY_TIME_COLOR 4
#define KEY_BACKGROUND_COLOR 5

const int DEFAULT_DISPLAY_TYPE = MORSE; // int value of 'd'
const bool DEFAULT_SHOW_SECONDS = false;

// extern int TYPES;
extern int display_type;
// extern char jeep_model = "mb";

extern bool show_seconds;
extern GColor jeep_color;
extern GColor time_color;
extern GColor background_color;

static Window *main_window;

void app_message_received(DictionaryIterator *iter, void *context) {
	Tuple *display_type_t = dict_find(iter, KEY_DISPLAY_TYPE);
	// Tuple *jeep_model_t = dict_find(iter, KEY_JEEP_MODEL);
	Tuple *show_seconds_t = dict_find(iter, KEY_SHOW_SECONDS);
	Tuple *jeep_color_t = dict_find(iter, KEY_JEEP_COLOR);
	Tuple *time_color_t = dict_find(iter, KEY_TIME_COLOR);
	Tuple *background_color_t = dict_find(iter, KEY_BACKGROUND_COLOR);

	if(display_type_t) {
		display_type = display_type_t->value->int32;
		persist_write_int(KEY_DISPLAY_TYPE,display_type);
	}

	if(show_seconds_t) {
		show_seconds = show_seconds_t->value->int32;
		persist_write_bool(KEY_SHOW_SECONDS,show_seconds);
	}

	if(jeep_color_t) {
		int jeep_color_i = jeep_color_t->value->int32;
		persist_write_int(KEY_JEEP_COLOR, jeep_color_i);
		jeep_color = GColorFromHEX(jeep_color_i);
	}

	if(time_color_t) {
		int time_color_i = time_color_t->value->int32;
		persist_write_int(KEY_TIME_COLOR, time_color_i);
		time_color = GColorFromHEX(time_color_i);
	}

	if(background_color_t) {
		int background_color_i = background_color_t->value->int32;
		persist_write_int(KEY_BACKGROUND_COLOR, background_color_i);
		background_color = GColorFromHEX(background_color_i);
	    window_set_background_color(main_window,background_color);
	}
	watchface_load(main_window);
}

void settings_load(Window *window) {
	main_window = window;

    display_type = persist_exists(KEY_DISPLAY_TYPE) ? persist_read_int(KEY_DISPLAY_TYPE) : DEFAULT_DISPLAY_TYPE;
    show_seconds = persist_exists(KEY_SHOW_SECONDS) ? persist_read_bool(KEY_SHOW_SECONDS) : DEFAULT_SHOW_SECONDS;
    jeep_color = persist_exists(KEY_JEEP_COLOR) ? GColorFromHEX(persist_read_int(KEY_JEEP_COLOR)) : GColorArmyGreen;
    time_color = persist_exists(KEY_TIME_COLOR) ? GColorFromHEX(persist_read_int(KEY_TIME_COLOR)) : GColorArmyGreen;
    background_color = persist_exists(KEY_BACKGROUND_COLOR) ? GColorFromHEX(persist_read_int(KEY_BACKGROUND_COLOR)) : GColorClear;
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Background COLOR %d", background_color);
    window_set_background_color(window,background_color);
}

void settings_unload() {

}