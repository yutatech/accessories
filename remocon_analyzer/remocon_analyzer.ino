unsigned long startTime;
bool state = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  Serial.begin(9600);
  delay(100);
  while (digitalRead(2) == HIGH);

  startTime = micros();
}

int lastData = -1;
void loop() {
  // put your main code here, to run repeatedly:
  bool digitalValue = digitalRead(2);
  if (digitalValue != state) {
    unsigned long nowTime = micros();
        Serial.print(state);
//        Serial.print(", ");
//        Serial.println(nowTime - startTime);

    unsigned long diff = nowTime - startTime;

    const long cmdlen = 425;
    unsigned long length = (diff + cmdlen / 2) / cmdlen * cmdlen;

        Serial.print(", ");
        Serial.println(length);
    startTime = nowTime;
    state = digitalValue;
//    if (lastData == -1) {
//      lastData = length;
//    }
//    else {
//      if (lastData == 8 && length == 4) {
//        Serial.println("L");
//        lastData = -1;
//      }
//      else if (lastData == 1 && length == 1) {
//        Serial.println("0");
//        lastData = -1;
//      }
//      else if (lastData == 1 && length == 3) {
//        Serial.println("1");
//        lastData = -1;
//      }
//      else
//        lastData = length;
//    }
  }
}
