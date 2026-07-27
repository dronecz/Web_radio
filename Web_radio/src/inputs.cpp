#include "inputs.h"
#include "app_shared.h"
#include "gui.h"
#include "ui_select.h"
#include "EncoderRead.h"
#include <Audio.h>
#include <algorithm>

extern Audio audio;
extern EncoderRead encoder;
extern PlayerMode playerMode;
extern uint8_t currentStation;
extern int buttonCount;
extern int buttonPins[];
extern bool currentStates[];
extern bool previousStates[];
extern unsigned long lastDebounceTime[];
extern unsigned long debounceDelay;
extern int activeButton;
extern int volumePin;
extern int samples;
extern unsigned long lastVolumeCheck;
extern unsigned long volumeCheckInterval;
extern int changeTreshold;
extern int lastSliderValue;
extern lv_obj_t *lvglButtons[];

extern void connectToStation(uint8_t station);
extern void playNextInCurrentMode();
extern void playPrevInCurrentMode();
extern void setPlayerMode(PlayerMode mode);

static bool encoderBtnPrevPressed = false;
static bool encoderLongPressHandled = false;
static unsigned long encoderPressStart = 0;
static const unsigned long encoderShortPressMinMs = 40;
static const unsigned long encoderLongPressMs = 700;
static int32_t encoderLastCounter = 0;

int read_potentiometer()
{
  int sum = 0;
  for (int i = 0; i < samples; i++)
  {
    sum += analogRead(volumePin);
    delay(2);
  }
  return sum / 10;
}

void readVolumeValue()
{
  unsigned long now = millis();
  if (now - lastVolumeCheck < volumeCheckInterval)
    return;

  lastVolumeCheck = now;

  int raw = read_potentiometer();
  uint8_t maxVolume = audio.getVolumeSteps();
  int volume = map(raw, 0, 4095, 0, maxVolume);

  if (abs(volume - lastSliderValue) >= changeTreshold)
  {
    ensureVolumeSliderVisibleOnActiveScreen();
    lv_obj_set_style_opa(ui_SldrVolume, LV_OPA_100, 0);
    lv_slider_set_value(ui_SldrVolume, volume, LV_ANIM_OFF);
    lastSliderValue = volume;
    audio.setVolume(volume);
    lv_obj_fade_out(ui_SldrVolume, 1000, 1000);
  }
}

void initButtons()
{
  for (int i = 0; i < buttonCount; i++)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
    currentStates[i] = digitalRead(buttonPins[i]);
    previousStates[i] = currentStates[i];
    lastDebounceTime[i] = 0;
  }
}

void processButtons()
{
  unsigned long currentTime = millis();

  for (int i = 0; i < buttonCount; i++)
  {
    int reading = digitalRead(buttonPins[i]);

    if (reading != previousStates[i])
    {
      lastDebounceTime[i] = currentTime;
    }

    if ((currentTime - lastDebounceTime[i]) > debounceDelay)
    {
      if (reading != currentStates[i])
      {
        currentStates[i] = reading;

        if (currentStates[i] == LOW)
        {
          activeButton = i;

          if (playerMode == MODE_SD_MP3)
          {
            lv_obj_t *musicButtons[buttonCount] = {ui_Button14, ui_Button9, ui_Button10, ui_Button11, ui_Button12};
            lv_obj_t *target = musicButtons[i];
            if (target)
              lv_obj_send_event(target, LV_EVENT_CLICKED, NULL);

            previousStates[i] = reading;
            continue;
          }

          if (playerMode != MODE_RADIO)
          {
            if (i == 0)
            {
              playPrevInCurrentMode();
            }
            else if (i == 1)
            {
              playNextInCurrentMode();
            }
            else if (i == 4)
            {
              setPlayerMode(MODE_RADIO);
            }
            previousStates[i] = reading;
            continue;
          }

          for (int j = 0; j < buttonCount; j++)
          {
            lv_obj_t *container = lv_obj_get_child(lvglButtons[j], 0);
            _ui_state_modify(container, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
          }

          if (lvglButtons[i] != nullptr)
          {
            lv_obj_send_event(lvglButtons[i], LV_EVENT_CLICKED, NULL);
          }
        }
      }
    }

    previousStates[i] = reading;
  }
}

void processEncoder()
{
  int32_t counter = encoder.getCounter();
  int32_t diff = counter - encoderLastCounter;
  if (diff != 0)
  {
    encoderLastCounter = counter;
    if (isModeMenuVisible())
    {
      if (diff > 0)
      {
        modeMenuSelection = (modeMenuSelection + 1) % 3;
      }
      else
      {
        modeMenuSelection = (modeMenuSelection + 2) % 3;
      }
      updateModeMenuSelectionVisual();
    }
  }

  bool pressed = encoder.encBtn();
  unsigned long now = millis();

  if (pressed && !encoderBtnPrevPressed)
  {
    encoderPressStart = now;
    encoderLongPressHandled = false;
  }

  if (pressed && !encoderLongPressHandled)
  {
    if ((now - encoderPressStart) >= encoderLongPressMs)
    {
      Serial.printf("[ENC] LONG press (%lu ms)\n", (unsigned long)(now - encoderPressStart));
      showModeMenu();
      encoderLongPressHandled = true;
    }
  }

  if (!pressed && encoderBtnPrevPressed)
  {
    unsigned long held = now - encoderPressStart;
    if (!encoderLongPressHandled && held >= encoderShortPressMinMs)
    {
      Serial.printf("[ENC] SHORT press (%lu ms)\n", (unsigned long)held);
      if (isModeMenuVisible())
      {
        lv_obj_t *btn = getModeMenuButtonBySelection(modeMenuSelection);
        if (btn)
          lv_obj_send_event(btn, LV_EVENT_CLICKED, NULL);
      }
    }
  }

  encoderBtnPrevPressed = pressed;
}

void btn_event_handler(lv_event_t *e)
{
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED)
  {
    Serial.print("Kliknuto na tlačítko: ");

    if (btn == ui_Button1)
    {
      Serial.println("Button1");
      _ui_state_modify(ui_Container1, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(0);
    }
    else if (btn == ui_Button2)
    {
      Serial.println("Button2");
      _ui_state_modify(ui_Container2, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(1);
    }
    else if (btn == ui_Button3)
    {
      Serial.println("Button3");
      _ui_state_modify(ui_Container3, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(2);
    }
    else if (btn == ui_Button4)
    {
      Serial.println("Button4");
      _ui_state_modify(ui_Container4, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(3);
    }
    else if (btn == ui_Button5)
    {
      Serial.println("Button5");
      _ui_state_modify(ui_Container5, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(4);
    }
  }
}

void mode_button_event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code != LV_EVENT_CLICKED)
    return;

  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  if (btn == ui_Button6)
  {
    setPlayerMode(MODE_RADIO);
  }
  else if (btn == ui_Button7)
  {
    setPlayerMode(MODE_SD_MP3);
  }
  else if (btn == ui_Button8)
  {
    setPlayerMode(MODE_JELLYFIN);
  }
}
