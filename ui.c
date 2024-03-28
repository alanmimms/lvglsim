#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "lvgl.h"
#include "settings.h"

static const char *TAG = "wallclockUI";

static const int SCREENW = 800;
static const int SCREENH = 480;


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

static lv_disp_t *disp;
static lv_obj_t *keyboard;
extern lv_font_t LoraBold240;
extern lv_font_t RobotoMedium40;


#define HEX_TO_LV_COLOR(H) {			\
    .red = 0xFF & ((uint32_t) (H) >> 16),	\
    .green = 0xFF & ((uint32_t) (H) >> 8),	\
    .blue = 0xFF & (uint32_t) (H)		\
}

static const lv_style_const_prop_t baseStyleProps[] = {
  LV_STYLE_CONST_BG_COLOR(HEX_TO_LV_COLOR(0x222244)),
  LV_STYLE_CONST_TEXT_COLOR(HEX_TO_LV_COLOR(0xAAAAAA)),
};
LV_STYLE_CONST_INIT(baseStyle, baseStyleProps);

static const lv_style_const_prop_t iconStyleProps[] = {
  LV_STYLE_CONST_BORDER_COLOR(HEX_TO_LV_COLOR(0x555588)),
  LV_STYLE_CONST_BORDER_WIDTH(2),
};
LV_STYLE_CONST_INIT(iconStyle, iconStyleProps);

static const lv_style_const_prop_t pageHeaderStyleProps[] = {
  LV_STYLE_CONST_BG_OPA(LV_OPA_COVER),
  LV_STYLE_CONST_BG_COLOR(HEX_TO_LV_COLOR(0xFCFCAA)),
  LV_STYLE_CONST_BORDER_COLOR(HEX_TO_LV_COLOR(0xFCFCAA)),
  LV_STYLE_CONST_TEXT_COLOR(HEX_TO_LV_COLOR(0x000000)),
  LV_STYLE_CONST_TEXT_FONT(&RobotoMedium40),
};
LV_STYLE_CONST_INIT(pageHeaderStyle, pageHeaderStyleProps);

static const lv_style_const_prop_t buttonStyleProps[] = {
  LV_STYLE_CONST_TEXT_ALIGN(LV_TEXT_ALIGN_CENTER),
};
LV_STYLE_CONST_INIT(buttonStyle, buttonStyleProps);


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
  lv_obj_t *screen;		/* Screen to load to show dialog */
  lv_obj_t *grid;		/* Grid to which we align all the stuff */
  lv_obj_t *pageHeader;		/* Heading at top of dialog */
  lv_obj_t *twelveHr;		/* Checkbox */
  lv_obj_t *showSeconds;	/* Checkbox */
  lv_obj_t *showDayDate;	/* Checkbox */
  lv_obj_t *ntp[N_NTP_SERVERS];	/* Each is a textarea */
  lv_obj_t *tz;			/* Timezone in TZ envar format (e.g., PST-08 for US Pacific) */
  lv_obj_t *wifi;		/* Table of WiFi access points */
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
static const Settings initialSettings = {
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

// When there are no NVS based settings values, this has to be
// initialized by ReadSettings() to use malloc() based strings.
Settings settings;
extern void ReadSettings(void);


// Platform API hack so I can test UI on a PC.
#if LV_PC_SIM
#include <stdio.h>
#define ESP_LOGI(TAG, FMT, VA...) fprintf(stderr, "[Info] %25s: " FMT "\n", TAG, ##VA)
#define ESP_LOGW(TAG, FMT, VA...) fprintf(stderr, "[Warn] %25s: " FMT "\n", TAG, ##VA)
#define ESP_LOGE(TAG, FMT, VA...) fprintf(stderr, "[Err!] %25s: " FMT "\n", TAG, ##VA)
#endif


static inline int max(const int a, const int b) {
  return a > b ? a : b;
}


static inline int min(const int a, const int b) {
  return a < b ? a : b;
}


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
  lv_obj_add_style(p, &baseStyle, LV_PART_MAIN);

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
  lv_obj_add_style(cogImg, &iconStyle, LV_PART_MAIN);
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
  lv_obj_t *button = lv_btn_create(parent);
  lv_obj_t *label = lv_label_create(button);
  lv_label_set_text(label, labelText);
  lv_obj_center(label);
  return button;
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

  uint32_t wifiRow, wifiCol;
  lv_table_get_selected_cell(settingsUI.wifi, &wifiRow, &wifiCol);

  if (wifiRow != LV_TABLE_CELL_NONE) {
    const char *ssid = lv_table_get_cell_value(settingsUI.wifi, wifiRow, wifiCol);
    setMallocString(&settings.ssid, ssid);
  }

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
  int32_t row;

  settingsUI.screen = lv_obj_create(NULL);
  lv_obj_add_style(settingsUI.screen, &baseStyle, LV_PART_MAIN);

  static const int32_t tbRowHeight = 35;
  static const int32_t tbRowWidth = 200;
  static const int32_t tbLabelWidth = 75;

  static const int32_t gridCols[] = {
    0.8 * SCREENW / 2,		/* checkboxes and WiFi list */
    tbLabelWidth,		/* text box labels  */
    0.8 * SCREENW / 2 - tbLabelWidth, /* textentry areas */
    LV_GRID_TEMPLATE_LAST};
  static const int32_t gridRows[] = {
    50,				/* Page heading */
    tbRowHeight,		/* 12hr		|  NTP 0 */
    tbRowHeight,		/* show seconds |  NTP 1 */
    tbRowHeight,		/* show day/date|  NTP 2 */
    tbRowHeight,		/* start of wifi|  TZ string */
    tbRowHeight,		/* more wifi    |  <empty> */
    tbRowHeight,		/* more wifi    |  <empty> */
    50,				/* OK/Cancel buttons */
    LV_GRID_TEMPLATE_LAST};
  p = settingsUI.grid = lv_obj_create(settingsUI.screen);
  lv_obj_set_grid_dsc_array(p, gridCols, gridRows);
  lv_obj_set_layout(p, LV_LAYOUT_GRID);
  lv_obj_set_size(p, lv_pct(95), lv_pct(95));
  lv_obj_set_style_bg_color(p, lv_color_hex(0x555577), LV_PART_MAIN);
  lv_obj_center(p);

  // Dialog title
  p = settingsUI.pageHeader = lv_label_create(settingsUI.grid);
  lv_label_set_text(p, "Settings");
  lv_obj_set_width(p, lv_pct(100));
  lv_obj_set_grid_cell(p, LV_GRID_ALIGN_CENTER, 0, 3, LV_GRID_ALIGN_START, 0, 1);
  lv_obj_set_style_text_align(p, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_add_style(p, &pageHeaderStyle, LV_PART_MAIN);

  // Checkboxes
  row = 1;
  static const int32_t marginBelowHeading = 20;
  p = settingsUI.twelveHr = lv_checkbox_create(settingsUI.grid);
  lv_checkbox_set_text(p, "12-hour time format");
  lv_obj_set_grid_cell(p, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row++, 1);
  setChecked(p, settings.twelveHr);

  p = settingsUI.showSeconds = lv_checkbox_create(settingsUI.grid);
  lv_checkbox_set_text(p, "Show seconds");
  lv_obj_set_grid_cell(p, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row++, 1);
  setChecked(p, settings.showSeconds);

  p = settingsUI.showDayDate = lv_checkbox_create(settingsUI.grid);
  lv_checkbox_set_text(p, "Show day/date");
  lv_obj_set_grid_cell(p, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row++, 1);
  setChecked(p, settings.showDayDate);

  // WIFI selector
  p = settingsUI.wifi = lv_table_create(settingsUI.grid);
  lv_table_set_column_width(p, 0, lv_pct(45));
  lv_obj_set_style_pad_left(p, 0, LV_PART_ITEMS);
  lv_obj_set_style_pad_top(p, 0, LV_PART_ITEMS);
  lv_obj_set_style_pad_bottom(p, 0, LV_PART_ITEMS);
  lv_obj_set_grid_cell(p, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, row, 3);
  lv_obj_set_style_bg_color(p, lv_color_hex(0xaaaaaa), LV_PART_ITEMS);
  lv_obj_set_size(p, SCREENW * 0.4, SCREENH * 0.3);

  for (int k=0; k < 50; ++k) {
    lv_table_set_cell_value_fmt(p, k, 0, "WiFi %03d", k);
  }

  // NTP servers
  row = 1;
  for (int k = 0; k < 3; ++k, ++row) {
    lv_obj_t *label = lv_label_create(settingsUI.grid);
    lv_label_set_text_fmt(label, "NTP #%d", k);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, row, 1);
    lv_obj_t *ta = lv_textarea_create(settingsUI.grid);
    lv_textarea_set_text(ta, settings.ntp[k]);
    lv_textarea_set_one_line(ta, true);
    lv_obj_set_grid_cell(ta, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, row, 1);
    settingsUI.ntp[k] = ta;
  }

#if 0
  // Timezone string
  static const int32_t tzGridCols[] = {tbLabelWidth, tbRowWidth, LV_GRID_TEMPLATE_LAST};
  static const int32_t tzGridRows[] = {tbRowHeight, LV_GRID_TEMPLATE_LAST};
  settingsUI.tzGrid = lv_obj_create(settingsUI.grid);
  lv_obj_set_grid_dsc_array(settingsUI.tzGrid, tzGridCols, tzGridRows);
  lv_obj_set_layout(settingsUI.tzGrid, LV_LAYOUT_GRID);
  lv_obj_set_size(settingsUI.tzGrid, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(settingsUI.tzGrid, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_align_to(settingsUI.tzGrid, settingsUI.grid, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
  lv_obj_add_style(settingsUI.tzGrid, &baseStyle, 0);
  lv_obj_update_layout(settingsUI.grid);

  lv_obj_t *label = lv_label_create(settingsUI.tzGrid);
  lv_label_set_text_static(label, "TZ string");
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_END, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
  lv_obj_t *ta = lv_textarea_create(settingsUI.tzGrid);
  lv_textarea_set_text(ta, settings.tz);
  lv_textarea_set_one_line(ta, true);
  lv_obj_set_width(ta, 250);
  lv_obj_set_grid_cell(ta, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
  settingsUI.tz = ta;

  lv_obj_update_layout(settingsUI.tzGrid);
#endif

  // above the bottom of the screen with their edges (left for OK,
  // right for Cancel) the same distance from the center of the screen
  // in opposite directions.

  static const char *buttonMap[] = {"OK", "Cancel", NULL};
  lv_obj_t *buttons = lv_buttonmatrix_create(settingsUI.screen);
  lv_buttonmatrix_set_map(buttons, buttonMap);
  lv_obj_add_event_cb(buttons, dialogButtonClickedCB, LV_EVENT_CLICKED, p);
  lv_obj_set_content_height(buttons, 25);
  lv_obj_set_style_bg_opa(buttons, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(buttons, 0, LV_PART_MAIN);
  lv_obj_add_style(buttons, &buttonStyle, 0);
  lv_obj_align_to(buttons, settingsUI.screen, LV_ALIGN_BOTTOM_MID, 0, -20);

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
  ReadSettings();
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
// This is used for mocking the NVS based settings when we're just
// testing the UI.
void ReadSettings(void) {
  settings = initialSettings;

  settings.tz = strdup(initialSettings.tz);
  for (int k=0; k < 3; ++k) settings.ntp[k] = strdup(initialSettings.ntp[k]);
  settings.ssid = strdup(initialSettings.ssid);
  settings.password = strdup(initialSettings.password);
}


int main(int argc, char *argv[]) {
  setlinebuf(stderr);
  lv_init();
  hal_init(SCREENW, SCREENH);

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
