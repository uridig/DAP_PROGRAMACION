//Blas Strambi, Echi Bespre, Gaby Mendelovich, Uriel Digestani

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

#define BOT_TOKEN ""
#define CHAT_ID ""

//estados
#define MODOINICIAL 0
#define CAMBIAR_UMBRAL 1
#define PASAJE_UMBRAL 2
#define VUELTA_MI 3  //Vuelta modo inicial
#define AUMENTAR_UMBRAL 4

//CAMBIO, NUEVOS ESTADOS
#define REDUCIR_UMBRAL 5
#define MODOINICIAL_PRIMERBOTON 6  //Cambio de Pantalla
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

void mostrarTemp();
///Ejemplo de uso de 2 loop en esp32 con el encendido de 2 led . Cada led se enciende y se utiliza un delay para bloquear la tarea

TaskHandle_t Task1;  //Definición de Tasks
TaskHandle_t Task2;

// LED pins


void setup() {
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
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
    if (dht.readTemperature() >= umbral) {
      if (y == 0) {
        bot.sendMessage(CHAT_ID, "La temp es mayor");  //Solo manda este mensaje una sola vez
        y = 1;
      }
    } else {
      y = 0;
    }
    funcionTelegram();
  }
}

//Task2code: blinks an LED every 700 ms
void Task2code(void* pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {  //si o si tiene que ir un while o un for para que nunca salga de la tarea
    maquinaEstado();
  }
}

void loop() {
}

void maquinaEstado() {
  Serial.println(estadoActivo);

  switch (estadoActivo) {
    case MODOINICIAL:  //Muestra Temp y Umbral
      if (digitalRead(BOTON1) == 0) {
        estadoActivo = MODOINICIAL_PRIMERBOTON;  //Cambio a modo intermedio 1
        tiempo = millis();
      }

      //CAMBIO, NUEVA FUNCION PARA MOSTRAR TEMP Y UMBRAL EN PANTALLA
      mostrarTemp();

      break;

    case MODOINICIAL_PRIMERBOTON:  //Modo Intermedio 1
      if (millis() - tiempo >= 5000) {
        estadoActivo = MODOINICIAL;  //Resetea a la primera pantalla
        tiempo = millis();
      }
      if (digitalRead(BOTON2) == 0 && digitalRead(BOTON1) == 1) {
        estadoActivo = MODOINICIAL_SEGUNDOBOTON;  //Pasa a modo intermedio 2
        tiempo = millis();
      }
      mostrarTemp();
      break;

    case MODOINICIAL_SEGUNDOBOTON:  //Modo intermedio 2
      if (millis() - tiempo >= 5000) {
        estadoActivo = MODOINICIAL;  //Resetea a pantalla inicial
        tiempo = millis();
      }
      if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 1) {
        estadoActivo = PASAJE_UMBRAL;  //Pasa a intermedio 3
        tiempo = millis();
      }
      mostrarTemp();
      break;
  }

  case PASAJE_UMBRAL:  //Espera a que ambos botones se liberen
    if (digitalRead(BOTON1) == 0 || digitalRead(BOTON2) == 0) {
      tiempo = millis();
    }
    if (millis() - tiempo >= 15) {
      tiempo = millis();
      estadoActivo = CAMBIAR_UMBRAL;
    }
    break;

  case CAMBIAR_UMBRAL:  //2da pantalla, solo se muestra Umbral (Se puede cambiar)

    u8g2.clearBuffer();  // clear the internal memory

    u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font

    u8g2.drawStr(15, 50, "Umbral:");
    sprintf(stringumbral, "%d", umbral);
    u8g2.drawStr(70, 50, stringumbral);
    u8g2.drawStr(90, 50, "C");
    u8g2.sendBuffer();
    if (digitalRead(BOTON1) == 0) {
      estadoActivo = AUMENTAR_UMBRAL;  //Umbral++
    }
    if (digitalRead(BOTON2) == 0) {
      estadoActivo = REDUCIR_UMBRAL;  //Umbral--
    }
    if (digitalRead(BOTON1) == 0 && digitalRead(BOTON2) == 0) {
      estadoActivo = VUELTA_MI;  //Intermedio para la vuelta, se aprietan ambos botones a la vez
      tiempo = millis();
    }
    break;

  case VUELTA_MI:  //Intermedio vuelta, espera a que ambos botones se liberen
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
      estadoActivo = VUELTA_MI;  //Corta y vuelve a pantalla 1
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
      estadoActivo = VUELTA_MI;  //Corta y vuelve a pantalla 1
    }
    if (millis() - tiempo >= 15) { 
      umbral--;
      estadoActivo = CAMBIAR_UMBRAL;
      tiempo = millis();
      umbrall = String(umbral);
    }

    break;
}
void funcionTelegram() {
  cantMsj = bot.getUpdates(bot.last_message_received + 1);
  int i = 0;
  while (cantMsj > 0) {
    for (i = 0; i < cantMsj; i++) {
      //TERCER CAMBIO, LEEMOS TODOS LOS MENSAJES NUEVOS EMPEZANDO DEL PRIMER MENSAJE ENVIADO, NO SOLO EL ULTIMO, USAMOS FOR
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
