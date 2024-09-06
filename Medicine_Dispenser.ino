#include <WiFi.h>
#include <ESP32Firebase.h>
#include <AccelStepper.h>

#define WIFI_SSID "Pixel_3"
#define WIFI_PASSWORD "12345678"
#define REFERENCE_URL "https://medicine-dispenser-2cf19-default-rtdb.asia-southeast1.firebasedatabase.app/"  

Firebase firebase(REFERENCE_URL);

#define LIMIT_SWITCH_X_PIN 14
#define LIMIT_SWITCH_Y_PIN 15
#define STEP_PIN_X 2
#define DIR_PIN_X 4
#define STEP_PIN_Y 5
#define DIR_PIN_Y 18

AccelStepper stepperX(1, STEP_PIN_X, DIR_PIN_X);
AccelStepper stepperY(1, STEP_PIN_Y, DIR_PIN_Y);

enum State {IDLE, FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5, FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10, HOMING};
State currentState = IDLE;
unsigned long lastFirebaseCheck = 0;
bool homeCompleted = false;
bool isRunningFunction = false; // Flag to track if a function is currently running

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(LIMIT_SWITCH_X_PIN, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_Y_PIN, INPUT_PULLUP);

  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(500);
  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(500);

  homePosition();
}

void loop() {
  unsigned long currentMillis = millis();

  // Only check Firebase when we are not performing other operations
  if (currentState == IDLE && !isRunningFunction && currentMillis - lastFirebaseCheck >= 1000) {
    lastFirebaseCheck = currentMillis;

    // Fetch the button value from Firebase
    String code = firebase.getString("buttonClicks/clickedButton");
    Serial.print("Firebase value: ");
    Serial.println(code);

    // Set the current state based on the Firebase value
    if (code == "btn1") {
      currentState = FUNCTION1;
    } else if (code == "btn2") {
      currentState = FUNCTION2;
    } else if (code == "btn3") {
      currentState = FUNCTION3;
    } else if (code == "btn4") {
      currentState = FUNCTION4;
    } else if (code == "btn5") {
      currentState = FUNCTION5;
    } else if (code == "btn6") {
      currentState = FUNCTION6;
    } else if (code == "btn7") {
      currentState = FUNCTION7;
    } else if (code == "btn8") {
      currentState = FUNCTION8;
    } else if (code == "btn9") {
      currentState = FUNCTION9;
    } else if (code == "btn10") {
      currentState = FUNCTION10;
    } else {
      currentState = IDLE;
    }
  }

  // State machine to handle different functions
  switch (currentState) {
    case FUNCTION1: runFunction(function1); break;
    case FUNCTION2: runFunction(function2); break;
    case FUNCTION3: runFunction(function3); break;
    case FUNCTION4: runFunction(function4); break;
    case FUNCTION5: runFunction(function5); break;
    case FUNCTION6: runFunction(function6); break;
    case FUNCTION7: runFunction(function7); break;
    case FUNCTION8: runFunction(function8); break;
    case FUNCTION9: runFunction(function9); break;
    case FUNCTION10: runFunction(function10); break;
    case HOMING: 
      if (!homeCompleted) {
        homePosition();
      }
      break;
    default: break;
  }
}

// A function to wrap function execution and handle the running state
void runFunction(void (*func)()) {
  if (!isRunningFunction) {
    isRunningFunction = true; // Set the flag before running a function
    func();
    isRunningFunction = false; // Reset the flag after the function is complete
    currentState = IDLE; // Return to IDLE state after function execution
  }
}

void homePosition() {
  if (digitalRead(LIMIT_SWITCH_X_PIN) == HIGH) {
    stepperX.setSpeed(-500);
    stepperX.runSpeed();
  } else {
    stepperX.setCurrentPosition(0);
  }

  if (digitalRead(LIMIT_SWITCH_Y_PIN) == HIGH) {
    stepperY.setSpeed(-500);
    stepperY.runSpeed();
  } else {
    stepperY.setCurrentPosition(0);
  }

  if (digitalRead(LIMIT_SWITCH_X_PIN) == LOW && digitalRead(LIMIT_SWITCH_Y_PIN) == LOW) {
    homeCompleted = true;
    currentState = IDLE; // Go back to idle state after homing
    Serial.println("Homing complete");
  }
}

bool isAtHomePosition() {
  return (digitalRead(LIMIT_SWITCH_X_PIN) == LOW && digitalRead(LIMIT_SWITCH_Y_PIN) == LOW);
}

void function1() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 1 is running.");
  stepperX.moveTo(1000);
  stepperY.moveTo(500);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function2() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 2 is running.");
  stepperX.moveTo(500);
  stepperY.moveTo(1000);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function3() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 3 is running.");
  stepperX.moveTo(1500);
  stepperY.moveTo(750);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function4() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 4 is running.");
  stepperX.moveTo(750);
  stepperY.moveTo(1500);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function5() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 5 is running.");
  stepperX.moveTo(2000);
  stepperY.moveTo(1000);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function6() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 6 is running.");
  stepperX.moveTo(1000);
  stepperY.moveTo(2000);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function7() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 7 is running.");
  stepperX.moveTo(2500);
  stepperY.moveTo(1250);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function8() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 8 is running.");
  stepperX.moveTo(1250);
  stepperY.moveTo(2500);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function9() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 9 is running.");
  stepperX.moveTo(3000);
  stepperY.moveTo(1500);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}

void function10() {
  if (!isAtHomePosition()) {
    Serial.println("End effector is not at home position. Homing now...");
    homePosition();
  }

  Serial.println("Function 10 is running.");
  stepperX.moveTo(1500);
  stepperY.moveTo(3000);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }

  homePosition();

  // Send "btn0" to Firebase
  firebase.setString("buttonClicks/clickedButton", "btn0");
}
