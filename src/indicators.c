#include <pebble.h>

#define     BATTERY_WARNING_LEVEL   21  

static Window *window;

static Layer *s_power_layer;
static Layer *s_bluetooth_layer;

static int s_battery_level;

static GBitmap *s_power_image;
static GBitmap *s_bluetooth_image;

static GPath *s_bluetooth_path = NULL;
static GPath *s_battery_path = NULL;


static const GPathInfo BLUETOOTH_PATH_INFO = {
    .num_points = 6,
    // .points = (GPoint[]) {{4, 7}, {16,13}, {10, 16}, {10, 4}, {16, 7}, {4, 13}} // original
    // .points = (GPoint[]) { {2,2},{10,8},{6,10},{6,1},{10,3},{2,9} } // smaller, better
    .points = (GPoint[]) {
        {0,3},
        {9,10},
        {5,13},
        {5,1},
        {9,4},
        {0,11}
    }
};

static const GPathInfo BATTERY_PATH_INFO = {
    .num_points = 8,
    .points = (GPoint[]) {
        {1,0},
        {10,0},
        {12,2},
        {12,4},
        {10,6},
        {1,6},
        {0,5},
        {0,1},
    }
};

static void turn_on_light(GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorPastelYellow);
    graphics_fill_circle(ctx, GPoint(10, 10), 8);
}


static void power_proc(Layer *layer, GContext *ctx) {
    turn_on_light(ctx);
    gpath_move_to(s_battery_path, GPoint(4, 7));
    graphics_context_set_fill_color(ctx, GColorMelon);
    graphics_context_set_stroke_color(ctx, GColorDarkCandyAppleRed);
    gpath_draw_filled(ctx, s_battery_path);
    gpath_draw_outline(ctx, s_battery_path);
    graphics_draw_line(ctx,GPoint(6,9), GPoint(6,11));
}


static void bluetooth_proc(Layer *layer, GContext *ctx) {
    turn_on_light(ctx);
    gpath_rotate_to(s_bluetooth_path, TRIG_MAX_ANGLE / 360 * 75);
    // graphics_context_set_stroke_color(ctx, GColorDukeBlue);
    graphics_context_set_stroke_color(ctx, GColorDarkCandyAppleRed);
    gpath_move_to(s_bluetooth_path, GPoint(15, 5));
    gpath_draw_outline_open(ctx, s_bluetooth_path);  
}

void battery_callback(BatteryChargeState state) {
    layer_set_hidden(s_power_layer, state.charge_percent > BATTERY_WARNING_LEVEL);
}

void bluetooth_callback(bool connected) {
    layer_set_hidden(s_bluetooth_layer, connected);
}


void indicators_load(Window *main_window) {

	window = main_window;

    s_bluetooth_path = gpath_create(&BLUETOOTH_PATH_INFO);
    s_battery_path = gpath_create(&BATTERY_PATH_INFO);

    // s_bluetooth_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH);
    // s_power_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_POWER);


    Layer *window_layer = window_get_root_layer(window);
    // GRect bounds = layer_get_bounds(window_layer);


    s_bluetooth_layer = layer_create(GRect(6, 69, 20, 20));
    layer_set_update_proc(s_bluetooth_layer, bluetooth_proc);
    layer_add_child(window_layer, s_bluetooth_layer);

    s_power_layer = layer_create(GRect(116, 69, 20, 20));
    layer_set_update_proc(s_power_layer, power_proc);
    layer_add_child(window_layer, s_power_layer);


	bluetooth_callback(bluetooth_connection_service_peek());
    battery_callback(battery_state_service_peek());    

    bluetooth_connection_service_subscribe(bluetooth_callback);
    battery_state_service_subscribe(battery_callback);

}

void indicators_unload() {
    gbitmap_destroy(s_bluetooth_image);
    gbitmap_destroy(s_power_image);

    layer_destroy(s_power_layer);
    layer_destroy(s_bluetooth_layer);

    gpath_destroy(s_bluetooth_path);
    gpath_destroy(s_battery_path);


}