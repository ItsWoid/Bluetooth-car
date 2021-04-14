#define MOTOR_MAX 255                 // максимальный сигнал на мотор (max 255)
#define JOY_MAX 40                    // рабочий ход джойстика (из приложения)
#define minDuty 0                     // скорость, при которой мотор должен начинать крутится (обычно 25-50)

#define MOTOR1_A 2
#define MOTOR1_B 3  // ШИМ!
#define MOTOR2_A 4
#define MOTOR2_B 5  // ШИМ!
#define MOTOR3_A 7
#define MOTOR3_B 6  // ШИМ!
#define MOTOR4_A 8
#define MOTOR4_B 9  // ШИМ!
#define BT_TX 13
#define BT_RX 12

#include "GyverMotor.h"
GMotor motorBL(DRIVER2WIRE, MOTOR1_A, MOTOR1_B, HIGH);
GMotor motorFL(DRIVER2WIRE, MOTOR2_B, MOTOR2_A, HIGH);
GMotor motorFR(DRIVER2WIRE, MOTOR3_A, MOTOR3_B, HIGH);
GMotor motorBR(DRIVER2WIRE, MOTOR4_A, MOTOR4_B, HIGH);

boolean doneParsing, startParsing;
int dataX, dataY;
String string_convert;

void setup() {
  TCCR2B = 0b00000100;
  TCCR2A = 0b00000011;
  TCCR1A = 0b00000001;
  TCCR1B = 0b00001011;

  Serial.begin(9600);

  // минимальный сигнал на мотор
  motorFR.setMinDuty(30);
  motorBR.setMinDuty(30);
  motorFL.setMinDuty(30);
  motorBL.setMinDuty(30);

  // режим мотора в АВТО
  motorFR.setMode(AUTO);
  motorBR.setMode(AUTO);
  motorFL.setMode(AUTO);
  motorBL.setMode(AUTO);
}

void loop() {
  parsing();         // функция парсинга
  if (doneParsing) { // если получены данные
    doneParsing = false;

    int joystickX = map((dataX), -JOY_MAX, JOY_MAX, -MOTOR_MAX / 2, MOTOR_MAX / 2); // сигнал по Х
    int joystickY = map((dataY), -JOY_MAX, JOY_MAX, -MOTOR_MAX, MOTOR_MAX);         // сигнал по Y

    int dutyFR = joystickY + joystickX;
    int dutyFL = joystickY - joystickX;
    int dutyBR = joystickY - joystickX;
    int dutyBL = joystickY + joystickX;

    dutyFR = constrain(dutyFR, -MOTOR_MAX, MOTOR_MAX);
    dutyFL = constrain(dutyFL, -MOTOR_MAX, MOTOR_MAX);
    dutyBR = constrain(dutyBR, -MOTOR_MAX, MOTOR_MAX);
    dutyBL = constrain(dutyBL, -MOTOR_MAX, MOTOR_MAX);

    motorFR.smoothTick(dutyFR);
    motorBR.smoothTick(dutyBR);
    motorFL.smoothTick(dutyFL);
    motorBL.smoothTick(dutyBL);
  }
}

void parsing() {
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();
    if (startParsing) {
      if (incomingChar == ' ') {
        dataX = string_convert.toInt();
        string_convert = "";
      }
      else if (incomingChar == ';') {
        dataY = string_convert.toInt();
        string_convert = "";
        startParsing = false;
        doneParsing = true;
      } else {
        string_convert += incomingChar;
      }
    }
    if (incomingChar == '$') {
      startParsing = true;
    }
  }
}
