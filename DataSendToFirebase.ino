
#include <WiFi.h>

#include <FirebaseESP32.h>

#define FIREBASE_HOST "https://loadcellautomation-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "mSt3j4KHO2X08FhPKRvxohBdWUQNcyAZTlG1d2iX"
FirebaseData firebaseData;

int Loai1, Loai2, Total, TotalMass;
String Chuoi = "0,0:0", Chuoi1, Chuoi2, Chuoi3;
byte ngat1, ngat2;

void setup()
{
Serial.begin(115200);

  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
//  WiFi.begin("Hama","hamabungbu");
  WiFi.begin("Trong Thuc","Cotroimoibietnhe");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println(0);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}


void loop()
{

  if(Serial.available()){
    Chuoi = Serial.readString();
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
  
  

    if ((Firebase.setInt(firebaseData, "/Loadcell/Loai1", Loai1)) == true) {
      Serial.println(Loai1);

    } else {
      Serial.println("Error");

    }
    if ((Firebase.setInt(firebaseData, "/Loadcell/Loai2", Loai2)) == true) {
      Serial.println(Loai2);

    } else {
      Serial.println("Error");

    }
    if ((Firebase.setInt(firebaseData, "/Loadcell/Total", Total)) == true) {
      Serial.println(Total);

    } else {
      Serial.println("Error");

    }
    if ((Firebase.setInt(firebaseData, "/Loadcell/TotalMass", TotalMass)) == true){
      Serial.println(TotalMass);
      
    } else {
      Serial.println("Error");
    }
    
    delay(5000);

}
