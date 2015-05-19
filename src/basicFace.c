// ***************** //
// * Steve Groves  * //
// * May 2015      * //
// * Pebble Steel  * //
// * watch face    * //
// ***************** //

#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_messageyear_layer;
static TextLayer *s_messagedate_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer for time and date
  static char time[] = "00:00";
  static char date[] = "00/00";
  static char year[] = "00000";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(time, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(time, sizeof("00:00"), "%I:%M", tick_time);
  }
  // display the date in day/month format
  strftime(date, sizeof("00/00"), "%d/%m", tick_time);
  
  strftime(year, sizeof("00000"), " %Y", tick_time);
  
  // Display time and date on their TextLayer(s)
  text_layer_set_text(s_time_layer, time);
  text_layer_set_text(s_messagedate_layer, date);
  text_layer_set_text(s_messageyear_layer, year);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window)
  {
// time layer on display
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

// year layer on display  
  s_messageyear_layer = text_layer_create(GRect (0, 120, 144, 50));
  text_layer_set_background_color(s_messageyear_layer, GColorBlack);
  text_layer_set_text_color(s_messageyear_layer, GColorWhite);  
  text_layer_set_font(s_messageyear_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(s_messageyear_layer, GTextAlignmentRight);

// date layer on display  
  s_messagedate_layer = text_layer_create(GRect (0, 0, 144, 50));
  text_layer_set_background_color(s_messagedate_layer, GColorBlack);
  text_layer_set_text_color(s_messagedate_layer, GColorWhite);  
  text_layer_set_font(s_messagedate_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28)); 
  text_layer_set_text_alignment(s_messagedate_layer, GTextAlignmentLeft);

// show the layers on the display - create three child layers on parent (root)  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_messageyear_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_messagedate_layer));
}

static void main_window_unload(Window *window)
  {
// kill all layers when we stop using the watch face  
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_messageyear_layer);
  text_layer_destroy(s_messagedate_layer);
}

// this code is used to initialise the watch face
static void init()
  {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers)
                             {
                               .load = main_window_load,
                               .unload = main_window_unload 
                             });
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
  
  // check every minute for changes
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);                           
}

static void deinit()
  {
  window_destroy(s_main_window);
}

// main programme sequence for the app
int main(void)
  {
  init();
  app_event_loop();
  deinit();
}