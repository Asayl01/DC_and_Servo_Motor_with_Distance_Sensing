// --- Pin definitions for the motor driver ---
#define ENA 9   // Enable pin for motors (speed control if using PWM)
#define IN1 2   // Motor A direction pin 1
#define IN2 3   // Motor A direction pin 2
#define IN3 4   // Motor B direction pin 1
#define IN4 5   // Motor B direction pin 2

// --- Motor control functions ---
void moveForward() {
  digitalWrite(ENA, HIGH);            // Enable motors
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Motor A forward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // Motor B forward
}

void moveBackward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); // Motor A backward
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // Motor B backward
}

void turnRight() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Motor A forward
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);  // Motor B stop
}

void turnLeft() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);  // Motor A stop
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  // Motor B forward
}

void stopMotors() {
  digitalWrite(ENA, LOW); // Disable motors
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// --- Setup: runs once at start ---
void setup() {
  // Set motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Start serial communication for logging
  Serial.begin(9600);
  Serial.println("--- Motor Test Sequence Started ---");

  // Task 1: Move forward for 30 seconds
  Serial.println("Task 1: Moving forward for 30 seconds...");
  moveForward();
  delay(30000);
  stopMotors();
  delay(800);

  // Task 2: Move backward for 60 seconds
  Serial.println("Task 2: Moving backward for 60 seconds...");
  moveBackward();
  delay(60000);
  stopMotors();
  delay(800);

  // Task 3: Alternate turning right and left for 60 seconds
  Serial.println("Task 3: Alternating right/left turns for 60 seconds...");
  for (int i = 0; i < 30; i++) { // 30 loops x 2 seconds = 60 seconds
    turnRight();
    delay(1000);
    turnLeft();
    delay(1000);
  }
  stopMotors();
  delay(800);

  Serial.println("--- Motor Test Sequence Complete ---");
}

// --- Main loop: empty because all actions are in setup() ---
void loop() {
  // Nothing here, robot stops after setup() tasks
}
