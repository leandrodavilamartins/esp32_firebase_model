#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);
// Insert your network credentials
#define WIFI_SSID "Mi Phone"
#define WIFI_PASSWORD "j6h6v2e2"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDkVzBpA1wr7fV8Y4v6J6jS06JzdVIRs54"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32-firebase-1c18f-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

float temperature;
float humidity;


//////////////////////////////
// Write float values to the database
void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
  Serial.print("Writing value: ");
  Serial.print (value);
  Serial.print(" on the following path: ");
  Serial.println(path);
  Serial.println("PASSED");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
}
  else {
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
  }
}




//SETUP/////////////////////
////////////////////////////
void setup(){

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

 // Inicia sensor 
  dht.begin();
}

//LOOP/////////////////
//////////////////////

void loop(){
  //int qtd = 0;
  //String caminho = "/sensor1";
  //json.set("DadoGravar",(String)qtd);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.println(temperature);
  delay(15000);

}
