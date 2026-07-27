#include "jellyfin_player.h"
#include "app_shared.h"
#include "gui.h"
#include "ui_select.h"
#include "arduino_secrets.h"
#include <Audio.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <algorithm>

extern Audio audio;
extern lv_obj_t *ui_ModeNetworkInfo;

#ifndef SECRET_JELLYFIN_SERVER
#define SECRET_JELLYFIN_SERVER ""
#endif
#ifndef SECRET_JELLYFIN_USERNAME
#define SECRET_JELLYFIN_USERNAME ""
#endif
#ifndef SECRET_JELLYFIN_PASSWORD
#define SECRET_JELLYFIN_PASSWORD ""
#endif
#ifndef SECRET_JELLYFIN_API_KEY
#define SECRET_JELLYFIN_API_KEY ""
#endif
#ifndef SECRET_JELLYFIN_USER_ID
#define SECRET_JELLYFIN_USER_ID ""
#endif

std::vector<JellyfinTrack> jellyfinTracks;
int currentJellyfinTrack = 0;
bool jellyfinReady = false;
String jellyfinBaseUrl;
String jellyfinToken;
String jellyfinUserId;

static String jellyfinUrlEncode(const String &value)
{
  String out;
  out.reserve(value.length() * 3);
  for (size_t i = 0; i < value.length(); i++)
  {
    char c = value[i];
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
    {
      out += c;
    }
    else
    {
      static const char *hex = "0123456789ABCDEF";
      out += '%';
      out += hex[(c >> 4) & 0x0F];
      out += hex[c & 0x0F];
    }
  }
  return out;
}

static String jellyfinAuthHeaderValue()
{
  return "MediaBrowser Client=\"WebRadio\", Device=\"ESP32-S3\", DeviceId=\"esp32s3_webradio\", Version=\"1.0.0\"";
}

static String jellyfinNormalizeBaseUrl(const char *base)
{
  String out = base ? String(base) : String();
  out.trim();
  while (out.endsWith("/"))
    out.remove(out.length() - 1);
  return out;
}

static bool jellyfinHttpGet(const String &url, String &response, int &statusCode, bool addToken)
{
  HTTPClient http;
  statusCode = -1;

  if (url.startsWith("https://"))
  {
    WiFiClientSecure client;
    client.setInsecure();
    if (!http.begin(client, url))
      return false;
  }
  else
  {
    WiFiClient client;
    if (!http.begin(client, url))
      return false;
  }

  http.addHeader("Accept", "application/json");
  http.addHeader("X-Emby-Authorization", jellyfinAuthHeaderValue());
  if (addToken && jellyfinToken.length() > 0)
    http.addHeader("X-Emby-Token", jellyfinToken);

  statusCode = http.GET();
  response = http.getString();
  http.end();
  return statusCode > 0;
}

static bool jellyfinHttpPostJson(const String &url, const String &body, String &response, int &statusCode)
{
  HTTPClient http;
  statusCode = -1;

  if (url.startsWith("https://"))
  {
    WiFiClientSecure client;
    client.setInsecure();
    if (!http.begin(client, url))
      return false;
  }
  else
  {
    WiFiClient client;
    if (!http.begin(client, url))
      return false;
  }

  http.addHeader("Accept", "application/json");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Emby-Authorization", jellyfinAuthHeaderValue());

  statusCode = http.POST((uint8_t *)body.c_str(), body.length());
  response = http.getString();
  http.end();
  return statusCode > 0;
}

static bool jellyfinResolveUserIdFromToken()
{
  if (jellyfinUserId.length() > 0)
    return true;

  String payload;
  int code = -1;
  if (!jellyfinHttpGet(jellyfinBaseUrl + "/Users/Me", payload, code, true) || (code < 200 || code >= 300))
    return false;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err)
    return false;

  const char *id = doc["Id"];
  if (!id || strlen(id) == 0)
    return false;

  jellyfinUserId = id;
  return true;
}

static bool jellyfinLogin()
{
  jellyfinReady = false;
  jellyfinTracks.clear();

  jellyfinBaseUrl = jellyfinNormalizeBaseUrl(SECRET_JELLYFIN_SERVER);
  jellyfinUserId = String(SECRET_JELLYFIN_USER_ID);
  jellyfinToken = "";

  if (jellyfinBaseUrl.length() == 0)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Set SECRET_JELLYFIN_SERVER");
    return false;
  }

  const String apiKey = String(SECRET_JELLYFIN_API_KEY);
  if (apiKey.length() > 0)
  {
    jellyfinToken = apiKey;
    if (!jellyfinResolveUserIdFromToken() && jellyfinUserId.length() == 0)
    {
      setModeInfoLabel(ui_ModeNetworkInfo, "Set SECRET_JELLYFIN_USER_ID for API key mode");
      return false;
    }
    return true;
  }

  const String username = String(SECRET_JELLYFIN_USERNAME);
  const String password = String(SECRET_JELLYFIN_PASSWORD);
  if (username.length() == 0 || password.length() == 0)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Set Jellyfin user/password or API key");
    return false;
  }

  JsonDocument bodyDoc;
  bodyDoc["Username"] = username;
  bodyDoc["Pw"] = password;
  String body;
  serializeJson(bodyDoc, body);

  String payload;
  int code = -1;
  if (!jellyfinHttpPostJson(jellyfinBaseUrl + "/Users/AuthenticateByName", body, payload, code) || (code < 200 || code >= 300))
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin login failed");
    return false;
  }

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin auth parse error");
    return false;
  }

  const char *token = doc["AccessToken"];
  const char *uid = doc["User"]["Id"];
  if (!token || !uid)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin auth missing token/user");
    return false;
  }

  jellyfinToken = token;
  jellyfinUserId = uid;
  return true;
}

static bool jellyfinLoadLibrary()
{
  if (jellyfinToken.length() == 0 || jellyfinBaseUrl.length() == 0)
    return false;

  jellyfinTracks.clear();
  const int pageSize = 100;
  int startIndex = 0;
  const int maxItems = 600;

  while ((int)jellyfinTracks.size() < maxItems)
  {
    String url;
    if (jellyfinUserId.length() > 0)
      url = jellyfinBaseUrl + "/Users/" + jellyfinUserId + "/Items";
    else
      url = jellyfinBaseUrl + "/Items";

    url += "?Recursive=true&IncludeItemTypes=Audio&SortBy=SortName&SortOrder=Ascending";
    url += "&StartIndex=" + String(startIndex) + "&Limit=" + String(pageSize);
    url += "&Fields=MediaSources";

    String payload;
    int code = -1;
    if (!jellyfinHttpGet(url, payload, code, true) || (code < 200 || code >= 300))
      return false;

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err)
      return false;

    JsonArray items = doc["Items"].as<JsonArray>();
    if (items.isNull() || items.size() == 0)
      break;

    for (JsonVariant item : items)
    {
      const char *id = item["Id"];
      const char *name = item["Name"];
      if (!id || !name)
        continue;

      JellyfinTrack track;
      track.id = id;
      track.name = name;
      track.streamUrl = jellyfinBaseUrl + "/Audio/" + track.id + "/stream?static=true&api_key=" + jellyfinUrlEncode(jellyfinToken);
      if (jellyfinUserId.length() > 0)
        track.streamUrl += "&UserId=" + jellyfinUrlEncode(jellyfinUserId);
      jellyfinTracks.push_back(track);

      if ((int)jellyfinTracks.size() >= maxItems)
        break;
    }

    if ((int)items.size() < pageSize)
      break;

    startIndex += pageSize;
  }

  std::sort(jellyfinTracks.begin(), jellyfinTracks.end(), [](const JellyfinTrack &a, const JellyfinTrack &b)
            { return a.name < b.name; });

  return !jellyfinTracks.empty();
}

bool ensureJellyfinSessionAndLibrary()
{
  if (jellyfinReady && !jellyfinTracks.empty())
    return true;

  setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin: login...");
  if (!jellyfinLogin())
    return false;

  setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin: loading library...");
  if (!jellyfinLoadLibrary())
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin: no tracks or browse failed");
    return false;
  }

  jellyfinReady = true;
  return true;
}

void playJellyfinTrack(int index)
{
  ensureModeScreensUi();
  if (!ensureJellyfinSessionAndLibrary())
    return;

  int total = (int)jellyfinTracks.size();
  if (total <= 0)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin library is empty");
    return;
  }

  if (index < 0)
    index = total - 1;
  if (index >= total)
    index = 0;

  currentJellyfinTrack = index;
  const JellyfinTrack &track = jellyfinTracks[currentJellyfinTrack];

  audio.stopSong();
  audio.connecttohost(track.streamUrl.c_str());
  setModeInfoLabel(ui_ModeNetworkInfo, track.name.c_str());
}
