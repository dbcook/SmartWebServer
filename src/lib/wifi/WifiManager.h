// wifi manager, used by the webserver and wifi serial IP
#pragma once

#include "../../Common.h"

#ifndef OPERATIONAL_MODE
#define OPERATIONAL_MODE OFF
#endif

#if OPERATIONAL_MODE == WIFI

#include "WifiManager.defaults.h"

#if defined(ESP32)
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WiFiAP.h>
  #if MDNS_SERVER == ON
    #include <ESPmDNS.h>
  #endif
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WiFiAP.h>
  #if MDNS_SERVER == ON
    #include <ESP8266mDNS.h>
  #endif
#else
  #error "Configuration (Config.h): No Wifi support is present for this device"
#endif

typedef struct AccessPointSettings {
  char ssid[32];   // BUG: 802.11 says SSID may be up to 32 octets.  This code uses null terminated strings and thus only allows 31 octet string length.
  char pwd[32];    // BUG: spec noncompliant.  WEP allows max 16 chars, WPAx-PSK allows 63 ascii encoded chars.
  unsigned char channel;
  uint8_t ip[4];
  uint8_t gw[4];
  uint8_t sn[4];
} AccessPointSettings;

typedef struct StationSettings {
  char host[32];
  char ssid[32];   // *** 802.11 noncompliant. Length needs to be 33 to permit 32 octet SSID as null-term string.
  char pwd[32];    // *** 802.11 noncompliant.  WPA-PSK and WPA2-PSK allow up to 63 ascii-encoded chars.
  bool dhcpEnabled;
  uint8_t target[4];
  uint8_t ip[4];
  uint8_t gw[4];
  uint8_t sn[4];
} StationSettings;

#define WifiStationCount 3
#define WifiSettingsSize 451    // *** hardcoded struct size, but only used as the expected value for a check to see if sizeof(WifiSettings) has grown.
typedef struct WifiSettings {
  char masterPassword[32];      // *** should be longer?

  bool accessPointEnabled;
  AccessPointSettings ap;

  bool stationEnabled;
  bool stationApFallback;
  StationSettings station[WifiStationCount];

} WifiSettings;

class WifiManager {
  public:
    bool init();
    void disconnect();
    #if STA_AUTO_RECONNECT == true
      void reconnectStation();
    #endif
    void setStation(int number);
    void writeSettings();

    StationSettings *sta;

    WifiSettings settings = {
      PASSWORD_DEFAULT,

      AP_ENABLED,
      {
        AP_SSID, AP_PASSWORD, AP_CHANNEL,
        AP_IP_ADDR, AP_GW_ADDR, AP_SN_MASK
      },

      STA_ENABLED,
      STA_AP_FALLBACK,

      {
        {
          STA1_HOST_NAME, STA1_SSID, STA1_PASSWORD, STA1_DHCP_ENABLED,
          STA1_TARGET_IP_ADDR, STA1_IP_ADDR, STA1_GW_ADDR, STA1_SN_MASK
        },

        {
          STA2_HOST_NAME, STA2_SSID, STA2_PASSWORD, STA2_DHCP_ENABLED,
          STA2_TARGET_IP_ADDR, STA2_IP_ADDR, STA2_GW_ADDR, STA2_SN_MASK
        },

        {
          STA3_HOST_NAME, STA3_SSID, STA3_PASSWORD, STA3_DHCP_ENABLED,
          STA3_TARGET_IP_ADDR, STA3_IP_ADDR, STA3_GW_ADDR, STA3_SN_MASK
        }

      }
    };

    bool active = false;
    int stationNumber = 1;

  private:
    
};

extern WifiManager wifiManager;

#endif
