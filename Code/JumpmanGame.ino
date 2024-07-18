/*
This code is for Tinkercad .
If you want to do this irl then you have to change this according to you.

delay(max(150 - score * 20, 2));\\Delay
Serial.println(max(150 - score * 20, 2));\\Delay

I made this code in my first year so I aplogise if it is bit messy.

-----------------------------------
-----------------------------------
||		Rock-Man game            ||
||	Made By KARTIK KHANDELWAL    ||
||        # Original             ||
-----------------------------------
-----------------------------------


*/

#include <Adafruit_LiquidCrystal.h>
#include <Wire.h>
#include <Keypad.h>

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int y = 0;
byte rock[] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100
};

byte pause_symbol[] = {
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110
};

byte player[] = {
  B00100,
  B00100,
  B11111,
  B10101,
  B10101,
  B00100,
  B01010,
  B10001
};

byte blank_space[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

int i;
int arr[2][14];
int score = 0;
int prev = 1;
int high_score=0;
void check_for_char(int p) {
    char customKey = customKeypad.getKey();
    if (customKey) {
        if (customKey == '2') {
            lcd.setCursor(0, 0);
            lcd.write(byte(2));
            if (prev == 0) {
                arr[1][0]--;
                arr[0][0]++;
                lcd.setCursor(0, 1);
                lcd.write(byte(0));
                prev = 1;
            }
            else if (prev == -1) {
                arr[0][0]++;
                prev = 1;
            }
        }
        else if (customKey == '5') {
            lcd.setCursor(0, 1);
            lcd.write(byte(2));
            if (prev == 1) {
                arr[1][0]++;
                arr[0][0]--;
                prev = 0;
                lcd.setCursor(0, 0);
                lcd.write(byte(0));
              	lcd.setCursor(0, 0);
                lcd.write(byte(0));
            }
            else if (prev == -1) {
                arr[1][0]++;
                prev = 0;
            }
        }
        else if (customKey == '#') {
            arr[0][0] = 2;
            check_for_end();
            startGame();
        }
        else if (customKey == 'A') {
            pause();
        }
    }
}

void pause() {
    int Continue = 0;
    lcd.setCursor(14, 1);
    lcd.print("P");
    while (!Continue) {
        char customKey = customKeypad.getKey();
        if (customKey == 'A') {
            lcd.setCursor(14, 1);
            lcd.write(byte(0));
            Continue = 1;
        }
    }
}

void game_over() {
    lcd.clear();
    lcd.setCursor(5, 0);
    Serial.println("Game Over");
    lcd.print("GAME");
    lcd.setCursor(5, 1);
    lcd.print("OVER");
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("YOUR SCORE: ");
    Serial.print("Game Over!! Your score: ");
    Serial.println(score);
    lcd.print(score);
  	lcd.setCursor(1,1);
  	lcd.print("HIGH SCORE: ");
  	lcd.print(highScore());
    delay(2000);
}

int highScore(){
  	high_score=max(high_score,score);
  	return high_score;
}
int check_for_end() {
    if (arr[0][0] >= 2 || arr[1][0] >= 2) {
        game_over();
        lcd.clear();
        y = 1;
        Serial.println("y=1");
        score = 0;
        return 1;
    }
    else {
        return 0;
    }
}


void moveTerrain(int p) {
    for (int i = 13; i >= 0; i--) {
        lcd.setCursor(i, p);
        lcd.write(byte(1));
        arr[p][i]++;
        check_for_char(p);
        if (check_for_end()) {
            break;
        }
        if (i <= 8) {
            lcd.setCursor(i + 5, p);
            lcd.write(byte(0));
            arr[p][i + 5]--;
            check_for_char(p);
            if (check_for_end()) {
                break;
            }
        }
        if (i == 0) {
            for (int j = 4; j >= 0; j--) {
                lcd.setCursor(j, p);
                lcd.write(byte(0));
                arr[p][j]--;
                check_for_char(p);
                if (check_for_end()) {
                    break;
                }
                //delay(150);
                delay(max(150 - score * 20, 2));
                Serial.println(max(150 - score * 20, 2));
            }
            score++;
            lcd.setCursor(14, 0);
            lcd.print(score);
        }
        delay(max(150 - score * 20, 2));
        Serial.println(max(150 - score * 20, 2));
        //delay(100);
    }
}

int start;
void check_for_star() {
    char customKey = customKeypad.getKey();
    if (customKey) {
        if (customKey == '*') {
            Serial.println("star is detected");
            start = 1;
        }
    }
}

void print_3_2_1() {
    lcd.setCursor(7, 0);
    lcd.print("3");
    delay(300);
    lcd.setCursor(7, 0);
    lcd.print("2");
    delay(300);
    lcd.setCursor(7, 0);
    lcd.print("1");
    delay(300);
    lcd.setCursor(7, 0);
    lcd.write(byte(0));
    delay(300);
}

void startGame() {
    Serial.println("Start The Game");
    y = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 14; j++) {
            arr[i][j] = 0;
        }
    }
    arr[0][0] = 1;
    lcd.clear();
    start = 0;
    lcd.setCursor(0, 0);
    lcd.print("Press * To Start");
    while (!start) {
        check_for_star();
    }
    lcd.clear();
    print_3_2_1();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(2));
    lcd.setCursor(14, 0);
    lcd.print("0");
    while (y == 0) {
        int up_or_down = random(2);
        moveTerrain(up_or_down);
        if (y == 1) {
            Serial.println("y = 1 is detected");
            break;
        }

    }
}


void setup() {
    lcd.begin(16, 2);
    Serial.begin(9600);
    lcd.setCursor(4, 0);
    lcd.print("JUMPMAN");
    lcd.setCursor(6, 1);
    lcd.print("GAME");
    delay(1500);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Made by -");
    delay(1000);
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("KARTIK");
    lcd.setCursor(3, 1);
    lcd.print("KHANDELWAL");
    lcd.setCursor(0, 0);
    lcd.createChar(0, blank_space);
    lcd.createChar(1, rock);
    lcd.createChar(2, player);
    lcd.createChar(3, pause_symbol);
    delay(1000);
    lcd.clear();
}

void loop() {
    startGame();
}
