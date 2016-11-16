
#include <pebble.h>
#include "settings.h"

static Window *s_window;
static Layer *s_window_layer;
static Layer *layers[60];
static int layers_count = 0;

typedef struct {
	Layer* A0;
	Layer* A1;
	Layer* A2;
	Layer* A3;
	Layer* A4;
	Layer* B0;
	Layer* B1;
	Layer* B2;
	Layer* B3;
	Layer* B4;
	int* field;
} Position;

typedef struct {
  int hours;
  int minutes;
  int seconds;
} Time;

static Time s_time;

static Position s_hour_markers;
static Position s_minute_markers;
static Position s_second_markers;

static void update_position(Position position) {
	int field_one = *position.field % 10;
	// int field_one = s_time.seconds % 10;
	layer_set_hidden(position.B0, field_one >= 1 && field_one < 6);
	layer_set_hidden(position.B1, field_one >= 2 && field_one < 7);
	layer_set_hidden(position.B2, field_one >= 3 && field_one < 8);
	layer_set_hidden(position.B3, field_one >= 4 && field_one < 9);
	layer_set_hidden(position.B4, field_one >= 5);

	layer_set_hidden(position.A0, *position.field >= 10 && *position.field < 60);
	layer_set_hidden(position.A1, *position.field >= 20 && *position.field < 70);
	layer_set_hidden(position.A2, *position.field >= 30 && *position.field < 80);
	layer_set_hidden(position.A3, *position.field >= 40 && *position.field < 90);
	layer_set_hidden(position.A4, *position.field >= 50);

}

static void unload_position(Position position) {
	while(layers_count > 0)
	{
		layer_destroy(layers[--layers_count]);
	}
}

static void update_positions() {
    update_position(s_hour_markers);
    update_position(s_minute_markers);
    update_position(s_second_markers);
}


static void fill_layer(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    graphics_context_set_fill_color(ctx, time_color);
    graphics_fill_rect(ctx, bounds, 3, GCornersAll);
}

static void track_layer(Layer *layer) {
	layers[layers_count++] = layer;
}

static Layer* add_marker(int x, int y, int w, int h) {
	Layer *hidden_layer = layer_create(GRect(x + w/3, y, w/3, h));
    layer_set_update_proc(hidden_layer, fill_layer);
    layer_add_child(s_window_layer, hidden_layer);

	Layer *new_layer = layer_create(GRect(x, y, w, h));
    layer_set_update_proc(new_layer, fill_layer);
    layer_add_child(s_window_layer, new_layer);

    track_layer(hidden_layer);
    track_layer(new_layer);

    return new_layer;
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    s_time.hours = tick_time->tm_hour;
    if(!clock_is_24h_style()) {
	    s_time.hours -= (s_time.hours > 12) ? 12 : 0;
	    if(s_time.hours == 0) {
	        s_time.hours = 12;
	    }
    }
    s_time.minutes = tick_time->tm_min;
    // if(s_show_seconds){
        s_time.seconds = tick_time->tm_sec;
    // }
    update_positions();
}

static Position create_position(int size, int offset) {
	Position new_position;

	new_position.A4 = add_marker(offset,120,size,8);
	new_position.A3 = add_marker(offset,129,size,8);
	new_position.A2 = add_marker(offset,138,size,8);
	new_position.A1 = add_marker(offset,147,size,8);
	new_position.A0 = add_marker(offset,156,size,8);

	new_position.B4 = add_marker(offset + size,120,size,8);
	new_position.B3 = add_marker(offset + size,129,size,8);
	new_position.B2 = add_marker(offset + size,138,size,8);
	new_position.B1 = add_marker(offset + size,147,size,8);
	new_position.B0 = add_marker(offset + size,156,size,8);

	return new_position;
}

void morse_clock_load(Window *main_window) {
	s_window = main_window;
	s_window_layer = window_get_root_layer(main_window);
    GRect bounds = layer_get_bounds(s_window_layer);
	// Layer *first = add_marker(50,120,20,20);
	// Layer *second = add_marker(100,120,20,20);
	// layer_set_hidden(second, false);


	int size;
	if(show_seconds) {
		size = (bounds.size.w / 6);
	} else {
		size = (bounds.size.w / 4);
	}

	// s_test = create_position(size);
	// s_test.field = &s_time.seconds;

	s_hour_markers = create_position(size, 0);
	s_hour_markers.field = &s_time.hours;

	s_minute_markers = create_position(size, size*2);
	s_minute_markers.field = &s_time.minutes;

	s_second_markers = create_position(size, size*4);
	s_second_markers.field = &s_time.seconds;

    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

void morse_clock_unload() {
    tick_timer_service_unsubscribe();
    unload_position(s_hour_markers);
    unload_position(s_minute_markers);
    unload_position(s_second_markers);
}