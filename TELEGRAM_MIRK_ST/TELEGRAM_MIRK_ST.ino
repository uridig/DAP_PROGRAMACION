//Blas, Echi, Gaby, Uri

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include <U8g2lib.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define SENSOR_TEMP 23  //fijarse bien el número
#define LED1 25

DHT dht(SENSOR_TEMP, DHT11);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE); //Seteo del sensor


const char* ssid = "ORT-IoT";
const char* password = "OrtIOTnew22$2"; 

#define BOT_TOKEN "7114974258:AAGbNV5exIYk7kbjlWK_UHg_MRgNB0kbq0Y"
#define CHAT_ID "5085758476"

//estados
#define MODOINICIAL 0
#define CAMBIAR_UMBRAL 1
#define PASAJE_UMBRAL 2
#define VUELTA_MI 3
#define AUMENTAR_UMBRAL 4

#define BOTON1 35
#define BOTON2 34

int estadoActivo = MODOINICIAL;
int x = 0;
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

///Ejemplo de uso de 2 loop en esp32 con el encendido de 2 led . Cada led se enciende y se utiliza un delay para bloquear la tarea

TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins


void setup() {
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);
  pinMode(LED1,OUTPUT);
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

  xTaskCreatePinnedToCore(
    Task1code, /* Task function. */
    "Task1",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task1,    /* Task handle to keep track of created task */
    0);        /* pin task to core 0 */
  delay(500);

  xTaskCreatePinnedToCore(
    Task2code, /* Task function. */
    "Task2",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task2,    /* Task handle to keep track of created task */
    1);        /* pin task to core 1 */
  delay(500);
}

//Task1code: blinks an LED every 1000 ms
void Task1code(void* pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {  //si o si tiene que ir un while o un for para que nunca salga de la tarea
    if (dht.readTemperature() >= umbral && millis() - tiempo2 > 6000) {
      bot.sendMessage(CHAT_ID, "La temp es mayor");
      tiempo2 = millis();
    }

    if (millis() - tiempomsg > 10000) {
      tiempomsg = millis();
      cantMsj = bot.getUpdates(bot.last_message_received + 1);
      if (cantMsj > 0) {
        String text = bot.messages[0].text;
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

        cantMsj = bot.getUpdates(bot.last_message_received + 1);
      }
    }
  }
}

//Task2code: blinks an LED every 700 ms
void Task2code(void* pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {  //si o si tiene que ir un while o un for para que nunca salga de la tarea
    Serial.println(estadoActivo);
    if (dht.readTemperature() >= umbral) {
      digitalWrite(LED1, HIGH);
    } else {
      digitalWrite(LED1, LOW);
    }
    switch (estadoActivo) {
      case MODOINICIAL:
        if (x <= 1 && digitalRead(BOTON1) == 0) {
          x = 1;
          tiempo = millis();
        }
        if (x >= 1 && x <= 2 && digitalRead(BOTON2) == 0 && digitalRead(BOTON1) == 1) {
          x = 2;
          tiempo = millis();
        }
        if (x == 2 && digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 1) {
          estadoActivo = PASAJE_UMBRAL;
          tiempo = millis();
          x = 0;
        }
        if (millis() - tiempo >= 5000) {
          x = 0;
          tiempo = millis();
        }
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
          x = 4;
        }
        if (digitalRead(BOTON2) == 0) {
          x = 5;
        }
        if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 0) {
          estadoActivo = VUELTA_MI;
          tiempo = millis();
          x = 0;
        }
        if (x >= 4) {
          estadoActivo = AUMENTAR_UMBRAL;
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
          x = 0;
        }
        if (millis() - tiempo >= 15) {
          if (x == 4) {
            umbral++;
            estadoActivo = CAMBIAR_UMBRAL;
            x = 0;
            tiempo = millis();
            umbrall = String(umbral);
          }
          if (x == 5) {
            umbral--;
            estadoActivo = CAMBIAR_UMBRAL;
            x = 0;
            tiempo = millis();
            umbrall = String(umbral);
          }
        }

        break;
    }
  }
}

void loop() {
}