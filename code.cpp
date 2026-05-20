#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo
Servo myServo;

// Pins
const int ledPin = 12;
const int buzzerPin = 11;

// Password
String password = "1234";
String input = "";

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  myServo.attach(10);
  myServo.write(0); // locked

  lcd.init();
  lcd.backlight();

  showHome();
}

void loop() {
  char key = keypad.getKey();

  if (key) {

    if (key == 'D') {
      checkPassword();
    }

    else if (key == 'C') {
      input = "";
      showMsg("Cleared");
    }

    else if (key == '*' || key == '#') {
      return;
    }

    else {
      input += key;
      showInput();
    }
  }
}

// ---------- HOME SCREEN ----------
void showHome() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

// ---------- SHOW * INPUT ----------
void showInput() {
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);

  for (int i = 0; i < input.length(); i++) {
    lcd.print("*");
  }
}

// ---------- MESSAGE ----------
void showMsg(String msg) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);
  delay(800);
  showHome();
}

// ---------- PASSWORD CHECK ----------
void checkPassword() {
  lcd.clear();

  if (input == password) {

    lcd.setCursor(0,0);
    lcd.print("Access Granted");

    myServo.write(90);
    delay(2000);
    myServo.write(0);
  }
  else {

    lcd.setCursor(0,0);
    lcd.print("Wrong Password");

    // SAFE ALERT (no overload)
    for (int i = 0; i < 5; i++) {

      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000);
      delay(200);

      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      delay(200);
    }
  }

  input = "";
  delay(800);
  showHome();
}