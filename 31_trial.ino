
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(10, 9);

#include <String.h>

const  int buttonPin1 = 7;
const  int buttonPin2 = 6;
const  int buttonPin3 = 5;
const  int buttonPin4 = 4;

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;// Give pushbutton a value
int id = 0;
int vote1;
int vote2;
int vote3;
int vote4;
int tot;
int value;
int j;
int y;
float x;

SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  vote1 = EEPROM.read(21);
  vote2 = EEPROM.read(22);
  vote3 = EEPROM.read(23);
  vote4 = EEPROM.read(24);

  pinMode(buttonPin1, INPUT); // Set pushbutton pin as input
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(11, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);

  gprsSerial.begin(9600);
  delay(1000);

  //Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

void loop()                     // run over and over again
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Insert FP");
  //delay(100);
  //lcd.clear();
  //delay(3000);
  //lcd.clear();

  id = getFingerprintID();
  Serial.println("...");



  delay(50);            //don't ned to run this at full speed.
  digitalWrite(buttonPin1, HIGH);
  digitalWrite(buttonPin2, HIGH);
  digitalWrite(buttonPin3, HIGH);
  digitalWrite(buttonPin4, HIGH);

  while (id > 0) {
    buttonState1 = digitalRead(buttonPin1); // Read input from pin 2
    buttonState2 = digitalRead(buttonPin2);
    buttonState3 = digitalRead(buttonPin3);
    buttonState4 = digitalRead(buttonPin4);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FP matched");
    lcd.setCursor(0, 1);
    lcd.print("Cast one vote");
    //delay(2000);
    //lcd.clear();
    value = EEPROM.read(id);

    if (value == 1)
    {
      //Serial.println("you can not vote");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("YOU can not VOTE");
      digitalWrite(11, HIGH);
      delay(2000);
      digitalWrite(11, LOW);
      //delay(2000);
      //lcd.clear();
      break;
    }
    else {
      if (buttonState1 == LOW)
      {

        vote1 = vote1 + 1;
        EEPROM.write(21, vote1);

        EEPROM.write(id, 1);
        result();
        delay(100);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vote taken");


        delay(2000);
        //lcd.clear();
        break;
      } //1 no vote deyoya sesh

      else if (buttonState2 == LOW)
      {
        vote2 = vote2 + 1;
        EEPROM.write(22, vote2);
        EEPROM.write(id, 1);


        result();
        delay(100);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vote taken");


        delay(2000);
        //lcd.clear();
        break;
      }// 2 no vote deoya sesh

      else if (buttonState3 == LOW)
      {
        vote3 = vote3 + 1;
        EEPROM.write(23, vote3);
        EEPROM.write(id, 1);


        result();
        delay(100);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vote taken");

        delay(2000);
        //lcd.clear();
        break;
      } // 3 no vote deyoya sesh

      else if (buttonState4 == LOW)
      {
        vote4 = vote4 + 1;
        EEPROM.write(24, vote4);
        EEPROM.write(id, 1);


        result();
        delay(100);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vote taken");



        delay(2000);
        //lcd.clear();
        break;
      } //4 no vote deyoya sesh

    } // vote deyoyar else()


  } // while loop sesh //
  tot = vote1 + vote2 + vote3 + vote4 ;
  j = EEPROM.read(25);
  x = tot / j ;
  if ( (x == 5) & (tot != 0) & tot % 5 == 0 ) // gsm
  {
    y = j + 1;
    EEPROM.write(25, y);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DONT VOTE");

    //Serial.println("start of the trrasnmition");
    //Serial.println("  ");
    if (gprsSerial.available())
      Serial.write(gprsSerial.read());

    gprsSerial.println("AT");
    delay(1000);

    gprsSerial.println("AT+CPIN?");
    delay(1000);

    gprsSerial.println("AT+CREG?");
    delay(1000);

    gprsSerial.println("AT+CGATT?");
    delay(1000);

    gprsSerial.println("AT+CIPSHUT");
    delay(1000);

    gprsSerial.println("AT+CIPSTATUS");
    delay(2000);

    gprsSerial.println("AT+CIPMUX=0");
    delay(2000);

    ShowSerialData();

    gprsSerial.println("AT+CSTT=\"airtelgprs.com\"");//start task and setting the APN,
    delay(1000);

    ShowSerialData();

    gprsSerial.println("AT+CIICR");//bring up wireless connection
    delay(3000);

    ShowSerialData();

    gprsSerial.println("AT+CIFSR");//get local IP adress
    delay(2000);

    ShowSerialData();

    gprsSerial.println("AT+CIPSPRT=0");
    delay(3000);

    ShowSerialData();

    gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
    delay(6000);

    ShowSerialData();

    gprsSerial.println("AT+CIPSEND");//begin send data to remote server
    delay(4000);
    ShowSerialData();

    String str = "GET https://api.thingspeak.com/update?api_key=FVQNSPY3SC86CP8S&field1=" + String(vote1) + "&field2=" + String(vote2) + "&field3=" + String(vote3) + "&field4=" + String(vote4);
    Serial.println(str);
    gprsSerial.println(str);//begin send data to remote server

    delay(4000);
    ShowSerialData();

    gprsSerial.println((char)26);//sending
    delay(5000);//waitting for reply, important! the time is base on the condition of internet
    gprsSerial.println();

    ShowSerialData();

    gprsSerial.println("AT+CIPSHUT");//close the connection
    delay(100);
    ShowSerialData();


  }// gsm er if sesh

} //void loop sesh //

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      // Serial.println("No finger detected");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return 0;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.clear();
    lcd.println("Not Enrolled");
    delay(2000);
    return 0;
  } else {
    Serial.println("Unknown error");
    return 0;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

void result()
{
  Serial.print("vote_1 ");
  Serial.print(vote1);
  Serial.print("  vote_2 ");
  Serial.println(vote2);
  Serial.print("  vote_3 ");
  Serial.println(vote3);
  Serial.print("  vote_4 ");
  Serial.println(vote4);

}
void ShowSerialData()
{
  while (gprsSerial.available() != 0)
    Serial.write(gprsSerial.read());
  delay(5000);

}
