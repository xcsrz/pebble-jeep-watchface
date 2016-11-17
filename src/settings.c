
#include <pebble.h>
#include "watchface.h"
#include "enums.h"


// #define KEY_DISPLAY_TYPE 0
// #define KEY_JEEP_MODEL 1
// #define KEY_SHOW_SECONDS 2
// #define KEY_JEEP_COLOR 3
// #define KEY_TIME_COLOR 4
// #define KEY_BACKGROUND_COLOR 5
// #define KEY_TWENTY_FOUR 6

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
	// Tuple *jeep_model_t = dict_find(iter, KEY_JEEP_MODEL);

	Tuple *display_type_t = dict_find(iter, MESSAGE_KEY_type);
	if(display_type_t) {
		// display_type = display_type_t->value->int32;
		switch(display_type_t->value->cstring[0]) {
			case 'b':
				display_type = 0;
				break;
			case 'd':
				display_type = 1;
				break;
			// case 'm':
			default:
				display_type = 2;
				break;
		}
				
		persist_write_int(MESSAGE_KEY_type, display_type);
	}

	Tuple *show_seconds_t = dict_find(iter, MESSAGE_KEY_seconds);
	if(show_seconds_t) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Seconds setting received as: %d", show_seconds_t->value->int32);
		show_seconds = show_seconds_t->value->int32 == 1;
		persist_write_bool(MESSAGE_KEY_seconds,show_seconds);
	}

	Tuple *jeep_color_t = dict_find(iter, MESSAGE_KEY_jeepColor);
	if(jeep_color_t) {
		int jeep_color_i = jeep_color_t->value->int32;
		persist_write_int(MESSAGE_KEY_jeepColor, jeep_color_i);
		jeep_color = GColorFromHEX(jeep_color_i);
	}

	Tuple *time_color_t = dict_find(iter, MESSAGE_KEY_timeColor);
	if(time_color_t) {
		int time_color_i = time_color_t->value->int32;
		persist_write_int(MESSAGE_KEY_timeColor, time_color_i);
		time_color = GColorFromHEX(time_color_i);
	}

	Tuple *background_color_t = dict_find(iter, MESSAGE_KEY_bgColor);
	if(background_color_t) {
		int background_color_i = background_color_t->value->int32;
		persist_write_int(MESSAGE_KEY_bgColor, background_color_i);
		background_color = GColorFromHEX(background_color_i);
	    window_set_background_color(main_window,background_color);
	}
	watchface_load(main_window);
}

void settings_load(Window *window) {

	// APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_DISPLAY_TYPE: %d", KEY_DISPLAY_TYPE);
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "MESSAGE_KEY_type: %d", MESSAGE_KEY_type);
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_SHOW_SECONDS: %d", KEY_SHOW_SECONDS);
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "MESSAGE_KEY_seconds: %d", MESSAGE_KEY_seconds);

	main_window = window;

    display_type = persist_exists(MESSAGE_KEY_type) ? persist_read_int(MESSAGE_KEY_type) : DEFAULT_DISPLAY_TYPE;
    show_seconds = persist_exists(MESSAGE_KEY_seconds) ? persist_read_bool(MESSAGE_KEY_seconds) : DEFAULT_SHOW_SECONDS;
    jeep_color = persist_exists(MESSAGE_KEY_jeepColor) ? GColorFromHEX(persist_read_int(MESSAGE_KEY_jeepColor)) : GColorArmyGreen;
    time_color = persist_exists(MESSAGE_KEY_timeColor) ? GColorFromHEX(persist_read_int(MESSAGE_KEY_timeColor)) : GColorArmyGreen;
    background_color = persist_exists(MESSAGE_KEY_bgColor) ? GColorFromHEX(persist_read_int(MESSAGE_KEY_bgColor)) : GColorClear;
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Background COLOR %d", background_color);
    window_set_background_color(window,background_color);
}

void settings_unload() {

}