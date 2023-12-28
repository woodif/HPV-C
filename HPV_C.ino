#include <TFT_eSPI.h>
#include <TFT_Touch.h>

TFT_eSPI tft = TFT_eSPI(240, 320); /* TFT instance */

// These are the pins used to interface between the 2046 touch controller and Arduino Pro
#define DOUT 39  /* Data out pin (T_DO) of touch screen */
#define DIN  32  /* Data in pin (T_DIN) of touch screen */
#define DCS  33  /* Chip select pin (T_CS) of touch screen */
#define DCLK 25  /* Clock pin (T_CLK) of touch screen */

#define pwmPin  22    // connector
#define sensorPin  27    // ldr
#define vr1Pin  35    // connector
#define vr2Pin  34    // connector //r

const int analogPin = 27; // Analog pin on ESP32
const int numReadings = 12; // Number of readings to average
int readings[numReadings];   // Array to store readings
int indexx = 0;               // Index for storing readings
int total = 0;               // Total of readings

const int numReadings1 = 10; // Number of readings to average
int readings1[numReadings1];   // Array to store readings
int indexx1 = 0;               // Index for storing readings
int total1 = 0;               // Total of readings

int average;
int average1;
/* Create an instance of the touch screen library */
TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

TaskHandle_t Task1;

int vr1;
int vr2;
int dutyCycle;
int passValue = 0;
int blink_delay = 0;
// setting PWM properties
const int freq = 5000;
const int ledChannel = 1;
const int resolution = 10;

void TaskAnalogRead( void *pvParameters );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pwmPin, OUTPUT);
  digitalWrite(pwmPin, LOW);    // turn the LED off by making the voltage LOW
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pwmPin, ledChannel);


  tft.begin();          /* TFT init */
  tft.setRotation(3); /* Landscape orientation, flipped */
  touch.setCal(526, 3443, 750, 3377, 320, 240, 1);
  tft.fillScreen(0x0);
  // Display Menu
  tft.setCursor(40, 20); // Set the cursor position X,Y
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);
  tft.print("HPV-C Detector");

  tft.setCursor(50, 125); // Set the cursor position X,Y
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);
  tft.print("ADC");

  tft.setCursor(50, 165); // Set the cursor position X,Y
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);
  tft.print("Light");
  tft.setCursor(245, 165); // Set the cursor position X,Y
  tft.print("%");

  tft.setCursor(50, 205); // Set the cursor position X,Y
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);
  tft.print("Time");
  tft.setCursor(245, 205); // Set the cursor position X,Y
  tft.print("S");

  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(30, 60); // Set the cursor position X,Y
  tft.setTextSize(3);
  tft.print("PRESS For Start");

  // This variant of task creation can also specify on which core it will be run (only relevant for multi-core ESPs)
  xTaskCreatePinnedToCore(
    TaskAnalogRead,   /* Task function. */
    "Analog Read",     /* name of task. */
    20000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    1);          /* pin task to core 0 */
}

/* Main program */
void loop()
{
  //  // Read analog value and add it to total
  //  total = total - readings[indexx]; // Subtract the oldest reading
  //  readings[indexx] = analogRead(analogPin); // Read new value
  //  total = total + readings[indexx]; // Add the new reading
  //
  //  // Move to the next index, wrap around if needed
  //  indexx = (indexx + 1) % numReadings;
  //
  //  // Calculate the average
  //  int average = total / numReadings;
  //
  //  Serial.print("Average Value: ");
  //  Serial.println(average);

  //  delay(100); // Delay for visibility (adjust as needed)

  if (touch.Pressed()) // Note this function updates coordinates stored within library variables
  {
    Serial.println("Start Programs");
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(30, 60); // Set the cursor position X,Y
    tft.setTextSize(3);
    tft.print("PRESS For Start");

    Serial.println("Start Programs");
    tft.setTextColor(TFT_PINK);
    tft.setCursor(55, 80); // Set the cursor position X,Y
    tft.setTextSize(4);
    tft.print("PROCESS..");

    dutyCycle = map(vr1, 0, 100, 0, 1024);
    blink_delay = vr2;
    Serial.print("PWM : ");
    Serial.println(dutyCycle);
    Serial.print("Delay Time: ");
    Serial.println(blink_delay);

    ledcWrite(ledChannel, dutyCycle); //ledcWrite(ledChannel, dutyCycle);
    delay(blink_delay * 1000);
    ledcWrite(ledChannel, 0); //ledcWrite(ledChannel, dutyCycle);

//    delay(1); // Delay for visibility (adjust as needed)
//
//    // Read analog value and add it to total
//    total = total - readings[indexx]; // Subtract the oldest reading
//    readings[indexx] = analogRead(analogPin); // Read new value
//    total = total + readings[indexx]; // Add the new reading
//
//    // Move to the next index, wrap around if needed
//    indexx = (indexx + 1) % numReadings;
//
//    // Calculate the average
//    int average = total / numReadings;
//
////    Serial.print("Average : ");
////    Serial.println(average);
//
//    // tft.setTextFont(GLCD);
//    tft.setTextSize(4);
//    tft.setCursor(60, 160);
//    tft.setTextColor(TFT_BLACK);
//    tft.println(average);

    Serial.println("Start Programs");
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(55, 80); // Set the cursor position X,Y
    tft.setTextSize(4);
    tft.print("PROCESS..");

//    if (average >= 0) {
//      //      tft.setTextColor(TFT_PINK);
//      //      tft.setCursor(30, 60); // Set the cursor position X,Y
//      //      tft.setTextSize(4);
//      //      tft.print("Average ADC");
//      //      tft.setCursor(140, 110); // Set the cursor position X,Y
//      //      tft.println(average);
//      //      delay(3000);
//      //
//      //      tft.setTextColor(TFT_BLACK);
//      //      tft.setCursor(30, 60); // Set the cursor position X,Y
//      //      tft.setTextSize(4);
//      //      tft.print("Average ADC");
//      //      tft.setCursor(140, 110); // Set the cursor position X,Y
//      //      tft.println(average);
//      int valprint = average;
//
//      if (average >= 100) { //
//        tft.setTextColor(TFT_RED);
//        tft.setCursor(90, 60); // Set the cursor position X,Y
//        tft.setTextSize(3);
//        tft.print("DETECT!");
//        tft.setCursor(150, 90); // Set the cursor position X,Y
//        tft.println(valprint);
//        delay(5000);
//
//        tft.setTextColor(TFT_BLACK);
//        tft.setCursor(90, 60); // Set the cursor position X,Y
//        tft.setTextSize(3);
//        tft.print("DETECT!");
//        tft.setCursor(150, 90); // Set the cursor position X,Y
//        tft.println(valprint);
//
//      } else {
//        tft.setTextColor(TFT_GREEN);
//        tft.setCursor(75, 60); // Set the cursor position X,Y
//        tft.setTextSize(3);
//        tft.print("NOT DETECT");
//        tft.setCursor(150, 90); // Set the cursor position X,Y
//        tft.println(valprint);
//        delay(5000);
//
//        tft.setTextColor(TFT_BLACK);
//        tft.setCursor(75, 60); // Set the cursor position X,Y
//        tft.setTextSize(3);
//        tft.print("NOT DETECT");
//        tft.setCursor(150, 90); // Set the cursor position X,Y
//        tft.println(valprint);
//
//      }
//    }  else {
//      int aa = average;
//      tft.setTextColor(TFT_WHITE);
//      tft.setCursor(65, 60); // Set the cursor position X,Y
//      tft.setTextSize(3);
//      tft.print("NO DATA");
//      tft.setCursor(150, 100); // Set the cursor position X,Y
//      tft.println(aa);
//      int
//      delay(5000);
//
//      tft.setTextColor(TFT_BLACK);
//      tft.setCursor(65, 60); // Set the cursor position X,Y
//      tft.setTextSize(3);
//      tft.print("NO DATA");
//      tft.setCursor(150, 100); // Set the cursor position X,Y
//      tft.println(aa);
//    }

    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(30, 60); // Set the cursor position X,Y
    tft.setTextSize(3);
    tft.print("PRESS For Start");

  }

}


void TaskAnalogRead(void *pvParameters) { // This is a task.
  (void) pvParameters;
  // Check if the given analog pin is usable - if not - delete this task

  for (;;) {

    // tft.setTextFont(GLCD);
    tft.setTextSize(4);
    tft.setCursor(150, 160);
    tft.setTextColor(TFT_BLACK);
    tft.println(vr1);
    // tft.setTextFont(GLCD);
    tft.setTextSize(4);
    tft.setCursor(150, 200);
    tft.setTextColor(TFT_BLACK);
    tft.println(vr2);

    vr1 = map(analogRead(vr1Pin), 0, 4095, 100, 0);
    vr2 = map(analogRead(vr2Pin), 0, 4095, 30, 0);

    // Read analog value and add it to total
    total1 = total1 - readings1[indexx1]; // Subtract the oldest reading
    readings1[indexx1] = analogRead(analogPin); // Read new value
    total1 = total1 + readings1[indexx1]; // Add the new reading

    // Move to the next index, wrap around if needed
    indexx1 = (indexx1 + 1) % numReadings1;

    // Calculate the average
    int average1 = total1 / numReadings1;

    Serial.print("Average Value: ");
    Serial.println(average1);
    // tft.setTextFont(GLCD);
    tft.setTextSize(4);
    tft.setCursor(150, 120);
    tft.setTextColor(TFT_ORANGE);
    tft.println(average1);

    // tft.setTextFont(GLCD);
    tft.setTextSize(4);
    tft.setCursor(150, 160);
    tft.setTextColor(TFT_RED);
    tft.println(vr1);
    // tft.setTextFont(GLCD);
    tft.setTextSize(4);
    tft.setCursor(150, 200);
    tft.setTextColor(TFT_GREEN);
    tft.println(vr2);

    delay(50);

    // tft.setTextFont(GLCD);
    tft.setTextSize(4);
    tft.setCursor(150, 120);
    tft.setTextColor(TFT_BLACK);
    tft.println(average1);
  }
}
