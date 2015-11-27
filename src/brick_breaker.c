#include <pebble.h>
#include "brick_breaker.h"

#define PLAT_BOUND_LEFT = 8
#define PLAT_BOUND_RIGHT = 156
#define REPEAT_INTERVAL 50

static Window *window;
static Layer *platform_layer;
static GPath *platform_path;
static void platform_update_proc(Layer *layer, GContext *ctx);

static int platform_x_pos = 53;

static void update_plat() {
  layer_mark_dirty(window_get_root_layer(window));
  gpath_move_to(platform_path, GPoint(platform_x_pos, 156));
}

static void move_left() {
  //up button
  if (platform_x_pos >= 0) {
    platform_x_pos -= 5;
  }
  
  update_plat();
}

static void move_right() {
  //down button
  if (platform_x_pos <= 114) {
    platform_x_pos += 5;
  }
  
  update_plat();
}

static void platform_update_proc(Layer *layer, GContext *ctx) {
  // Could change this later for platform movement OR could make a new func with gpath_move_to()
  
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  
  gpath_draw_filled(ctx, platform_path);
  gpath_draw_outline(ctx, platform_path);
  
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	move_right();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	move_left();
}

static void click_config_provider(void *context) {
	window_single_repeating_click_subscribe(BUTTON_ID_UP, REPEAT_INTERVAL, (ClickHandler) up_click_handler);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, REPEAT_INTERVAL, (ClickHandler) down_click_handler);
}

static void main_window_load() {
  Layer *window_layer = window_get_root_layer(window);
  
  platform_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(platform_layer, platform_update_proc);
  layer_add_child(window_layer, platform_layer);
  
  
}

static void main_window_unload() {
  gpath_destroy(platform_path);
  layer_destroy(platform_layer);
}

static void init() {
  window = window_create();
  
  window_set_background_color(window, GColorWhite);
  #ifdef PBL_SDK_2 
    window_set_fullscreen(window, true);
  #endif
  
  window_set_click_config_provider(window, click_config_provider);
  
  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  window_stack_push(window, true);
  
  platform_path = gpath_create(&platform_path_points);
  gpath_move_to(platform_path, GPoint(53, 156));
}

static void deinit() {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  return 0;
}