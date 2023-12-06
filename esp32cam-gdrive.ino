#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
#include "Base64.h"
#include <esp_camera.h>
#include "MyCamera.h"
#include "sleep_time.h"

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */

time_t time_to_sleep;       /* Time ESP32 will go to sleep (in seconds) */
const int ledPin = 4;  // GPIO pin connected to the onboard LED
const int brightness = 50;  // Desired brightness level (0-255)

// RTC_DATA_ATTR int bootCount = 0;

const char* ssid     = "xxxxxxxx";   //your network SSID
const char* password = "xxxxxxxx";   //your network password

My_camera my_camera;
Sleep_Time sleep_t;

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);
  delay(10);
  
  /* Initialisation */
  Serial.println(">>>>>Waking up to execute task>>>>>");
  print_wakeup_reason(); //Print the wakeup reason for ESP32
  initWiFi();
  sleep_t.init();
  my_camera.init();

  /* Turn on LED, capture an image and upload to google drive */
  pinMode(ledPin, OUTPUT);
  // Turn on the LED with the specified brightness
  analogWrite(ledPin, brightness);
  // delay(2000);
  my_camera.saveCapturedImage();
  // analogWrite(ledPin, 0);

  /* Get the epoch time difference between the 
  10th of the next month at 12:00 and the current time 
  */

  time_t currentTime = sleep_t.getTime();
  //In case fetching current time fails, try again until we get it.
  if (currentTime==0){
    Serial.print("Fetch time online failed, trying again");
    while (currentTime==0){
      currentTime = sleep_t.getTime();
      Serial.print('.');
    }
    Serial.println();
  }
  // time_t currentTime = 1702209640; // test edge cases around Dec
  Serial.print("Current Time: ");
  Serial.println(String(currentTime)+"  Standar Format: "+sleep_t.convertEpochToStandardTime(currentTime));
  time_t targetTime = sleep_t.getNextMonthTime(currentTime, 10, 12);
  Serial.print("Target Time: ");
  Serial.println(String(targetTime)+"  Standar Format: "+sleep_t.convertEpochToStandardTime(targetTime));
  time_t timeDifference = targetTime - currentTime;
  // Print the epoch & standard time difference
  Serial.println("Target Time - Current Time: "+String(timeDifference)+"  Standard Format: "+sleep_t.convertEpochDiffToStandardTime(timeDifference));


  /*
  Finally configure the wake up source
  We set our ESP32 to wake up based on the timeDifference
  */
  time_to_sleep = timeDifference;
  esp_sleep_enable_timer_wakeup(time_to_sleep * uS_TO_S_FACTOR);
  Serial.println("ESP32-CAM next scheduled wake up date & time: " + sleep_t.convertEpochToStandardTime(targetTime));
  /* Optionally decide what peripherals to shut down/keep on */
  Serial.println("*****Going to sleep now*****");
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

// boolean enviar = true;

void loop() {
  //if(enviar) {
    // saveCapturedImage();
    // enviar = false;
    // delay(60000);
  //}
}

void initWiFi(){
  WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("STAIP address: ");
  Serial.println(WiFi.localIP());
    
  Serial.println("");
}
/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
