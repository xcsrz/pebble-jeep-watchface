
#include <pebble.h>
#include "binary_clock.h"
#include "digital_clock.h"
#include "morse_clock.h"
#include "settings.h"
#include "enums.h"

static int current_display_type;
static bool initial_load;

void unload_current_watchface() {
	
	switch(current_display_type) {
        case BINARY:
		    APP_LOG(APP_LOG_LEVEL_DEBUG, "UNLOADING BINARY");
		    binary_clock_unload();
        	break;
        case DIGITAL:
		    APP_LOG(APP_LOG_LEVEL_DEBUG, "UNLOADING DIGITAL");
		    digital_clock_unload();
        	break;
        case MORSE:
		    APP_LOG(APP_LOG_LEVEL_DEBUG, "UNLOADING MORSE");
		    morse_clock_unload();
        	break;
	}
}

void watchface_load(Window *main_window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded status %d", initial_load);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "SWITCHING TO %d FROM %d", current_display_type, display_type);
	if(initial_load) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Unloading watchface before reloading");
		unload_current_watchface();
	}
    switch(display_type) {
        case DIGITAL:
            digital_clock_load(main_window);
            break;
        case BINARY:
            binary_clock_load(main_window);
            break;
        case MORSE:
            morse_clock_load(main_window);
            break;
    }	
	current_display_type = display_type;
	initial_load = true;
}


void watchface_unload() {
	unload_current_watchface();
}