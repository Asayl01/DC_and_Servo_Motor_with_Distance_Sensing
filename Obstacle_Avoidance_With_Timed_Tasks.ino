#include <Servo.h>

// === Pins ===
#define ENA        9
#define IN1        2
#define IN2        3
#define IN3        4
#define IN4        5
#define TRIG_PIN  12
#define ECHO_PIN  11
#define SERVO_PIN 10
#define LED_PIN   13   // On-board LED for status indication

Servo myServo;

// ===== Servo Actions =====
void servoMark(int angle, int hold_ms=500) {
  // Move to angle with a small shake for confirmation
  myServo.write(angle);
  delay(hold_ms);
  myServo.write(constrain(angle - 10, 0, 180));
  delay(150);
  myServo.write(constrain(angle, 0, 180));
  delay(150);
}

void servoCelebrate() {
  // Quick side-to-side movement (celebration)
  myServo.write(10);   delay(250);
  myServo.write(170);  delay(250);
  myServo.write(10);   delay(250);
  myServo.write(170);  delay(250);
  myServo.write(90);   delay(300);
}

// ===== Motor Movements =====
void moveForward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void turnRight() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  digitalWrite(ENA, LOW);
}

// ===== Ultrasonic Distance =====
long getDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return 999;
  return (long)(duration * 0.034 / 2.0); // cm
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);

  myServo.attach(SERVO_PIN);

  // Wake-up servo movement
  myServo.write(0);   delay(600);
  myServo.write(180); delay(600);
  myServo.write(90);  delay(400);

  // === Scheduled Tasks ===
  Serial.println("Task 1: Moving forward for 30 seconds");
  moveForward();
  delay(30000);
  stopMotors(); delay(400);
  servoMark(45, 500);

  Serial.println("Task 2: Moving backward for 60 seconds");
  moveBackward();
  delay(60000);
  stopMotors(); delay(400);
  servoMark(135, 500);

  Serial.println("Task 3: Alternating right and left turns for 60 seconds");
  for (int i = 0; i < 30; i++) {
    turnRight(); delay(1000);
    turnLeft();  delay(1000);
  }
  stopMotors(); delay(400);
  servoCelebrate();

  Serial.println("Scheduled tasks complete. Switching to obstacle avoidance mode");
}

void loop() {
  long d = getDistance();
  Serial.print("Distance: "); Serial.print(d); Serial.println(" cm");

  if (d <= 15) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Obstacle detected! Stopping and scanning");

    stopMotors(); delay(250);

    // Scan right
    myServo.write(20); delay(350);
    long rightD = getDistance();
    Serial.print("Right distance: "); Serial.println(rightD);

    // Scan left
    myServo.write(160); delay(400);
    long leftD = getDistance();
    Serial.print("Left distance: "); Serial.println(leftD);

    // Back to center
    myServo.write(90); delay(200);

    // Step back then turn
    moveBackward(); delay(300); stopMotors(); delay(100);

    if (rightD > leftD) {
      Serial.println("Turning right for 1.2 seconds");
      turnRight(); delay(1200);
    } else {
      Serial.println("Turning left for 1.2 seconds");
      turnLeft(); delay(1200);
    }
    stopMotors(); delay(150);
    digitalWrite(LED_PIN, LOW);

  } else {
    moveForward();
  }

  delay(60);
}
