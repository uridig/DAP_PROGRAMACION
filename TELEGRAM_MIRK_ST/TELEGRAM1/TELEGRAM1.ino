#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include <U8g2lib.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define SENSOR_TEMP 23  //fijarse bien el número
#define LED 25

DHT dht(SENSOR_TEMP, DHT11);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);


const char* ssid = "ORT-IoT";
const char* password = "OrtIOTnew22$2";

#define BOT_TOKEN "7114974258:AAGbNV5exIYk7kbjlWK_UHg_MRgNB0kbq0Y"
#define CHAT_ID "5085758476"

//estados
#define MODOINICIAL 0
#define CAMBIAR_UMBRAL 1
#define PASAJE_UMBRAL 2
#define VUELTA_MI 3 //Vuelta modo inicial
#define AUMENTAR_UMBRAL 4

//CAMBIO, NUEVOS ESTADOS
#define REDUCIR_UMBRAL 5
#define MODOINICIAL_PRIMERBOTON 6
#define MODOINICIAL_SEGUNDOBOTON 7

#define BOTON1 35
#define BOTON2 34

int estadoActivo = MODOINICIAL;
int y = 0;
int temp;
long int tiempo2 = -600000;
int umbral = 28;
String umbrall;
int tiempo = millis();
char stringumbral[5];
char stringtemp[5];
int cantMsj;
int tiempomsg = 0;
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);


void setup() {
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);

  Serial.begin(9600);
  u8g2.begin();
  dht.begin();
  pinMode(SENSOR_TEMP, INPUT);  //FIJARSE SI ESTO VA

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  if (dht.readTemperature() >= umbral) {
    if (y == 0) {
      bot.sendMessage(CHAT_ID, "La temp es mayor");
      y = 1;
    }
  } else {
    y = 0;
  }
  //PRIMER CAMBIO, DA MÁS VELOCIDAD, YA NO SE MANDA EL MENSAJE MUCHAS VECES

  maquinaEstado();

  if (millis() - tiempomsg > 10000) {
    tiempomsg = millis();
    funcionTelegram();
    //SEGUNDO CAMBIO, HACEMOS TELEGRAM DENTRO DE UNA FUNCION
  }


  void maquinaEstado() {
    Serial.println(estadoActivo);

    switch (estadoActivo) {
      case MODOINICIAL:
        if (digitalRead(BOTON1) == 0) {
          estadoActivo = MODOINICIAL_PRIMERBOTON;
          tiempo = millis();
        }

        //CAMBIO, NUEVA FUNCION PARA MOSTRAR TEMP Y UMBRAL
        mostrarTemp();

        break;
      case CAMBIAR_UMBRAL:

        u8g2.clearBuffer();  // clear the internal memory

        u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font

        u8g2.drawStr(15, 50, "Umbral:");
        sprintf(stringumbral, "%d", umbral);
        u8g2.drawStr(70, 50, stringumbral);
        u8g2.drawStr(90, 50, "C");
        u8g2.sendBuffer();
        if (digitalRead(BOTON1) == 0) {
          estadoActivo = AUMENTAR_UMBRAL;
        }
        if (digitalRead(BOTON2) == 0) {
          estadoActivo = REDUCIR_UMBRAL;
        }
        if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 0) {
          estadoActivo = VUELTA_MI;
          tiempo = millis();
        }


        break;
      case PASAJE_UMBRAL:
        if (digitalRead(BOTON1) == 0 || digitalRead(BOTON2) == 0) {
          tiempo = millis();
        }
        if (millis() - tiempo >= 15) {
          tiempo = millis();
          estadoActivo = CAMBIAR_UMBRAL;
        }
        break;
      case VUELTA_MI:
        if (digitalRead(BOTON1) == 1 && digitalRead(BOTON2) == 1) {
          estadoActivo = MODOINICIAL;
          tiempo = millis();
        }
        break;
      case AUMENTAR_UMBRAL:
        if (digitalRead(BOTON1) == 0 || digitalRead(BOTON2) == 0) {
          tiempo = millis();
        }
        if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 0) {
          tiempo = millis();
          estadoActivo = VUELTA_MI;
        }
        if (millis() - tiempo >= 15) {
          umbral++;
          estadoActivo = CAMBIAR_UMBRAL;
          tiempo = millis();
          umbrall = String(umbral);
        }

        break;
      //CAMBIO, NUEVO ESTADO
      case REDUCIR_UMBRAL:
        if (digitalRead(BOTON1) == 0 || digitalRead(BOTON2) == 0) {
          tiempo = millis();
        }
        if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 0) {
          tiempo = millis();
          estadoActivo = VUELTA_MI;
        }
        if (millis() - tiempo >= 15) {
          umbral--;
          estadoActivo = CAMBIAR_UMBRAL;
          tiempo = millis();
          umbrall = String(umbral);
        }

        break;
      case MODOINICAL_PRIMERBOTON:
        if (millis() - tiempo >= 5000) {
          estadoActivo = MODOINICIAL;
          tiempo = millis();
        }
        if (digitalRead(BOTON2) == 0 && digitalRead(BOTON1) == 1) {
          estadoActivo = MODOINICIAL_SEGUNDOBOTON;
          tiempo = millis();
        }
        mostrarTemp();
        break;
      case MODOINICIAL_SEGUNDOBOTON:
        if (millis() - tiempo >= 5000) {
          estadoActivo = MODOINICIAL;
          tiempo = millis();
        }
        if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 1) {
          estadoActivo = PASAJE_UMBRAL;
          tiempo = millis();
        }
        mostrarTemp();
        break;
    }
  }

  void funcionTelegram() {
    cantMsj = bot.getUpdates(bot.last_message_received + 1);
    if (cantMsj > 0) {
      for (i = 0, i < cantMsj, i++) {
        //TERCER CAMBIO, LEEMOS TODOS LOS MENSAJES NUEVOS, NO SOLO EL ULTIMO, USAMOS FOR
        String text = bot.messages[i].text;
        if (text == "/sensor") {
          String temperaturaString = String(dht.readTemperature());  // Interroga por datos del sensor
          bot.sendMessage(CHAT_ID, "La temperatura es " + temperaturaString);
        } else {
          if (text == "/umbral") {
            umbrall = String(umbral);
            bot.sendMessage(CHAT_ID, "El umbral es " + umbrall);
          } else {
            bot.sendMessage(CHAT_ID, "Las funciones son /umbral y /sensor");
          }
        }
      }
      cantMsj = bot.getUpdates(bot.last_message_received + 1);
    }
  }
}
void mostrarTemp() {
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
  u8g2.sendBuffer();
}
