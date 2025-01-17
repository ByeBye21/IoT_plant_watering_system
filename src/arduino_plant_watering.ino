/*
YOUNES RAHEBI - B221210588
UMUT DIREK - B221210090
*/

#define BLYNK_TEMPLATE_ID "TMPL6L-yKXulz"
#define BLYNK_TEMPLATE_NAME "Plant Watering"
#define BLYNK_PRINT Serial
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//LCD ekranını başlat
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "nI-Lmdvd6qb5WM2XcLoFI4q9nZUI1Dr-"; //auth tokeni
char ssid[] = "FiberHGW_ZTK62X_2.4GHz";     //wifi adı
char pass[] = "dEdxYf3uN4K9";  //wifi şifresi

BlynkTimer zamanlayici;
bool role = 0;

//bileşen pinlerini tanımla
#define nem_sensoru A0
#define su_pompasi D3

void setup() {
  Serial.begin(9600);
  pinMode(su_pompasi, OUTPUT);
  digitalWrite(su_pompasi, HIGH);
  lcd.init();
  lcd.backlight();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.setCursor(1, 0);
  lcd.print("Sistem Yukleniyor");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();

  //fonksiyonu çağır
  zamanlayici.setInterval(100L, toprakNemSensoru);
}

//buton değerini al
BLYNK_WRITE(V1) {
  role = param.asInt();

  if (role == 1) {
    digitalWrite(su_pompasi, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor ACILDI ");
  } else {
    digitalWrite(su_pompasi, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor KAPANDI");
  }
}

//toprak nem değerlerini al
void toprakNemSensoru() {
  int deger = analogRead(nem_sensoru);
  deger = map(deger, 0, 1024, 0, 100);
  deger = (deger - 100) * -1;

  Blynk.virtualWrite(V0, deger);
  lcd.setCursor(0, 0);
  lcd.print("Nem :");
  lcd.print(deger);
  lcd.print(" ");
}

void loop() {
  Blynk.run(); //Blynk kütüphanesini çalıştır
  zamanlayici.run(); //Blynk zamanlayıcısını çalıştır
} 
