//Charnis, Digestani, Lopez, Strambi
//Grupo 7
#include <Preferences.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>



#define SENSOR_TEMP 23  //fijarse bien el número
#define LED 25
#define MOSTRAR 0
#define CICLO 1
#define PASAJE_CICLO 2
#define VUELTA_MOSTRAR 3
#define SUMAR_CICLO 4
#define BAJAR_CICLO 5
#define BOTON1 35
#define BOTON2 34


DHT dht(SENSOR_TEMP, DHT11);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
int tiempo;
int tiempoPasaje21;
int temp;
int umbral = 23;
int estadoActivo = MOSTRAR;
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
    case MOSTRAR:
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


      if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 0) {
        tiempo = millis();
        estadoActivo = PASAJE_CICLO;
      }

      break;
    case CICLO:

      if (digitalRead(BOTON1) == 0) {
        estadoActivo = SUMAR_CICLO;
        tiempo = millis();
      }
      if (digitalRead(BOTON2) == 0) {
        estadoActivo = BAJAR_CICLO;
        tiempo = millis();
      }
      u8g2.clearBuffer();  // clear the internal memory

      u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font

      u8g2.drawStr(15, 50, "Umbral:");
      sprintf(stringumbral, "%d", umbral);
      u8g2.drawStr(70, 50, stringumbral);
      u8g2.drawStr(90, 50, "C");
      u8g2.sendBuffer();  // transfer internal memory to the display
      break;
    case PASAJE_CICLO:
      if (digitalRead(BOTON1) == 0 || digitalRead(BOTON2) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        preferences.begin("Lugar guardado", false);
        umbral = preferences.getUInt("umbral", 23);
        estadoActivo = CICLO;
      }

      break;
    case VUELTA_MOSTRAR:
      if (digitalRead(BOTON2) == 0 || digitalRead(BOTON1) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        preferences.putUInt("umbral", umbral);
        preferences.end();
        estadoActivo = MOSTRAR;
      }
      break;
    case SUMAR_CICLO:
      if (digitalRead(BOTON1) == 0) {
        tiempo = millis();
      }
      if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 0) {
        estadoActivo = VUELTA_MOSTRAR;
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        estadoActivo = CICLO;
        umbral++;
      }


      break;
    case BAJAR_CICLO:
      if (digitalRead(BOTON2) == 0) {
        tiempo = millis();
      }
      if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 0) {
        estadoActivo = VUELTA_MOSTRAR;
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        estadoActivo = CICLO;
        umbral--;
      }

      break;
  }
}