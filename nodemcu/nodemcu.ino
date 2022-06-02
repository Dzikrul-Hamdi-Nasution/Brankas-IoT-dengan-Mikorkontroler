#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define FIREBASE_HOST "brankas-online-default-rtdb.firebaseio.com"
#define WIFI_SSID "Server Project"
#define WIFI_PASSWORD "Master75wew"
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


String data_buka, waktu_riwayat;
int buka = 16, riwayat_bobol = 1, riwayat_masuk = 3;
int randNumber, kunci = 0;


void setup () {
  Serial.begin(9600);
  pinMode(buka, OUTPUT);
  pinMode(riwayat_masuk, INPUT);
  pinMode(riwayat_bobol, INPUT);
  digitalWrite(buka, LOW);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Firebase.begin(FIREBASE_HOST);
  timeClient.begin();
  delay(1000);




}

void loop () {

  cek_database();


  if (data_buka == "1") {
    digitalWrite(buka, HIGH);
    delay(2000);
    Firebase.setString("data_buka", "0");
  }
  else {
    digitalWrite(buka, LOW);
  }


  if (digitalRead(riwayat_masuk) == HIGH) {
    if (kunci == 0) {
      cek_waktu();
      randNumber = random(3000);
      String alamat = String(randNumber);
      Firebase.setString("riwayat/" + alamat + "/waktu", waktu_riwayat);
      kunci = 1;
      delay(3000);
    }
  }
  if (digitalRead(riwayat_masuk) == LOW) {
    kunci = 0;
  }

  

  if (digitalRead(riwayat_bobol) == HIGH) {
    if (kunci == 0) {
      cek_waktu();
      randNumber = random(3000);
      String alamat = String(randNumber);
      Firebase.setString("status", "ALARM");
      kunci = 1;
      delay(3000);
    }
  }
  if (digitalRead(riwayat_bobol) == LOW) {
    kunci = 0;
    Firebase.setString("status", "SAFE");
  }
  





}





void cek_database() {

  data_buka = Firebase.getString("data_buka");

}


void cek_waktu() {
  timeClient.update();
  /*
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  */
  int waktu_kalibrasi = timeClient.getHours() + 6;
  waktu_riwayat = String(daysOfTheWeek[timeClient.getDay()]) + "," + String(waktu_kalibrasi) + ":" + String(timeClient.getMinutes()) + ":" + String(timeClient.getSeconds());
}

