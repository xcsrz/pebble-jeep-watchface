#pragma once

int display_type;
bool show_seconds;
GColor jeep_color;
GColor time_color;
GColor background_color;

void app_message_received();

void load_watchface();

void settings_load();

void settings_unload();