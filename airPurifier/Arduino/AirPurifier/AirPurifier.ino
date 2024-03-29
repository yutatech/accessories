/*
   Partition SchemeMinimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)
*/

#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>

#define STATE_LAMP_1_PIN 35
#define STATE_LAMP_2_PIN 32
#define STATE_LAMP_3_PIN 33

WiFiServer server(80);

TaskHandle_t thp[1];

class Button {
  public:
    Button(int _digitalPinNum) : digitalPinNum(_digitalPinNum) {}
    void Push() {
      counter = 0;
    }

    void loop() { // execute every 1ms
      if (counter < 50) {
        digitalWrite(digitalPinNum, HIGH);
        counter++;
      }
      else {
        digitalWrite(digitalPinNum, LOW);
      }
    }
  private:
    int digitalPinNum;
    int counter;
};

Button light_button(18);
Button power_button(19);

int cur_power = 0;
int tar_power = 0;

#define TIMEOUT 2000
hw_timer_t * timer;
void IRAM_ATTR resetModule() {
  Serial.println("Reboooooot!!");
  esp_restart();
}

void setup() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &resetModule, true);
  timerAlarmWrite(timer, TIMEOUT * 1000, false);
  timerAlarmEnable(timer);
  timerWrite(timer, 0);

  Serial.begin(115200);

  pinMode(32, INPUT); // state lamp 2
  pinMode(33, INPUT); // state lamp 3
  pinMode(34, INPUT); // light state
  pinMode(35, INPUT); // state lamp 1

  pinMode(18, OUTPUT); // light button
  pinMode(19, OUTPUT); // power button

  WiFi.begin();
  Serial.print("WiFi Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    timerWrite(timer, 0);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("AirPurifier")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  server.begin();
  ArduinoOTA.setHostname("AirPurifier");
  ArduinoOTA.begin();

  xTaskCreatePinnedToCore(Core0a, "Core0a", 4096, NULL, 3, &thp[0], 0);
}

String rep_ap_stat() {
  if (cur_power == 0)
    return "cur_ap_off\n";
  else if (cur_power == 1)
    return "cur_ap_low\n";
  else if (cur_power == 2)
    return "cur_ap_middle\n";
  else if (cur_power == 3)
    return "cur_ap_high\n";
}

String processCommand(const String cmd) {
  String reply = "";
  if (cmd.endsWith("tar_ap_off\n"))
    tar_power = 0;
  else if (cmd.endsWith("tar_ap_low\n"))
    tar_power = 1;
  else if (cmd.endsWith("tar_ap_middle\n"))
    tar_power = 2;
  else if (cmd.endsWith("tar_ap_high\n"))
    tar_power = 3;
  else if (cmd.endsWith("tar_light_on\n")) {
  }
  else if (cmd.endsWith("tar_light_off\n")) {
  }
  else if (cmd.endsWith("req_ap_stat\n"))
    reply = rep_ap_stat();
  else if (cmd.endsWith("req_light_stat\n")) {
  }
  return reply;
}

void loop() {
  ArduinoOTA.handle();
  WiFiClient client = server.available();

  delay(1);

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      delay(1);
      while (client.available()) {
        delay(1);
        char c = client.read();
        currentLine += c;
        if (c == '\n') {
          Serial.println(currentLine);
          String reply = processCommand(currentLine);
          if (reply.length() > 0) {
            Serial.println(reply);
            client.print(reply);
          }
          currentLine = "";
        }
      }
    }
    client.stop();
  }

  if (WiFi.status() != WL_CONNECTED) {
    bool power3flag = false;
    if (cur_power == 3) {
      tar_power = 0;
      power3flag = true;
      timerWrite(timer, 0);
      delay(1000);
      timerWrite(timer, 0);
      delay(1000);
    }
    WiFi.disconnect();
    WiFi.reconnect();
    Serial.print("WiFi Reconnecting");
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED && counter++ < 30) {
      delay(500);
      timerWrite(timer, 0);
      Serial.print(".");
    }
    if (counter < 30) {
      Serial.println("WiFi Reconnected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }
    else {
      Serial.println("WiFi Reconnection Failed");
    }

    if (power3flag) {
      tar_power = 3;
    }
  }
}

int state_lamp_1_value;
int prv_power = 0;
#define PUSH_INTERVAL 200
unsigned long push_interval_counter = PUSH_INTERVAL;
void Core0a(void *args) {
  while (1) {
    timerWrite(timer, 0);
    /* read power indicator */
    const float rate = 0.01;
    state_lamp_1_value = state_lamp_1_value * (1 - rate) + analogRead(STATE_LAMP_1_PIN) * rate;

    if (analogRead(STATE_LAMP_3_PIN) > 1000)
      cur_power = 3;
    else if (analogRead(STATE_LAMP_2_PIN) > 1000)
      cur_power = 2;
    else if (state_lamp_1_value > 10) {
      if (prv_power == 3) { // 直前が3なら今回は0であるはず。閾値を超えているのはローパスフィルタが原因
        state_lamp_1_value = 0;
        cur_power = 0;
      }
      else
        cur_power = 1;
    }
    else
      cur_power = 0;

    prv_power = cur_power;
    /* read power indicator */

    /* change state */
    if (push_interval_counter > PUSH_INTERVAL) {
      if (tar_power != cur_power) {
        power_button.Push();
        push_interval_counter = 0;
      }
    }
    else {
      push_interval_counter++;
    }
    /* change state */

    light_button.loop();
    power_button.loop();
    delay(1);
  }
}
