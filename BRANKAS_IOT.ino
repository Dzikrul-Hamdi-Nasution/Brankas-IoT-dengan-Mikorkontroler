#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library modul I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
String read_rfid;
String ID_1 = "4167b42c82c80";
int buzzer = 5, relay = 6, sensor = 4;
int kunci = 0, perintah = A0, riwayat_masuk = 2, riwayat_bobol = 3;
int isi_perintah, kunci2 = 0, kunci3 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(buzzer, OUTPUT);
  pinMode(riwayat_masuk, OUTPUT);
  pinMode(riwayat_bobol, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);
  Serial.println("Dekatkan Tag RFID (Dapat berupa kartu atau gantungan kunci) ke RFID reader");
  Serial.println();
  lcd.backlight();
  lcd.init();
  lcd.print("BRANKAS IOT");
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  digitalWrite(relay, HIGH);
  digitalWrite(riwayat_masuk, LOW);
  digitalWrite(riwayat_bobol, LOW);

}




void loop() {
  cek_kartu();
  if (read_rfid == ID_1) {
    kunci = 2;
    lcd.setCursor(0, 1);
    lcd.print("TERBUKA ");
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    digitalWrite(relay, LOW);
    read_rfid = "";
    digitalWrite(riwayat_masuk, HIGH);
    delay(2500);
    digitalWrite(riwayat_masuk, LOW);

  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("TAP CARD");
    digitalWrite(relay, HIGH);
  }

  if (digitalRead(sensor) == HIGH) {
    if (kunci == 0) {
      if (kunci2 == 0) {
        kunci2 = 2;
        digitalWrite(riwayat_bobol, HIGH);

      }
      lcd.setCursor(10, 1);
      lcd.print("ALARM");
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(buzzer, HIGH);

    }
  }
  if (digitalRead(sensor) == LOW) {
    lcd.setCursor(10, 1);
    lcd.print("SAFE ");
    digitalWrite(riwayat_bobol, LOW);
    kunci = 0;
    kunci2 = 0;
    digitalWrite(buzzer, HIGH);
  }

  isi_perintah = analogRead(perintah);
  if (isi_perintah > 600) {
    kunci = 2;
    lcd.setCursor(0, 1);
    lcd.print("TERBUKA ");
    if (kunci3 == 0) {
      kunci3 = 2;
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      digitalWrite(relay, LOW);
      digitalWrite(riwayat_masuk, HIGH);
      delay(2500);
      digitalWrite(relay, HIGH);
      digitalWrite(riwayat_masuk, LOW);
    }
  }
  else {
    kunci3 = 0;
  }

}




void cek_kartu() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  dump_byte_array(rfid.uid.uidByte, rfid.uid.size);
  //Serial.println(read_rfid);
}


void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }
}
