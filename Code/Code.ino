// ###########################################################################################
// #
// # Code for "Soap dish with hand wash timer" project: https://www.printables.com/de/model/212345-soap-dish-with-hand-wash-timer
// #
// # Code by https://github.com/AWSW-de 
// #
// # Released under license: GNU General Public License v3.0: https://github.com/AWSW-de/Soap-dish-with-hand-wash-timer/blob/main/LICENSE
// #
// ###########################################################################################

// ###########################################################################################
// #
// # Code version:
// #
// ###########################################################################################
// ###########################################################################################
#define CODE_VERSION "V1.1.1"
// ###########################################################################################
// ###########################################################################################
// Change program paramters here:
// ##############################
int defcount = 30;                  // Default timer countdown time = 30s
int distance = 25;                  // Default distance to start the timer countdown
int debugmode = 0;                  // Use serial output in debug mode = 1 only to save power
// ###########################################################################################
// ###########################################################################################
// 7 segment Display parameters:
#include <TM1637Display.h> // Library "TM1637" required --> https://github.com/avishorp/TM1637
#define CLK D3
#define DIO D2
TM1637Display display(CLK, DIO);
uint8_t data[] = {0, 0, 0, 0};
#define TEST_DELAY 1000   // Default sleep time = 1s for the countdown
// ###########################################################################################
// Ultra sonic sensor parameters:
const int trigPin = D6;
const int echoPin = D5;
long last_distance = 0;
// ###########################################################################################
void ledtimer() {
  display.setBrightness(7, true);  // Turn display on
  display.clear();
  delay(TEST_DELAY);
  for (int i = defcount; i >= 0; i--) {
    if (debugmode == 1) Serial.print("Countdown: ");
    if (debugmode == 1) Serial.println(i);
    data[3] = display.encodeDigit(i / 1 % 10);
    data[2] = display.encodeDigit(i / 10 % 10);
    data[1] = display.encodeDigit(i / 100 % 10);
    data[0] = display.encodeDigit(i / 1000 % 10);
    display.setSegments(data);
    delay(TEST_DELAY);
  }
  display.clear();
  display.setBrightness(7, false);  // Turn display off
}
// ###########################################################################################
void measureDistance()
{
  long duration = 0;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  last_distance = microsecondsToCentimeters(duration);
  if (debugmode == 1) Serial.print("Current distance: ");
  if (debugmode == 1) Serial.println(last_distance);
  static char buf[10] = { '\0' };
  static char prev[10] = { '\0' };
  if (last_distance < distance)
  {
    ledtimer();
  }
}
// ###########################################################################################
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  // For all others: The magic is happening here! ;-)
  return microseconds / 29 / 2;
}
// ###########################################################################################
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);               // Turn off build in LED of the D1 mini
  digitalWrite(LED_BUILTIN, HIGH);            // Turn off build in LED of the D1 mini
  if (debugmode == 1) Serial.begin(115200);
  if (debugmode == 1) delay(2000);
  if (debugmode == 1) Serial.print("AWSW WashTimer - ");
  if (debugmode == 1) Serial.println(CODE_VERSION);
  if (debugmode == 1) Serial.println("Measure distance now...");
  measureDistance();
  ESP.deepSleep(5000000); // 5 seconds of deep sleep time = 5000000
  if (debugmode == 1) Serial.println("This will never be printed... Sleeping now....");
}
// ###########################################################################################
void loop()
{
  // The loop function stays empty. Deep sleep is used and the setup hold the main code !!!
}
// ###########################################################################################
