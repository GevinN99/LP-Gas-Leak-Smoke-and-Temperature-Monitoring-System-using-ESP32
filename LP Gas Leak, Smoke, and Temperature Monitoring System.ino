#include <WiFi.h>         //Including wifi.h library it will take care of all wifi related task
#include <HTTPClient.h>   //Including HTTPClient.h library to use all api

#include "DHT.h"
#define DHT11PIN 22

DHT dht(DHT11PIN, DHT11);

// int Gas_analog = 4;
int Gas_digital = 4;

int buzzerPin = 2;

int relayPin = 25;

const char* ssid = "Gevin_N";             //WiFi ssid
const char* password =  "gskn1234";    //WiFi password

String apiKey = "7439970";
String phone_number = "+94773910444";

String url;                            //url String will be used to store the final generated URL

void setup() 
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);              // Try to connect with the given SSID and PSS
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {  // Wait until WiFi is connected
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to the WiFi network"); 

  pinMode(Gas_digital, INPUT);

  pinMode(buzzerPin, OUTPUT);

  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, HIGH);

  dht.begin();

  message_to_whatsapp("__Security System On__"); 
}

void loop()
{
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  // int gassensorAnalog = analogRead(Gas_analog);
  int gassensorDigital = digitalRead(Gas_digital);

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("ºC ");x 

  Serial.print(" Gas Digital : ");
  Serial.println(gassensorDigital);

  if( gassensorDigital != 1 ){
    message_to_whatsapp("Alert!!! Gas Leak!!!!");  // Sending alert message to WhatsApp
    buzzer();
  }

  if(temp > 35.0 && gassensorDigital != 1 ){
    message_to_whatsapp("Temperature>35C!!! May cause explosions!!!");  // Sending alert message to WhatsApp
    buzzer();
  }


}

void  message_to_whatsapp(String message)       // send meassage to WhatsApp app
{
  url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);

  postData();
}

void postData()     //function used to call api(POST data)
{
  int httpCode;     // variable used to get the responce http code after calling api
  HTTPClient http;  // Declare object of class HTTPClient
  http.begin(url);  // begin the HTTPClient object with generated url
  httpCode = http.POST(url); // Finaly Post the URL with this function and it will store the http code
  // if (httpCode == 200)      // Check if the responce http code is 200
  // {
  //   Serial.println("Sent ok."); // print message sent ok message
  // }
  // else                      // if response HTTP code is not 200 it means there is some error.
  // {
  //   Serial.println("Error."); // print error message.
  // }
  http.end();          // After calling API end the HTTP client object.
}

String urlencode(String str)  // Function used for encoding the url
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}

void buzzer (){                   // Function used for warning
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(relayPin, LOW);
  delay(400);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(relayPin, HIGH);
  delay(100);
}
