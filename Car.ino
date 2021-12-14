#define MAX_SPEED 255

#define MOTOR1_A 2
#define MOTOR1_B 3  // PWM
#define MOTOR2_A 4
#define MOTOR2_B 5  // PWM
#define MOTOR3_A 7
#define MOTOR3_B 6  // PWM
#define MOTOR4_A 8
#define MOTOR4_B 9  // PWM

#include <AccelMotor.h>
AccelMotor motorBL(DRIVER2WIRE, MOTOR1_A, MOTOR1_B, HIGH);
AccelMotor motorFL(DRIVER2WIRE, MOTOR2_A, MOTOR2_B, HIGH);
AccelMotor motorFR(DRIVER2WIRE, MOTOR3_A, MOTOR3_B, HIGH);
AccelMotor motorBR(DRIVER2WIRE, MOTOR4_A, MOTOR4_B, HIGH);

#include <Pixy2.h>
Pixy2 pixy;

#define PS2_DAT A0
#define PS2_CMD A1
#define PS2_SEL A2
#define PS2_CLK A3

#include <PS2X_lib.h>
PS2X ps2x;

void setup() {
  TCCR2B = 0b00000100;  // x64
  TCCR2A = 0b00000011;  // fast pwm

  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00001011;  // x64 fast pwm
  
  pixy.init();

  pixy.setLamp(1, 0);

  pixy.changeProg("line");

  motorFR.setMode(AUTO);
  motorBR.setMode(AUTO);
  motorFL.setMode(AUTO);
  motorBL.setMode(AUTO);
  
}

int currentMode = 0;

void loop() {
  static uint32_t tmr;
  if (millis() - tmr >= 50) {
    tmr += 50;
    bool success = ps2x.read_gamepad(false, 0);
    ps2x.reconfig_gamepad();

    if (success) {
      if (currentMode == 0) {
        int valLX = map(ps2x.Analog(PSS_LX), 0, 256, -MAX_SPEED, MAX_SPEED);
        int valLY = map(ps2x.Analog(PSS_LY), 256, 0, -MAX_SPEED, MAX_SPEED);
        int valRX = map(ps2x.Analog(PSS_RX), 0, 256, -MAX_SPEED, MAX_SPEED);
        int valRY = map(ps2x.Analog(PSS_RY), 256, 0, -MAX_SPEED, MAX_SPEED);

        int dutyFR = valLY + valLX;
        int dutyFL = valLY - valLX;
        int dutyBR = valLY - valLX;
        int dutyBL = valLY + valLX;

        dutyFR += valRY - valRX;
        dutyFL += valRY + valRX;
        dutyBR += valRY - valRX;
        dutyBL += valRY + valRX;

        motorFR.smoothTick(dutyFR);
        motorBR.smoothTick(dutyBR);
        motorFL.smoothTick(dutyFL);
        motorBL.smoothTick(dutyBL);
      } else if (currentMode == 1) { 
        int8_t res;

        res = pixy.line.getAllFeatures();

        motorFR.smoothTick(255);
        motorBR.smoothTick(255);
        motorFL.smoothTick(255);
        motorBL.smoothTick(255);

        if (res&LINE_BARCODE) {
          if (pixy.line.barcodes->m_code==0) {
            pixy.setLED(255, 0, 0);
            stopMotors();
          }
        } else {
          pixy.setLED(255, 255, 255);
        }
      }
    }
  }
}

void stopMotors() {
  motorFR.setSpeed(0);
  motorBR.setSpeed(0);
  motorFL.setSpeed(0);
  motorBL.setSpeed(0);
}
