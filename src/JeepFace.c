#include <pebble.h>
#include "binary_clock.h"
#include "digital_clock.h"
#include "morse_clock.h"
#include "indicators.h"
#include "settings.h"


static Window *window;

static Layer *s_top_layer;
static Layer *s_grill_layer;
static GBitmap *s_forest_image;
static GBitmap *s_logo_image;

static GPath *s_windshield_path = NULL;

static const GPathInfo WINDSHIELD_PATH_INFO = {
    .num_points = 25,
    .points = (GPoint[]) {
        // outside right
        {-5,-5},
        {149, -5},
        {149, 116},
        {127, 116},
        {127, 102},

        // inner right
        {128, 99},
        {139, 83},
        {139, 76},

        // right corner
        {138, 69},
        {136, 67},
        {134, 66},
        {131, 65},

        // across the hood
        {120, 64},
        {22 , 64},
        // left corner
        {11, 65},
        {8, 66},
        {6, 67},
        {4, 69},

        // inner left
        {3, 76},
        {3, 83},
        {14, 99},

        // outer left
        {15 , 102},
        {15 , 116},
        {-5 , 116},
        {-5, 70},
    }
};

static void grill_proc(Layer *layer, GContext *ctx) {
    // headlights
    graphics_fill_circle(ctx, GPoint(16, 22), 10);
    graphics_fill_circle(ctx, GPoint(126, 22), 10);

    // indicators
    graphics_fill_circle(ctx, GPoint(21,  40), 4);
    graphics_fill_circle(ctx, GPoint(121, 40), 4);


    int g = 3;
    int w = 7;
    int l = (144 - ((9*w) + (8*g))) / 2;
    for (int i = 0; i < 9; i++)
    {
        int y = 10;
        int h = 50;
        if(i == 0 || i == 8) {
            y = 12;
            h = 47;
        }
        graphics_context_set_fill_color(ctx, GColorBlack);
        graphics_fill_rect(ctx, GRect(l + (i * (w+g)), y, w, h), 3, GCornersAll);
        graphics_context_set_fill_color(ctx, GColorDarkGray);
        graphics_fill_rect(ctx, GRect(l + (i * (w+g))+1, y+1, w-2, h-2), 2, GCornersAll);
    }
}

static void windshield_proc(Layer *layer, GContext *ctx) {
    // fill it first
    graphics_context_set_fill_color(ctx, GColorArmyGreen);
    gpath_draw_filled(ctx,s_windshield_path);
    // now stroke it
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, GColorBlack);
    gpath_draw_outline(ctx, s_windshield_path);

    // glass
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, GRect(4, 4, 136, 50), 3, GCornersAll);
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_rect(ctx, GRect(5, 5, 134, 48), 3, GCornersAll);


    // forest
    // graphics_context_set_compositing_mode(ctx, GCompOpSet);
    // graphics_draw_bitmap_in_rect(ctx, s_forest_image, GRect( 6, 6, 132, 46));

    // logo
    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    graphics_draw_bitmap_in_rect(ctx, s_logo_image, GRect( 14, 6, 117, 46));

}

static void window_load() {
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "inside window_load()");
    // settings_load(window);
    // screen size 144 x 168
    if(false) {
        digital_clock_load(window);
    } else if (true) {
        morse_clock_load(window);
    } else {
        binary_clock_load(window);
    }

    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    

    s_windshield_path = gpath_create(&WINDSHIELD_PATH_INFO);

    s_top_layer = layer_create(GRect(0, 0, bounds.size.w, 130));
    layer_set_update_proc(s_top_layer, windshield_proc);
    layer_add_child(window_layer,s_top_layer);

    s_grill_layer = layer_create(GRect(0, 57, bounds.size.w, 60));
    layer_set_update_proc(s_grill_layer, grill_proc);
    layer_add_child(window_layer,s_grill_layer);

    indicators_load(window);
}

static void window_unload() {
    digital_clock_unload();
    binary_clock_unload();
    morse_clock_load();
    indicators_unload();
    // settings_unload();

    layer_destroy(s_grill_layer);

    gpath_destroy(s_windshield_path);
}

static void init() {
    window = window_create();

    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });

    const bool animated = true;
    window_stack_push(window, animated);

    s_forest_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FOREST);
    s_logo_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO);

}

static void deinit() {
    gbitmap_destroy(s_forest_image);
    gbitmap_destroy(s_logo_image);
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
