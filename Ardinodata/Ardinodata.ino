
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <OneWire.h>
#include <DallasTemperature.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Dialog 4G 477"
#define WIFI_PASSWORD "aFdC24D3"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyDoZv5XO2i2pIXR5NwW4zmQVPhvrXa85z0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "water-6aa5e-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "sis@gmail.com"
#define USER_PASSWORD "sis123456"


// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;



#define SensorPin A0
#define Offset 41.02740741
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40
#define uart  Serial


const int pHArray [ArrayLenth];
const int pHArrayIndex = 0;

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

//---- Define input pin for Solu Water Sensor ----
const const int resval = 0; //holds the value
int respin = A5; //sensor pin used

  //---- Define input pin for relay ----
const int Relaypin= 3;


void setup (void)


{
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  pinMode (LED, OUTPUT);
  uart.begin (115200);
  uart.println ("pH Sensor Testing.");

  //tempureture
  {
  sensors.begin();  // Start up the library
  Serial.begin(115200);

  }
  
    //-- relay --
 
 // put your setup code here, to run once:
pinMode(Relaypin, OUTPUT); // Define the Relaypin as output pin

  //start the serial console
  Serial.begin(115200);

}
void function()

{
  static unsigned long samplingTime = millis ();
  static unsigned long printTime = millis ();
  static float pHValue, voltage;
  
  
  if (millis () - samplingTime > samplingInterval)
  {
    pHArray [pHArrayIndex++] = analogRead (SensorPin);
    
    if (pHArrayIndex == ArrayLenth) pHArrayIndex = 0;
    voltage = avergearray (pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = -19.18518519 * voltage + Offset;
    samplingTime = millis ();
  }
  
  if (millis () - printTime > printInterval)
  {
    uart.print ("Voltage:");
    uart.print (voltage, 2);
    uart.print ("    pH value: ");
    uart.println (pHValue, 2);
    digitalWrite (LED, digitalRead (LED) ^ 1);
    printTime = millis ();
  }

  //tempureture

  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("°");//shows degrees character
  Serial.print("C  |  ");
  
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print("°");//shows degrees character
  Serial.println("F");
  
  delay(500);
     resval=analogRead(respin);//Read data from analog pin and store it to resval variable

   if (resval<=350){
    
     //Relay module is OFF
  digitalWrite(Relaypin, HIGH); // Sends high signal 
  delay(1000); // Waits for 1 second
  
    Serial.println("Water Level:-  Low");
   }

   else {

            //Relay Module is ON
    digitalWrite(Relaypin, LOW); // Makes the signal low
    delay(1000); // Waits for 1 second
    
    Serial.println("Water Level:- Good");
   }
   delay(1000);
}
double avergearray (int* arr, int number)

{
  int i;
  int max, min;
  double avg;
  long amount = 0;
  
  if (number <= 0)
  
  {
    uart.println ("Error number for the array to avraging!/n");
    return 0;
  }
  
  if (number < 5)
  {
    for (i = 0; i < number; i++)
    {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
    }
  
  else
  
  {
    if (arr[0] < arr[1])
    {
      min = arr[0]; max = arr[1];
    }
    
    else
    {
      min = arr[1]; max = arr[0];
    }
    
    for (i = 2; i < number; i++)
    {
      if (arr[i] < min)
      {
        amount += min;
        min = arr[i];
      }
      else
      {
        if (arr[i] > max)
        {
          amount += max;
          max = arr[i];
        }
        else
        {
          amount += arr[i];
        }
      }
    }
    avg = (double) amount / (number - 2);
  }
  return avg;
  }

void loop()
{
function();
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {

    sendDataPrevMillis = millis();

   Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, F("/tempurature/celcuence/int"),sensors.getTempCByIndex(0)) ? "ok" : fbdo.errorReason().c_str());

   Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, F("/temmpurature/firenthitr/int"),sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0)?" ok" : fbdo.errorReason().c_str());



    Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/Water level/string"),Water_level) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, F("/Water level/int"),Water_Level) ? "ok" : fbdo.errorReason().c_str());

   Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, F("/Ph/int")pHValue, 2) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());



    Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/voltage/int"),voltage, 2) ? "ok" : fbdo.errorReason().c_str());
 
  }

delay(5000);
   
}
