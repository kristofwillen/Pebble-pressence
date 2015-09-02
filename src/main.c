#include <pebble.h>
#include "analogue.h"
  
Window *my_window;
TextLayer *date_layer;
//static BitmapLayer *s_dials_layer;
static GBitmap *s_background_bitmap, *s_fuel_bitmap, *s_dow_bitmap, *s_hour_bitmap;
static GPath *s_minute_arrow, *s_hour_arrow, *s_fuel_arrow, *s_week_arrow;
static Layer *s_hands_layer;
static BitmapLayer *s_background_layer, *s_fuel_layer, *s_dow_layer, *s_hour_layer;
int batteryLevel = 100;
static char s_date_buffer[10];
int32_t hour_angle_loc_old, fuel_angle_loc_old, dow_angle_loc_old = 0;
static int dow_x, dow_y, dow_old_x, dow_old_y, hour_x, hour_y, hour_old_x, hour_old_y, fuel_x, fuel_y, fuel_old_x, fuel_old_y = 0;

static void battery_handler(BatteryChargeState new_state) {
  
  batteryLevel = (int)new_state.charge_percent;
}


void animation_stopped(Animation *animation, bool finished, void *data) {
   
  #ifdef PBL_COLOR
    // Do nothing, Basalt does this automagically
  #else  
    property_animation_destroy((PropertyAnimation*) animation);
  #endif
}


static void hands_update_proc(Layer *layer, GContext *ctx) {
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  //int16_t second_hand_length = bounds.size.w / 2;
  int dow_length  = 42;
  int fuel_length = 42;
  int hour_length = 32;
  static GRect dow_to_frame, dow_from_frame, hour_to_frame, hour_from_frame, fuel_to_frame, fuel_from_frame;
  static PropertyAnimation *s_dow_animation, *s_hour_animation, *s_fuel_animation;

 
  strftime(s_date_buffer, sizeof(s_date_buffer), "%d", t);
  text_layer_set_text(date_layer, s_date_buffer);
  
  
  //int32_t second_angle = (TRIG_MAX_ANGLE * t->tm_sec / 90 - (TRIG_MAX_ANGLE/3)) % TRIG_MAX_ANGLE;
  int32_t minute_angle = TRIG_MAX_ANGLE * t->tm_min/60;
  //int32_t hour_angle   = (TRIG_MAX_ANGLE * (t->tm_hour%12 + t->tm_min / 60) / 12) % TRIG_MAX_ANGLE;
  int32_t hour_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
  int32_t week_angle   = ((TRIG_MAX_ANGLE * t->tm_wday / 7) - TRIG_MAX_ANGLE/16)%TRIG_MAX_ANGLE;
  int32_t fuel_angle   = ((TRIG_MAX_ANGLE / 3 * batteryLevel / 100) + (TRIG_MAX_ANGLE * 300/360)) % TRIG_MAX_ANGLE;
  
  int32_t dow_plate_angle  = (minute_angle + TRIG_MAX_ANGLE/7)%TRIG_MAX_ANGLE;
  int32_t hour_plate_angle = (minute_angle - TRIG_MAX_ANGLE/2)%TRIG_MAX_ANGLE;
  int32_t fuel_plate_angle = (minute_angle - TRIG_MAX_ANGLE/7)%TRIG_MAX_ANGLE;
  
  dow_x = (int16_t)(sin_lookup(dow_plate_angle) * (int32_t)dow_length / TRIG_MAX_RATIO) + center.x - 20;
  dow_y = (int16_t)(-cos_lookup(dow_plate_angle) * (int32_t)dow_length / TRIG_MAX_RATIO) + center.y - 20;
  dow_from_frame  = GRect(dow_old_x, dow_old_y, 39,37);
  dow_to_frame    = GRect(dow_x, dow_y,39,37);
  s_dow_animation = property_animation_create_layer_frame((Layer *)s_dow_layer, &dow_from_frame, &dow_to_frame);
  animation_set_duration((Animation *)s_dow_animation,2000);
  animation_set_handlers((Animation*) s_dow_animation, (AnimationHandlers) {
    .stopped = (AnimationStoppedHandler) animation_stopped,
  }, NULL);
  
  
  hour_x = (int16_t)(sin_lookup(hour_plate_angle) * (int32_t)hour_length / TRIG_MAX_RATIO) + center.x - 33;
  hour_y = (int16_t)(-cos_lookup(hour_plate_angle) * (int32_t)hour_length / TRIG_MAX_RATIO) + center.y - 33;
  hour_from_frame  = GRect(hour_old_x, hour_old_y, 67,67);
  hour_to_frame    = GRect(hour_x, hour_y,67,67);
  s_hour_animation = property_animation_create_layer_frame((Layer *)s_hour_layer, &hour_from_frame, &hour_to_frame);
  animation_set_duration((Animation *)s_hour_animation,2000);
  animation_set_handlers((Animation*) s_hour_animation, (AnimationHandlers) {
    .stopped = (AnimationStoppedHandler) animation_stopped,
  }, NULL);
  
  
  fuel_x = (int16_t)(sin_lookup(fuel_plate_angle) * (int32_t)fuel_length / TRIG_MAX_RATIO) + center.x - 18;
  fuel_y = (int16_t)(-cos_lookup(fuel_plate_angle) * (int32_t)fuel_length / TRIG_MAX_RATIO) + center.y - 8;
  fuel_from_frame  = GRect(fuel_old_x, fuel_old_y, 36,17);
  fuel_to_frame    = GRect(fuel_x, fuel_y,36,17);
  s_fuel_animation = property_animation_create_layer_frame((Layer *)s_fuel_layer, &fuel_from_frame, &fuel_to_frame);
  animation_set_duration((Animation *)s_fuel_animation,2000);
  animation_set_handlers((Animation*) s_fuel_animation, (AnimationHandlers) {
    .stopped = (AnimationStoppedHandler) animation_stopped,
  }, NULL);
  
  animation_schedule((Animation*) s_dow_animation);
  animation_schedule((Animation*) s_hour_animation);
  animation_schedule((Animation*) s_fuel_animation);
  
  gpath_move_to(s_hour_arrow, GPoint(hour_x+33,hour_y+33));
  graphics_context_set_fill_color(ctx, GColorCyan);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  gpath_rotate_to(s_hour_arrow, hour_angle);
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);
  
  graphics_context_set_fill_color(ctx, GColorRajah);
  gpath_rotate_to(s_minute_arrow, minute_angle);
  gpath_draw_filled(ctx, s_minute_arrow);
  gpath_draw_outline(ctx, s_minute_arrow);
  
  gpath_move_to(s_week_arrow, GPoint(dow_x+20,dow_y+19));
  graphics_context_set_fill_color(ctx, GColorYellow);
  gpath_rotate_to(s_week_arrow, week_angle);
  gpath_draw_filled(ctx, s_week_arrow);
  gpath_draw_outline(ctx, s_week_arrow);
  
  gpath_move_to(s_fuel_arrow, GPoint(fuel_x+18,fuel_y+16));
  graphics_context_set_fill_color(ctx, GColorFolly);
  gpath_rotate_to(s_fuel_arrow, fuel_angle);
  gpath_draw_filled(ctx, s_fuel_arrow);
  gpath_draw_outline(ctx, s_fuel_arrow);
  
  dow_old_x  = dow_x;
  dow_old_y  = dow_y;
  hour_old_x = hour_x;
  hour_old_y = hour_y;
  fuel_old_x = fuel_x;
  fuel_old_y = fuel_y;
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(my_window));
}



void handle_init(void) {
  my_window = window_create();
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0,0,144,168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(s_background_layer));
  
  s_dow_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_DOW);
  s_dow_layer = bitmap_layer_create(GRect(0,0,39,37));
  bitmap_layer_set_bitmap(s_dow_layer, s_dow_bitmap);
  layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(s_dow_layer));
  
  s_fuel_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_FUEL);
  s_fuel_layer = bitmap_layer_create(GRect(50,28,36,17));
  bitmap_layer_set_bitmap(s_fuel_layer, s_fuel_bitmap);
  layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(s_fuel_layer));
  
  s_hour_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_HOUR_TRANS);
  s_hour_layer = bitmap_layer_create(GRect(0,0,67,67));
  bitmap_layer_set_bitmap(s_hour_layer, s_hour_bitmap);
  bitmap_layer_set_compositing_mode(s_hour_layer,GCompOpSet);
  layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(s_hour_layer));
  
  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
  
  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
  s_fuel_arrow = gpath_create(&FUEL_HAND_POINTS);
  s_week_arrow   = gpath_create(&WEEK_HAND_POINTS);
  
  gpath_move_to(s_minute_arrow, GPoint(74,86));
  gpath_move_to(s_hour_arrow, GPoint(50,99));
  gpath_move_to(s_week_arrow, GPoint(105,106));
  gpath_move_to(s_fuel_arrow, GPoint(68,45));

 
  date_layer = text_layer_create(GRect(115, 145, 20, 20));
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorIcterine);
  
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(date_layer));
  
  window_stack_push(my_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Get the current battery level
  battery_handler(battery_state_service_peek());
  }


void handle_deinit(void) {
  text_layer_destroy(date_layer);
  bitmap_layer_destroy(s_dow_layer);
  bitmap_layer_destroy(s_fuel_layer);
  bitmap_layer_destroy(s_hour_layer);
  bitmap_layer_destroy(s_background_layer);
  window_destroy(my_window);
  tick_timer_service_unsubscribe();
}


int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
