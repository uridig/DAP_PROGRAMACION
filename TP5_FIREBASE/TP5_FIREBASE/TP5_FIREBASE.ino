//Grupo 4. Mendelovich, Cambiasso, De Los Santos, Digestani

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define SSID "ORT-IoT"
#define PASSWORD "OrtIOT24"
#define API_KEY "AIzaSyDomB5rYPLjIL5D39rRTta_g0H3dV4x4yU"
#define USER_EMAIL "urid720@gmail.com"
#define USER_PASSWORD "123456789"
#define DATABASE_URL "https://tp5-st-grupo4-default-rtdb.firebaseio.com/"

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




FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;
String databasePath;
String tempPath = "/temperature";
String timePath = "/timestamp";

String parentPath;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

DHT dht(SENSOR_TEMP, DHT11);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
int tiempo;
int temp;
unsigned long tiempoCiclo = 120000;
int ultimaActu = 0;
int estadoActivo = MOSTRAR;
char stringtemp[5];
char stringciclo[5];

void setup() {
  Serial.begin(9600);

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();

  u8g2.begin();
  dht.begin();
  pinMode(SENSOR_TEMP, INPUT);  //FIJARSE SI ESTO VA
  pinMode(LED, OUTPUT);
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);

  configTime(0, 0, ntpServer);

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);

  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  databasePath = "/UsersData/" + uid + "/readings";
}

void loop() {
  maquinaEstado();

  if (Firebase.ready() && (millis() - ultimaActu >= tiempoCiclo || ultimaActu == 0)) {

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Wifi conectado");
    
    }

    Serial.println("ESTAMOS MANDANDO");
    ultimaActu = millis();

    timestamp = getTime();
    parentPath = databasePath + "/" + String(timestamp);
    json.set(tempPath, String(temp));
    json.set(timePath, String(timestamp));
    Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json);
    Serial.println("YA MANDAMOS");
  }
}

void maquinaEstado() {
  switch (estadoActivo) {
    case MOSTRAR:
      temp = dht.readTemperature();
      //Serial.println(temp);
      u8g2.clearBuffer();  // clear the internal memory

      u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
      sprintf(stringtemp, "%d", temp);
      u8g2.drawStr(15, 30, "Temp:");
      u8g2.drawStr(70, 30, stringtemp);
      u8g2.drawStr(90, 30, "C");
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

      u8g2.drawStr(15, 20, "Ciclo:");
      sprintf(stringciclo, "%d", tiempoCiclo);
      u8g2.drawStr(10, 50, stringciclo);
      u8g2.drawStr(70, 50, "Segundos");
      u8g2.sendBuffer();  // transfer internal memory to the display
      break;
    case PASAJE_CICLO:
      if (digitalRead(BOTON1) == 0 || digitalRead(BOTON2) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        estadoActivo = CICLO;
      }
      break;
    case VUELTA_MOSTRAR:
      if (digitalRead(BOTON2) == 0 || digitalRead(BOTON1) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {

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
        tiempoCiclo = tiempoCiclo + 30000;
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
        if(tiempoCiclo>30000){
        tiempoCiclo = tiempoCiclo - 30000;
        } else{
          Serial.println("ERROR, MINIMO INTERVALO ALCANZADO");
        }
      }
      break;
  }
}


unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return (0);
  }
  time(&now);
  return now;
}