#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "doan-kythuatdientu-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "nRa37DMVCO7AY1wwFkr2xgmh4HmhsdWOinUD8nbG"
#define WIFI_SSID "Hama"
#define WIFI_PASSWORD "hamabungbu"


int Loai1, Loai2, Total, TotalMass;
String Chuoi = "0,0:0", Chuoi1, Chuoi2, Chuoi3;
byte ngat1, ngat2;


void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop(){

  if(Serial.available()){
    Chuoi = Serial.readString();
    Serial.println(Chuoi);
  }
  
    for(int i = 0; i < Chuoi.length(); i++){
      if(Chuoi.charAt(i) == ','){
        ngat1 = i;
      } 
      if(Chuoi.charAt(i) == ':'){
        ngat2 = i;
      }
    }

    Chuoi1 = Chuoi;
    Chuoi2 = Chuoi;
    Chuoi3 = Chuoi;
    Chuoi1.remove(ngat1);
    Chuoi2.remove(0, ngat1 + 1);
    Chuoi3.remove(0, ngat2 + 1);
    Loai1 = Chuoi1.toInt();
    Loai2 = Chuoi2.toInt();
    TotalMass = Chuoi3.toInt();
    
  

    Total = Loai1 + Loai2;


    Firebase.setInt("/Loadcell/Loai1", Loai1);
    
    Firebase.setInt("/Loadcell/Loai2", Loai2);
    
    Firebase.setInt("/Loadcell/Total", Total);
    
    Firebase.setInt("/Loadcell/TotalMass", TotalMass);
     
    delay(5000);

    
}
