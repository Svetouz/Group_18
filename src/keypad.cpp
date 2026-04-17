#include <keypad.h>

// how many rows and collums
const byte ROWS = 4;
const byte COLS = 4;

// array to represent keys on keypad
char hexaKeys [ROWS][COLS] ={
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

// pin numbers
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// setup: name the keypad, tell the computer wich pins to use and the size of the keypad
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

bool started = false;
int inputCount = 0;
char rawInputs[6];
int sectors[3];

void setup() {
  Serial.begin(9600);
  Serial.println("Press * to start...");
}

void loop(){
  
  char key = keypad.getKey();

  if (key) {

    // wait for * to begin
    if (!started) {
      if (key == '*') {
        started = true;
        inputCount = 0;
        Serial.println("Started! Enter 3 sector numbers (2 digits each):");
      }

    // collect digits
    } else {

      if (key >= '0' && key <= '9') {
        rawInputs[inputCount] = key;
        inputCount++;
        Serial.println(key);

        // prompt what to enter next
        if (inputCount < 6) {
          int sectorNum = (inputCount / 2) + 1;
          int digitNum  = (inputCount % 2) + 1;
          Serial.print("Sector ");
          Serial.print(sectorNum);
          Serial.print(", digit ");
          Serial.print(digitNum);
          Serial.print(": ");
        }

        // all 6 digits received
        if (inputCount == 6) {

          // convert pairs of digits into sector numbers
          for (int i = 0; i < 3; i++) {
            int firstDigit  = rawInputs[i * 2] - '0';
            int secondDigit = rawInputs[i * 2 + 1] - '0';
            sectors[i] = firstDigit * 10 + secondDigit;
          }

          // print the results
          Serial.println("All sectors received!");
          for (int i = 0; i < 3; i++) {
            Serial.print("Sector ");
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.println(sectors[i]);
          }

          // reset for next round
          started = false;
          inputCount = 0;
          Serial.println("Press * to start again...");
        }
      }
    }
  }
}
