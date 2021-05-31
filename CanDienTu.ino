
/*
   pin 2 -> SCK: loadcell ( dây cam )
   pin 3 -> DT:  loadcell ( dây đỏ )
   A4    -> SDA: LCD ( dây xám )
   A5    -> SCL: LCD ( dây tím )
   pin 4 -> OUT: Tiệm cận 1 ( xanh dương )
   pin 6 -> Điều khiển 1 ( xanh lá )
   pin 7 -> Điều khiển 2 ( dây vàng )

   pin 10 -> RX
   pin 11 -> TX

   loadcell -> 5V
   LCD -> 5V
   Tiệm cận -> 5V
*/

#include <Q2HX711.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>

SoftwareSerial sentData(10,11);

#define reset 8

const byte hx711_data_pin = 3;
const byte hx711_clock_pin = 2;

float y1 = 20.0; // calibrated mass to be added
long x1 = 0L;
long x0 = 0L;
float avg_size = 10.0; // amount of averages for each mass measurement

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);
LiquidCrystal_I2C lcd(0x27, 20, 4);

float mass, klg;
int demtam=0, tiemcan=0, XamXi=0;
String hieuChuan = " Thu tai hoan thanh ";
String thuTai = "Thu tai he thong can";
String title = "He thong can dien tu";
String dinhMuc = "Khoi luong = 20g";
String caption1 = "Chanh";
String caption2 = "Tac";
String Loai1, Loai2, L1="0", L2="0";
char ch;
int soLuong = 0, Chanh = 0, Tac = 0;

void ResetBoard( uint8_t time){
  wdt_enable(time);
  while(1){}
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(title);

  Serial.begin(9600);
  sentData.begin(9600);
  
  delay(1000); // allow load cell and hx711 to settle
               // tare procedure
  for (int ii = 0; ii < int(avg_size); ii++) {
    delay(10);
    x0 += hx711.read();
  }
  x0 /= long(avg_size);
  Serial.println(thuTai);

  lcd.setCursor(0, 2);
  lcd.print(thuTai);
  lcd.setCursor(2, 3);
  lcd.print(dinhMuc);

  pinMode(reset, INPUT_PULLUP);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  // De hieu chuan thi khoi luong vao phai bang y1
  int ii = 1;
  while (true) {
    if (hx711.read() < x0 + 10000) {
    } else {
      ii++;
      delay(2000);
      for (int jj = 0; jj < int(avg_size); jj++) {
        x1 += hx711.read();
      }
      x1 /= long(avg_size);
      break;
    }
  }
  Serial.println(hieuChuan);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(title);
  lcd.setCursor(0, 2);
  lcd.print(hieuChuan);
  delay(1000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print(title);
  khoiLuongLCD();
  soLuongLCD();
  

  // averaging reading
  long reading = 0;
  for (int jj = 0; jj < int(avg_size); jj++) {
    reading += hx711.read();
  }
  reading /= long(avg_size);
  // calculating mass based on calibration and linear fit
  float ratio_1 = (float) (reading - x0);
  float ratio_2 = (float) (x1 - x0);
  float ratio = ratio_1 / ratio_2;
  mass = (y1 * ratio)*3.6;

  lcd.setCursor(12, 1);
  lcd.print(mass);

  if (digitalRead(4) == 0) {
    tiemcan++;
    if(tiemcan==1){
      soLuong++;
    }

    XamXi++;
    if(XamXi==3){
      if(mass>50){
        Chanh++;
        klg += mass;
        L1 = String(Chanh);
        Loai1 = L1 + "," +  L2 + ":" + klg;
        Serial.println(Loai1);
        sentData.println(Loai1);

        soLuong = Chanh + Tac;
      }
      
      if(mass<=50 && mass>0){
        Tac++;
        klg += mass;
        L2 = String(Tac);
        Loai2 = L1 + "," + L2 + ":" + klg;
        Serial.println(Loai2);
        sentData.println(Loai2);
        
        soLuong = Chanh + Tac;
      }
    }
  }

  if(digitalRead(4) == 1){
    tiemcan=0;
    XamXi=0;
  }

  soLuongLCD();
  showChanh();
  showTac();
  

  if(digitalRead(reset)==LOW){
    Chanh = 0;
    Tac = 0;
    soLuong = 0;
    sentData.println("0,0:0");
  }
}


void showChanh(){
  lcd.setCursor(2, 3);
  lcd.print(caption1);
  lcd.setCursor(9,3);
  lcd.print(Chanh);
 
}

void showTac(){
  lcd.setCursor(12, 3);
  lcd.print(caption2);
  lcd.setCursor(17,3);
  lcd.print(Tac);

}

void khoiLuongLCD(){
  lcd.setCursor(0, 1);
  lcd.print("Khoi luong: ");
  lcd.setCursor(19,1);
  lcd.print("g");
}
void soLuongLCD() {
  lcd.setCursor(0, 2);
  lcd.print("So luong: ");
  lcd.setCursor(12, 2);
  lcd.print(soLuong);
  lcd.setCursor(16, 2);
  lcd.print("trai");
}
