#pragma once

#include <Arduino.h>
#include <vector>

struct JellyfinTrack
{
  String id;
  String name;
  String streamUrl;
};

extern std::vector<JellyfinTrack> jellyfinTracks;
extern int currentJellyfinTrack;
extern bool jellyfinReady;
extern String jellyfinBaseUrl;
extern String jellyfinToken;
extern String jellyfinUserId;

bool ensureJellyfinSessionAndLibrary();
void playJellyfinTrack(int index);
