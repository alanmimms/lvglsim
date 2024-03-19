#include <stdio.h>
#include <stdlib.h>
#include <lvgl.h>

#define HSIZE 800
#define VSIZE 480


static lv_disp_draw_buf_t drawBuf;
static lv_color_t drawBuf1[HSIZE * VSIZE / 10];
static lv_disp_drv_t displayDriver;
static lv_indev_drv_t indevDriver;


static void flushCB(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  int32_t x, y;

  /*It's a very slow but simple implementation.
   *`set_pixel` needs to be written by you to a set pixel on the screen*/
  for(y = area->y1; y <= area->y2; y++) {
    for(x = area->x1; x <= area->x2; x++) {
      set_pixel(x, y, *color_p);
      color_p++;
    }
  }

  lv_disp_flush_ready(disp);
}


static void indevRead(lv_indev_t *indev, lv_indev_data_t *data) {

  /*`touchpad_is_pressed` and `touchpad_get_xy` needs to be implemented by you*/
  if(touchpad_is_pressed()) {
    data->state = LV_INDEV_STATE_PRESSED;
    touchpad_get_xy(&data->point.x, &data->point.y);
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}


int main(int argc, char *argv[]) {
  lv_init();
  lv_disp_draw_buf_init(&drawBuf, drawBuf1, NULL, HSIZE * VSIZE / 10);

  /*Basic initialization*/
  lv_disp_drv_init(&displayDriver);

  disp_drv.flush_cb = flushCB;
  disp_drv.draw_buf = &drawBuf;
  disp_drv.hor_res = HSIZE;
  disp_drv.ver_res = VSIZE;
  lv_disp_drv_register(&displayDriver);

  lv_indev_drv_init(&indevDriver);
  indevDriver.type = LV_INDEV_TYPE_POINTER;
  indevDriver.read_cb = indevRead;
  lv_indev_drv_register(&indevDriver);

  /*Handle LVGL tasks*/
  while(1) {
    lv_timer_handler();
    usleep(5000);
  }
}
