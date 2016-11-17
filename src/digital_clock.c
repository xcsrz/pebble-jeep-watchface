
#include <pebble.h>
#include "settings.h"

static TextLayer *s_clock_layer;

typedef struct {
  int hours;
  int minutes;
  int seconds;
  bool pm;
} Time;

static Time s_time;
// static bool s_show_seconds = true;
static GFont s_font;



static void update_my_clock() {
    text_layer_set_text_color(s_clock_layer, time_color);

    static char s_buffer[15];
    char *pos = s_buffer;

    if(show_seconds) {
        // snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d:%02d\n%s", s_time.hours, s_time.minutes, s_time.seconds, (s_time.pm) ? "PM" : "AM" );
        pos += snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d:%02d", s_time.hours, s_time.minutes, s_time.seconds);
    } else {
        // snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d\n%s", s_time.hours, s_time.minutes, (s_time.pm) ? "PM" : "AM" );
        pos += snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d", s_time.hours, s_time.minutes);
    }

    if(!clock_is_24h_style()) {
        pos += snprintf(pos, sizeof(s_buffer) - strlen(s_buffer), " %s", (s_time.pm) ? "PM" : "AM");
    }
	text_layer_set_text(s_clock_layer, s_buffer);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    s_time.hours = tick_time->tm_hour;
    if(!clock_is_24h_style()) {
        if (s_time.hours > 12) {
    	    s_time.hours -= 12;
        	s_time.pm = true;
        }
        if(s_time.hours == 0) {
            s_time.hours = 12;
            s_time.pm = false;
        }
        else if(s_time.hours == 12) {
            s_time.pm = true;
        }
    }
    s_time.minutes = tick_time->tm_min;
    s_time.seconds = tick_time->tm_sec;
    update_my_clock();
}

void digital_clock_load(Window *main_window) {
	Layer *window_layer = window_get_root_layer(main_window);
    GRect bounds = layer_get_bounds(window_layer);

	s_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BLACK_OPS_26));


    s_clock_layer = text_layer_create(GRect(0, bounds.size.h - 55, bounds.size.w, 50));

	text_layer_set_font(s_clock_layer, s_font);

	text_layer_set_text(s_clock_layer, "checking time");
	text_layer_set_overflow_mode(s_clock_layer, GTextOverflowModeWordWrap);
	text_layer_set_text_alignment(s_clock_layer, GTextAlignmentCenter);
    text_layer_set_background_color(s_clock_layer,GColorClear);
    layer_add_child(window_layer, text_layer_get_layer(s_clock_layer));

    if(show_seconds) {
        tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    } else {
        tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    }
}

void digital_clock_unload() {
    tick_timer_service_unsubscribe();
    settings_unload();
	text_layer_destroy(s_clock_layer);
}