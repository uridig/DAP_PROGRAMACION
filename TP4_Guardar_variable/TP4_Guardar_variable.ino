#include <Preferences.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>



#define SENSOR_TEMP 23  //fijarse bien el número
#define LED 25
#define P1 0
#define P2 1
#define ANTIRREBOTE12 2
#define ANTIRREBOTE21 3
#define SUMAR_UMBRAL 4
#define BAJAR_UMBRAL 5
#define BOTON1 35
#define BOTON2 34


DHT dht(SENSOR_TEMP, DHT11);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
int tiempo;
int tiempoPasaje21;
int temp;
int umbral = 23;
int estadoActivo = P1;
char stringumbral[5];
char stringtemp[5];
Preferences preferences;



void setup() {
  Serial.begin(9600);
  u8g2.begin();
  dht.begin();
  pinMode(SENSOR_TEMP, INPUT);  //FIJARSE SI ESTO VA
  pinMode(LED, OUTPUT);
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);

  preferences.begin("Lugar guardado", false);
  umbral = preferences.getUInt("umbral", 23);
  preferences.end();
}

void loop() {
  maquinaEstado();

  if (dht.readTemperature() >= umbral) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

void maquinaEstado() {
  Serial.println(estadoActivo);

  switch (estadoActivo) {
    case P1:
      temp = dht.readTemperature();
      //Serial.println(temp);
      u8g2.clearBuffer();  // clear the internal memory

      u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
      sprintf(stringtemp, "%d", temp);
      u8g2.drawStr(15, 20, "Temp:");
      u8g2.drawStr(70, 20, stringtemp);
      u8g2.drawStr(90, 20, "C");
      u8g2.drawStr(15, 50, "Umbral:");
      sprintf(stringumbral, "%d", umbral);
      u8g2.drawStr(70, 50, stringumbral);
      u8g2.drawStr(90, 50, "C");
      u8g2.sendBuffer();  // transfer internal memory to the display


      if (digitalRead(BOTON1) == 1) {
        tiempo = millis();
      }
      if (millis() - tiempo > 5000) {
        estadoActivo = ANTIRREBOTE12;
        tiempo = millis();
      }
      break;
    case P2:

      if (digitalRead(BOTON1) == 0) {
        estadoActivo = SUMAR_UMBRAL;
        tiempo = millis();
      }
      if (digitalRead(BOTON2) == 0) {
        estadoActivo = BAJAR_UMBRAL;
        tiempo = millis();
        tiempoPasaje21 = millis();
      }
      u8g2.clearBuffer();  // clear the internal memory

      u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font

      u8g2.drawStr(15, 50, "Umbral:");
      sprintf(stringumbral, "%d", umbral);
      u8g2.drawStr(70, 50, stringumbral);
      u8g2.drawStr(90, 50, "C");
      u8g2.sendBuffer();  // transfer internal memory to the display
      break;
    case ANTIRREBOTE12:
      if (digitalRead(BOTON1) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        preferences.begin("Lugar guardado", false);
        umbral = preferences.getUInt("umbral", 23);
        estadoActivo = P2;
      }
      break;
    case ANTIRREBOTE21:
      if (digitalRead(BOTON2) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        preferences.putUInt("umbral", umbral);
        preferences.end();
        estadoActivo = P1;
      }
      break;
    case SUMAR_UMBRAL:
      if (digitalRead(BOTON1) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        estadoActivo = P2;
        umbral++;
      }


      break;
    case BAJAR_UMBRAL:
      if (digitalRead(BOTON2) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempoPasaje21 > 5000) {
        estadoActivo = ANTIRREBOTE21;

        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        estadoActivo = P2;
        umbral--;
      }

      break;
  }
}