#ifndef __SETTINGS_H__
#define __SETTINGS_H__

typedef struct Settings {
  uint8_t twelveHr;	  /* Set if 12hr is needed (24hr otherwise) */
  uint8_t showSeconds;	  /* Display and update seconds */
  uint8_t showDayDate;	  /* Display day of week and date */
  char *tz;		  /* String for TZ envar */
  char *ntp[3];		  /* String for NTP sites */
  char *ssid;		  /* String for current selected WiFi SSID */
  char *password;	  /* String for current selected Wifi SSID's password */
} Settings;


extern Settings settings;

#endif // __SETTINGS_H__
