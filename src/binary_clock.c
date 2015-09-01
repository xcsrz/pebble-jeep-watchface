#include <pebble.h>

#define     BOX_GAP         4
#define     CLOCK_HEIGHT    45

static Window *window;
static Layer *s_dots_layer;

typedef struct {
  int hours;
  int minutes;
  int seconds;
} Time;

static Time s_time;

static int s_box_width;
static int s_box_height;
static bool s_show_seconds = false;
static int s_hours_y;
static int s_minutes_y;
static int s_seconds_y;

static void draw_a_dot(GContext *ctx, int x, int y) {
    // highlight
    graphics_context_set_fill_color(ctx, GColorBrass);
    graphics_fill_rect(ctx, GRect(x, y, s_box_width, s_box_height), 3, GCornersAll);

    // shadow
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, GRect(x+1, y+1, s_box_width-1, s_box_height-1), 3, GCornersAll);

    // main dot
    graphics_context_set_fill_color(ctx, GColorMidnightGreen);
    graphics_fill_rect(ctx, GRect(x+1, y+1, s_box_width-2, s_box_height-2), 3, GCornersAll);
}

static void draw_a_row(GContext *ctx, int y, int val) {
    int block = 64;
    int x = BOX_GAP;
    while(val > 0) {
        block = block / 2;
        if(val >= block) {
            draw_a_dot(ctx, x, y);
            val = val % block;
        }
        x = x + (s_box_width + BOX_GAP);
    }
}

static void update_my_dots(Layer *layer, GContext *ctx) {
    draw_a_row(ctx, s_hours_y, s_time.hours);
    draw_a_row(ctx, s_minutes_y, s_time.minutes);
    if(s_show_seconds) {   
        draw_a_row(ctx, s_seconds_y, s_time.seconds);
    }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    s_time.hours = tick_time->tm_hour;
    s_time.hours -= (s_time.hours > 12) ? 12 : 0;
    if(s_time.hours == 0) {
        s_time.hours = 12;
    }
    s_time.minutes = tick_time->tm_min;
    if(s_show_seconds){
        s_time.seconds = tick_time->tm_sec;
    }

    if(s_dots_layer) {
        // This will clear the layer and trigger update_my_dots
        layer_mark_dirty(s_dots_layer);
    }
}

void binary_clock_load(Window *main_window) {
	window = main_window;

	Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
	s_box_width = (bounds.size.w - (7 * BOX_GAP)) / 6;

    // determine where the rows will be
    if(s_show_seconds) {
        s_box_height = (CLOCK_HEIGHT - 3*BOX_GAP) / 3;
        s_hours_y = bounds.size.h - CLOCK_HEIGHT;
        s_minutes_y = s_hours_y + s_box_height + BOX_GAP;
        s_seconds_y = s_minutes_y + s_box_height + BOX_GAP;
    } else {
        s_box_height = (CLOCK_HEIGHT - 2*BOX_GAP) / 2;
        s_hours_y = bounds.size.h - CLOCK_HEIGHT;
        s_minutes_y = s_hours_y + s_box_height + BOX_GAP;
    }

    s_dots_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
    layer_set_update_proc(s_dots_layer, update_my_dots);
    layer_add_child(window_layer, s_dots_layer);

    if(s_show_seconds) {
        tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    } else {
        tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    }

}

void binary_clock_unload() {
    layer_destroy(s_dots_layer);
}
