#include <DHT.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

LiquidCrystal_I2C lcd(0x27,16,2);

//Array simbolo grau
byte grau[8] ={ B00001100,
                B00010010,
                B00010010,
                B00001100,
                B00000000,
                B00000000,
                B00000000,
                B00000000,};

#define DHTPIN 4     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// Initialize Wifi connection to the router
char ssid[] = "";     // your network SSID (name)
char password[] = ""; // your network key

// Initialize Telegram BOT
#define BOTtoken "1692733546:AAFqsLQqj5LCl0YjQFwZxbiTcWZB5KsRTGA"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

String id, text;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario
unsigned long tempo;

float temperatura (){
    float t = dht.readTemperature();
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(t)) {
      lcd.print("Not working");
    }
  
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print("Temp : ");
    lcd.print(" ");
    lcd.setCursor(7,0);
    lcd.print(t,1);
    lcd.setCursor(12,0);
   
  //Mostra o simbolo do grau formado pelo array
    lcd.write((byte)0);
    lcd.print("C");
   
  //Mostra o simbolo do grau quadrado
  //lcd.print((char)223);
  
    return t;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

float umidade(){
   float h = dht.readHumidity();
   lcd.setCursor(0,1);
   lcd.print("Umid : ");
   lcd.print(" ");
   lcd.setCursor(7,1);
   lcd.print(h,1);
   lcd.setCursor(12,1);
   lcd.print("%");

   return h;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  
  Serial.begin(115200);
  Serial.println(F("DHT11 test!"));
  lcd.init();
  //Cria o caractere customizado com o simbolo do grau
  lcd.createChar(0, grau);
  dht.begin();
//---------------------------------------------------------------------------------------------------------------
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
}

  
void loop() {
 
 float temp = temperatura();
 float umid = umidade();
 
//Intervalo recomendado para leitura do sensor
 delay(2000);
//------------------------------------------------------------------------------------------------------------------------------------------
 
 int parar = 0;

   int newmsg = bot.getUpdates(bot.last_message_received + 1);
   for (int i = 0; i < newmsg; i++){//Caso haja X mensagens novas, fara este loop X Vezes.
      id = bot.messages[i].chat_id;//Armazenara o ID do Usuario à Váriavel.
      text = bot.messages[i].text;//Armazenara o TEXTO do Usuario à Váriavel.
      text.toUpperCase();//Converte a STRING_TEXT inteiramente em Maiuscúla.
     
      if(text.indexOf("STOP") > -1){
        bot.sendSimpleMessage(id, "OFF", "");
      }else if (text.indexOf("START") > -1){//Caso o texto recebido contenha "START
          while(parar != 1){

            temp = temperatura();
            umid = umidade();
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            if(umid < 75){
              bot.sendSimpleMessage(id, "UMIDADE BAIXA", "");
              String messageUmid = "Umidade: ";
              messageUmid.concat(" ");
              messageUmid.concat(umid);
  
              bot.sendSimpleMessage(id, messageUmid, "");
  
              String messageTemp = "Temperatura: ";
              messageTemp.concat(" ");
              messageTemp.concat(temp);
  
              bot.sendSimpleMessage(id, messageTemp, "");
            }

            temp = temperatura();
            umid = umidade();
            
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            int newmsg = bot.getUpdates(bot.last_message_received + 1);
             for (int i = 0; i < newmsg; i++){//Caso haja X mensagens novas, fara este loop X Vezes.
                id = bot.messages[i].chat_id;//Armazenara o ID do Usuario à Váriavel.
                text = bot.messages[i].text;//Armazenara o TEXTO do Usuario à Váriavel.
                text.toUpperCase();//Converte a STRING_TEXT inteiramente em Maiuscúla.

                 if(text.indexOf("STOP") > -1){
                  parar = 1;
                }
             }

            temp = temperatura();
            umid = umidade();
          } 
      }else//Caso o texto recebido nao for nenhum dos acima, Envia uma mensagem de erro.
      {
         bot.sendSimpleMessage(id, "Comando Invalido", "");
      }
   }
}
