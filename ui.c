#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "lvgl.h"

static const char *TAG = "wallclockUI";

// THIS COULD BE HELPFUL:
// https://components.espressif.com/components/espressif/esp_lvgl_port
// https://github.com/pjaos/mgos_esp32_littlevgl_wifi_setup/tree/master
// https://github.com/lvgl/lv_port_pc_eclipse
// https://docs.lvgl.io/8/get-started/pc-simulator.html


// XXX remove this after debugging
static const char *evToName[] = {
  "EVENT_ALL",

  /** Input device events*/
  "EVENT_PRESSED",
  "EVENT_PRESSING",
  "EVENT_PRESS_LOST",
  "EVENT_SHORT_CLICKED",
  "EVENT_LONG_PRESSED",
  "EVENT_LONG_PRESSED_REPEAT",
  "EVENT_CLICKED",
  "EVENT_RELEASED",
  "EVENT_SCROLL_BEGIN",
  "EVENT_SCROLL_END",
  "EVENT_SCROLL",
  "EVENT_GESTURE",
  "EVENT_KEY",
  "EVENT_FOCUSED",
  "EVENT_DEFOCUSED",
  "EVENT_LEAVE",
  "EVENT_HIT_TEST",

  /** Drawing events*/
  "EVENT_COVER_CHECK",
  "EVENT_REFR_EXT_DRAW_SIZE",
  "EVENT_DRAW_MAIN_BEGIN",
  "EVENT_DRAW_MAIN",
  "EVENT_DRAW_MAIN_END",
  "EVENT_DRAW_POST_BEGIN",
  "EVENT_DRAW_POST",
  "EVENT_DRAW_POST_END",
  "EVENT_DRAW_PART_BEGIN",
  "EVENT_DRAW_PART_END",

  /** Special events*/
  "EVENT_VALUE_CHANGED",
  "EVENT_INSERT",
  "EVENT_REFRESH",
  "EVENT_READY",
  "EVENT_CANCEL",

  /** Other events*/
  "LV_EVENT_DELETE",
  "EVENT_CHILD_CHANGED",
  "EVENT_CHILD_CREATED",
  "EVENT_CHILD_DELETED",
  "EVENT_SCREEN_UNLOAD_START",
  "EVENT_SCREEN_LOAD_START",
  "EVENT_SCREEN_LOADED",
  "EVENT_SCREEN_UNLOADED",
  "EVENT_SIZE_CHANGED",
  "EVENT_STYLE_CHANGED",
  "EVENT_LAYOUT_CHANGED",
  "EVENT_GET_SELF_SIZE",
};



#define HRESOLUTION 800
#define VRESOLUTION 480

#define LVGL_TICK_PERIOD_MS 10

static int hours = 0;
static int minutes = 0;
static int seconds = 0;

// Our icons created with https://lvgl.io/tools/imageconverter
extern const lv_img_dsc_t visible;
extern const lv_img_dsc_t invisible;
extern const lv_img_dsc_t cog;


static struct {
  lv_style_t base;
  lv_style_t icon;
  lv_style_t widget;
  lv_style_t time;
  lv_style_t selectedItem;
  lv_style_t pageHeader;
} styles;


static lv_disp_t *disp;
static lv_obj_t *keyboard;
extern lv_font_t LoraBold240;
extern lv_font_t RobotoMedium40;


// The display has one LVGL "screen" loaded at a time, and this is
// used to set the UI mode for wallclock (`timeUI`), settings
// (`settingsUI`), WiFi SSID scanner (`wifiUI`), WiFi SSID password
// entry (`passwordUI`), etc. The default at startup time is to load
// the `timeUI` screen.
//
// Each UI has a `screen` that contains its UI elements. The struct
// for each UI contains only the things I have to dink around with
// after creation time.  I don't bother putting things like static
// headings in this since they never need to be accessed after they're
// initially configured properly.
static struct {
  lv_obj_t *screen;
  lv_obj_t *settingsButton;
  lv_obj_t *time;
  lv_obj_t *seconds;
  lv_obj_t *ampm;
  lv_obj_t *tz;
  lv_obj_t *date;
  lv_obj_t *apName;
  lv_obj_t *ntpName;
} timeUI;


#define N_NTP_SERVERS	3

static struct {
  lv_obj_t *screen;
  lv_obj_t *twelveHr;		/* Checkbox */
  lv_obj_t *showSeconds;	/* Checkbox */
  lv_obj_t *showDayDate;	/* Checkbox */
  lv_obj_t *ntpGrid;		/* Grid for NTP text boxes */
  lv_obj_t *ntp[N_NTP_SERVERS];	/* Each is a textarea */
  lv_obj_t *tzGrid;		/* Grid for TZ text box */
  lv_obj_t *tz;			/* Timezone in TZ envar format (e.g., PST-08 for US Pacific) */
  lv_obj_t *ok;			/* Button */
  lv_obj_t *cancel;		/* Button */
} settingsUI;


static struct {
  lv_obj_t *screen;
  lv_obj_t *list;
  lv_obj_t *ok;
  lv_obj_t *cancel;
  lv_obj_t *wifiList;		/* button matrix */
} wifiUI;


static struct {
  lv_obj_t *screen;
  lv_obj_t *pass;		/* textarea userdata=keyboard when focused */
  lv_obj_t *visible;
  lv_obj_t *ok;
  lv_obj_t *cancel;
} passwordUI;


// The settings as they are currently set. These come from NVS
// initially, are modified by the settingsUI, wifiUI, and passwordUI,
// and are saved back to NVS when changed.
static struct {
  uint8_t twelveHr;	  /* Set if 12hr is needed (24hr otherwise) */
  uint8_t showSeconds;	  /* Display and update seconds */
  uint8_t showDayDate;	  /* Display day of week and date */
  char *tz;		  /* String for TZ envar */
  char *ntp[3];		  /* String for NTP sites */
  char *ssid;		  /* String for current selected WiFi SSID */
  char *password;	  /* String for current selected Wifi SSID's password */
} settings = {
  .twelveHr = 0,
  .showSeconds = 1,
  .showDayDate = 1,
  .tz = "PST",
  .ntp[0] = "pool.ntp.org",
  .ntp[1] = "time.google.com",
  .ntp[2] = "clock.uregina.ca",
  .ssid = "My Sooper Seekyure WiFi",
  .password = "s00per-skeQret",
};


// Platform API hack so I can test UI on a PC.
#if LV_PC_SIM
#include <stdio.h>
#define ESP_LOGI(TAG, FMT, VA...) fprintf(stderr, "[Info] %25s: " FMT "\n", TAG, ##VA)
#define ESP_LOGW(TAG, FMT, VA...) fprintf(stderr, "[Warn] %25s: " FMT "\n", TAG, ##VA)
#define ESP_LOGE(TAG, FMT, VA...) fprintf(stderr, "[Err!] %25s: " FMT "\n", TAG, ##VA)
#endif


// Timer callback for updating the displayed time and date every period.
//
// XXX Does this get called with lvgl_port_lock already held or do I
// need to protect here or will that hang or what?
//
// https://components.espressif.com/components/espressif/esp_lvgl_port
//
// lvgl_port_lock(0);
// ...
// lvgl_port_unlock();
static void secondsCB(lv_timer_t *timerP) {

  if (++seconds > 59) {
    seconds = 0;

    if (++minutes > 59) {
      minutes = 0;

      if (++hours > 23) hours = 0;
    }
  }

#if 0
  time_t now;
  struct tm tm;
  time(&now);
  tzset();
  localtime_r(&now, &tm);
  
  char buf[64];
  int st;
  char *timeFmtP = settings.twelveHr ? "%I:%M%p" : "%R";

  st = strftime(buf, sizeof(buf)-1, timeFmtP, &tod);
  lv_label_set_text(timeUI.time, buf);
  lv_label_set_text_fmt(timeUI.seconds, "%02d", seconds);

  int st = strftime(buf, sizeof(buf)-1, "%A %B %d, %Y");
  lv_label_set_text(timeUI.date, date);
#endif
}


static void cogClickedCB(lv_event_t *ev) {
  lv_scr_load(settingsUI.screen);
}


static void setupClockUI(void) {
  // For each block that configures an LVGL object, use this pointer
  // to avoid copypasta errors.
  lv_obj_t *p;

  ESP_LOGI(TAG, "[set up clock UI]");

  p = timeUI.screen = lv_obj_create(NULL);
  lv_obj_add_style(p, &styles.base, LV_PART_MAIN);

  // Set up the default style for the large-ish text. Most of the
  // objects on this screen use this.
  lv_obj_set_style_text_font(p, &RobotoMedium40, LV_PART_MAIN);

  // Left edge of hh:mm
  static const int timeL = 32;
  // Top edge of hh:mm
  static const int timeT = 32;

  // Padding below hh:mm:ss before top fo timezone
  static const int timeBottomPad = 10;

  // The hours:minutes
  p = timeUI.time = lv_label_create(timeUI.screen);
  lv_label_set_text_static(p, "00:00");
  lv_obj_set_style_text_font(p, &LoraBold240, LV_PART_MAIN);
  lv_obj_set_pos(p, timeL, timeT);

  // The AM/PM indicator
  if (settings.twelveHr || 1) {
    p = timeUI.ampm = lv_label_create(timeUI.screen);
    lv_label_set_text_static(p, "AM");
    lv_obj_set_align(p, LV_ALIGN_TOP_LEFT);
    lv_obj_align_to(p, timeUI.time, LV_ALIGN_TOP_RIGHT, 48, 36);
  }

  // The seconds
  p = timeUI.seconds = lv_label_create(timeUI.screen);
  lv_label_set_text_static(p, "00");
  lv_obj_set_align(p, LV_ALIGN_BOTTOM_LEFT);
  lv_obj_align_to(p, timeUI.time, LV_ALIGN_BOTTOM_RIGHT, 48, -50);

  // The timezone
  p = timeUI.tz = lv_label_create(timeUI.screen);
  lv_label_set_text_static(p, "PST");
  lv_obj_set_align(p, LV_ALIGN_TOP_RIGHT);
  lv_obj_align_to(p, timeUI.seconds, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, timeBottomPad);

  // The day/date
  p = timeUI.date = lv_label_create(timeUI.screen);
  lv_label_set_text_static(p, "Blurday Franuary 33, 1999");
  lv_obj_set_align(p, LV_ALIGN_TOP_RIGHT);
  lv_obj_align_to(p, timeUI.tz, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);

  // The WiFi SSID in status bar
  p = timeUI.apName = lv_label_create(timeUI.screen);
  lv_obj_set_align(p, LV_ALIGN_BOTTOM_LEFT);
  lv_label_set_text_static(p, settings.ssid);
  lv_obj_set_width(p, LV_SIZE_CONTENT);

  // The NTP source site in status bar
  p = timeUI.ntpName = lv_label_create(timeUI.screen);
  lv_obj_set_align(p, LV_ALIGN_BOTTOM_RIGHT);
  lv_label_set_text_static(p, settings.ntp[0]);
  lv_obj_set_width(p, LV_SIZE_CONTENT);

  // The settings (cog) button
  p = timeUI.settingsButton = lv_btn_create(timeUI.screen);
  lv_obj_align_to(p, timeUI.screen, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_size(p, 32, 32);
  lv_obj_add_event_cb(p, cogClickedCB, LV_EVENT_CLICKED, p);

  // The settings (cog) button's image
  lv_obj_t *cogImg = lv_img_create(p);
  lv_img_set_src(cogImg, &cog);
  lv_obj_add_style(cogImg, &styles.icon, LV_PART_MAIN);
  lv_obj_center(cogImg);

  lv_obj_update_layout(timeUI.screen);

  lv_scr_load(timeUI.screen);

  lv_timer_create(secondsCB, 1000, NULL);
}


static void setupPasswordUI(void) {
  (void) &passwordUI;		/* Don't warn me */
}


static void setupWifiUI(void) {
  (void) &wifiUI;		/* Don't warn me */
}


#if 0
static void networkScanner(void) {
}
#endif


// Define each style by initializing its static variable as a new
// style and calling each of the SETTINGS style setting functions with
// their respective parameters.
static void setupStyles(void) {

#define INIT(STYLE)			lv_style_init(&styles.STYLE)
#define SET(STYLE,ATTR,PARMS...)	lv_style_set_##ATTR(&styles.STYLE, PARMS)

  INIT(base);
  SET(base, bg_color, lv_color_hex(0x222244));
  SET(base, text_color, lv_color_hex(0xAAAAAA));

  INIT(icon);
  SET(icon, border_color, lv_color_hex(0x5555AA));
  SET(icon, border_width, 2);
  SET(icon, pad_all, 16);

  INIT(widget);
  SET(widget, text_font, &lv_font_montserrat_24);

  INIT(pageHeader);
  SET(pageHeader, bg_color, lv_color_hex(0xFCFCAA));
  SET(pageHeader, bg_opa, LV_OPA_COVER);
  SET(pageHeader, text_color, lv_color_black());
  SET(pageHeader, text_font, &RobotoMedium40);

#define STYLES								\
  DO1(statusStyle,							\
      SET1(text_font, &lv_font_montserrat_10))				\
  DO1(buttonStyle,							\
      SET1(border_color, lv_palette_main(LV_PALETTE_GREY)))		\
  DO1(okButtonStyle,							\
      SET1(border_color, lv_palette_main(LV_PALETTE_GREEN)))		\
  DO1(cancelButtonStyle,						\
      SET1(border_color, lv_palette_main(LV_PALETTE_ORANGE)))		\
  DO1(popupStyle,							\
      SET1(bg_color, lv_palette_main(LV_PALETTE_ORANGE)))

// First, declare the static variables for our styles
#define DO1(S, SETTINGS...)	static lv_style_t S;
    STYLES
#undef DO1


#define SET1(F, ARGS...)	lv_style_set_ ## F(s, ARGS);

#define DO1(S, SETTINGS...)			\
  {						\
    lv_style_t *s = &S;				\
    lv_style_init(s);				\
    SETTINGS					\
  }

  STYLES
#undef SET1
#undef DO1

#undef SET
#undef INIT
}


static void setupKeyboard(void) {
  keyboard = lv_keyboard_create(lv_scr_act());
  lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}


static lv_obj_t *makeTextBox(lv_obj_t *parent, const char *prompt, int taWidth) {
  lv_obj_t *group = lv_obj_create(parent);
  lv_obj_set_size(group, taWidth * 2, LV_SIZE_CONTENT);
  lv_obj_set_layout(group, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(group, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(group, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
  lv_obj_t *label = lv_label_create(group);
  lv_label_set_text_static(label, prompt);
  lv_obj_t *ta = lv_textarea_create(group);
  lv_textarea_set_text(ta, "");
  lv_textarea_set_one_line(ta, true);
  lv_obj_set_width(ta, taWidth);
  return group;
}


static lv_obj_t *makeLabelButton(lv_obj_t *parent, const char *labelText) {
  lv_obj_t *buttonP = lv_btn_create(parent);
  lv_obj_t *labelP = lv_label_create(buttonP);
  lv_label_set_text(labelP, labelText);
  return buttonP;
}


static void settingsDismiss(void) {
  lv_screen_load(timeUI.screen);
}


static int getChecked(lv_obj_t *checkbox) {
  return lv_obj_get_state(checkbox);
}


static char *setMallocString(char **stringPP, const char *valueP) {

  if (*stringPP) {
    free(*stringPP);
    *stringPP = NULL;
  }

  int len = strlen(valueP) + 1;
  *stringPP = strdup(valueP);

  if (!*stringPP) ESP_LOGE(TAG, "Failed to allocate %d bytes in strdup()!", len);

  return *stringPP;
}


static void settingsHandleOK(void) {
  settings.twelveHr = getChecked(settingsUI.twelveHr);
  settings.showSeconds = getChecked(settingsUI.showSeconds);
  settings.showDayDate = getChecked(settingsUI.showDayDate);
  setMallocString(&settings.tz, lv_textarea_get_text(settingsUI.tz));

  for (int k = 0; k < 3; ++k) {
    setMallocString(&settings.ntp[k], lv_textarea_get_text(settingsUI.ntp[k]));
  }
}


static void dialogButtonClickedCB(lv_event_t *ev) {
  lv_obj_t *p = lv_event_get_user_data(ev);

  if (p == settingsUI.ok) {
    settingsHandleOK();
    settingsDismiss();
  } else if (p == settingsUI.cancel) {
    settingsDismiss();
  }
}


static void setChecked(lv_obj_t *checkbox, int value) {

  if (value)
    lv_obj_add_state(checkbox, LV_STATE_CHECKED);
  else
    lv_obj_remove_state(checkbox, LV_STATE_CHECKED);
}


static void setupSettingsUI(void) {
  lv_obj_t *p;

  settingsUI.screen = lv_obj_create(NULL);
  lv_obj_add_style(settingsUI.screen, &styles.base, LV_PART_MAIN);

  p = lv_label_create(settingsUI.screen);
  lv_label_set_text(p, "Settings");
  lv_obj_add_style(p, &styles.pageHeader, LV_PART_MAIN);
  lv_obj_align(p, LV_ALIGN_TOP_MID, 0, 25);
  lv_obj_set_width(p, lv_pct(90));

  p = settingsUI.twelveHr = lv_checkbox_create(settingsUI.screen);
  lv_checkbox_set_text(p, "12-hour time format");
  lv_obj_align(p, LV_ALIGN_TOP_LEFT, 80, 75);
  setChecked(p, settings.twelveHr);

  p = settingsUI.showSeconds = lv_checkbox_create(settingsUI.screen);
  lv_checkbox_set_text(p, "Show seconds");
  lv_obj_align(p, LV_ALIGN_BOTTOM_LEFT, 0, 10);
  lv_obj_align_to(p, settingsUI.twelveHr, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
  setChecked(p, settings.showSeconds);

  p = settingsUI.showDayDate = lv_checkbox_create(settingsUI.screen);
  lv_checkbox_set_text(p, "Show day/date");
  lv_obj_align(p, LV_ALIGN_BOTTOM_LEFT, 0, 10);
  lv_obj_align_to(p, settingsUI.showSeconds, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
  setChecked(p, settings.showDayDate);

  static const int32_t tbRowHeight = 35;
  static const int32_t ntpGridCols[] = {75, 250, LV_GRID_TEMPLATE_LAST};
  static const int32_t ntpGridRows[] = {tbRowHeight, tbRowHeight, tbRowHeight, LV_GRID_TEMPLATE_LAST};
  settingsUI.ntpGrid = lv_obj_create(settingsUI.screen);
  lv_obj_set_grid_dsc_array(settingsUI.ntpGrid, ntpGridCols, ntpGridRows);
  lv_obj_set_layout(settingsUI.ntpGrid, LV_LAYOUT_GRID);
  lv_obj_set_size(settingsUI.ntpGrid, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(settingsUI.ntpGrid, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_align_to(settingsUI.ntpGrid, settingsUI.twelveHr, LV_ALIGN_TOP_LEFT, 275, 0);
  lv_obj_add_style(settingsUI.ntpGrid, &styles.base, 0);

  for (int k = 0; k < 3; ++k) {
    lv_obj_t *label = lv_label_create(settingsUI.ntpGrid);
    lv_label_set_text_fmt(label, "NTP #%d", k);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_END, 0, 1, LV_GRID_ALIGN_CENTER, k, 1);
    lv_obj_t *ta = lv_textarea_create(settingsUI.ntpGrid);
    lv_textarea_set_text(ta, settings.ntp[k]);
    lv_textarea_set_one_line(ta, true);
    lv_obj_set_width(ta, 220);
    lv_obj_set_grid_cell(ta, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, k, 1);
    settingsUI.ntp[k] = ta;
  }

  lv_obj_update_layout(settingsUI.ntpGrid);

  static const int32_t tzGridCols[] = {75, 450, LV_GRID_TEMPLATE_LAST};
  static const int32_t tzGridRows[] = {tbRowHeight, LV_GRID_TEMPLATE_LAST};
  settingsUI.tzGrid = lv_obj_create(settingsUI.screen);
  lv_obj_set_grid_dsc_array(settingsUI.tzGrid, tzGridCols, tzGridRows);
  lv_obj_set_layout(settingsUI.tzGrid, LV_LAYOUT_GRID);
  lv_obj_set_size(settingsUI.tzGrid, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(settingsUI.tzGrid, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_align_to(settingsUI.tzGrid, settingsUI.ntpGrid, LV_ALIGN_BOTTOM_LEFT, -250, 100);
  lv_obj_add_style(settingsUI.tzGrid, &styles.base, 0);

  lv_obj_t *label = lv_label_create(settingsUI.tzGrid);
  lv_label_set_text_static(label, "TZ string");
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_END, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
  lv_obj_t *ta = lv_textarea_create(settingsUI.tzGrid);
  lv_textarea_set_text(ta, settings.tz);
  lv_textarea_set_one_line(ta, true);
  lv_obj_set_width(ta, 420);
  lv_obj_set_grid_cell(ta, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
  settingsUI.tz = ta;

  lv_obj_update_layout(settingsUI.tzGrid);



  /*
  p = settingsUI.ntp[0] = makeTextBox(settingsUI.screen, "NTP #1:", 220);
  lv_obj_align(p, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_align_to(p, settingsUI.twelveHr, LV_ALIGN_BOTTOM_LEFT, 200, 0);

  p = settingsUI.ntp[1] = makeTextBox(settingsUI.screen, "NTP #2:", 220);
  lv_obj_align(p, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_align_to(p, settingsUI.ntp[0], LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

  p = settingsUI.ntp[2] = makeTextBox(settingsUI.screen, "NTP #3:", 220);
  lv_obj_align(p, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_align_to(p, settingsUI.ntp[1], LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
  */

  // Buttons are in a horizontal row aligned a little above the bottom
  // of the screen with their edges (left for OK, right for Cancel)
  // the same distance from the center of the screen in opposite
  // directions.
  p = settingsUI.ok = makeLabelButton(settingsUI.screen, "OK");
  lv_obj_set_width(p, 150);
  lv_obj_align_to(p, settingsUI.screen, LV_ALIGN_OUT_BOTTOM_MID, -100, -75);
  lv_obj_add_event_cb(p, dialogButtonClickedCB, LV_EVENT_CLICKED, p);

  p = settingsUI.cancel = makeLabelButton(settingsUI.screen, "Cancel");
  lv_obj_set_width(p, 150);
  lv_obj_align_to(p, settingsUI.screen, LV_ALIGN_OUT_BOTTOM_MID, 100, -75);
  lv_obj_add_event_cb(p, dialogButtonClickedCB, LV_EVENT_CLICKED, p);

  lv_obj_update_layout(settingsUI.screen);
}


#if 0
static void settingsButtonEventCB(lv_event_t *e) {

  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    lv_obj_clear_flag(settingsUI.screen, LV_OBJ_FLAG_HIDDEN);
  }
}
#endif


#if 0
static void settingsCloseButtonEventCB(lv_event_t *e) {

  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    lv_obj_add_flag(settingsUI.screen, LV_OBJ_FLAG_HIDDEN);
  }
}
#endif


#if 0
static void mboxConnectButtonEventCB(lv_event_t *e) {
  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    ssidPW = String(lv_textarea_get_text(mboxPassword));
    networkConnector();
    lv_obj_move_background(mboxConnect);
    popupMsgBox("Connecting!", "Attempting to connect to the selected network.");
  }
}
#endif


#if 0
static void buttonEventCB(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = lv_event_get_target(e);

  if (code == LV_EVENT_CLICKED) {
    if (btn == mboxConnectBtn) {
      ssidPW = String(lv_textarea_get_text(mboxPassword));

      networkConnector();
      lv_obj_move_background(mboxConnect);
      popupMsgBox("Connecting!", "Attempting to connect to the selected network.");
    } else if (btn == mboxCloseBtn) {
      lv_obj_move_background(mboxConnect);
    } else if (btn == popupBoxCloseBtn) {
      lv_obj_move_background(popupBox);
    }

  } else if (code == LV_EVENT_VALUE_CHANGED) {

    if (ntScanTaskHandler == NULL) {
      networkStatus = NETWORK_SEARCHING;
      networkScanner();
      timer = lv_timer_create(timerForNetwork, 1000, wfList);
      lv_list_add_text(wfList, "WiFi: Looking for Networks...");
    }
  }
}
#endif


void SetupWallclockUI(void) {
  setupStyles();
  setupClockUI();
  setupKeyboard();
  setupPasswordUI();
  setupWifiUI();
  setupSettingsUI();
}


#if LV_PC_SIM
/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{
  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);

  lv_indev_t * keyboard = lv_sdl_keyboard_create();
  lv_indev_set_display(keyboard, disp);
  lv_indev_set_group(keyboard, lv_group_get_default());

  return disp;
}
#endif


#if LV_PC_SIM
int main(int argc, char *argv[]) {
  setlinebuf(stderr);
  lv_init();
  hal_init(800, 480);

  SetupWallclockUI();

  /*To hide the memory and performance indicators in the corners
   *disable `LV_USE_MEM_MONITOR` and `LV_USE_PERF_MONITOR` in `lv_conf.h`*/

  while(1) {
      /* Periodically call the lv_task handler.
       * It could be done in a timer interrupt or an OS task too.*/
      lv_timer_handler();
      usleep(10* 1000);
  }
}
#endif
