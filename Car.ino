#define MOTOR1_A 2
#define MOTOR1_B 3  // ШИМ!
#define MOTOR2_A 4
#define MOTOR2_B 5  // ШИМ!
#define MOTOR3_A 7
#define MOTOR3_B 6  // ШИМ!
#define MOTOR4_A 8
#define MOTOR4_B 9  // ШИМ!

#include <AccelMotor.h>
AccelMotor motorBL(DRIVER2WIRE, MOTOR1_A, MOTOR1_B, HIGH);
AccelMotor motorFL(DRIVER2WIRE, MOTOR2_A, MOTOR2_B, HIGH);
AccelMotor motorFR(DRIVER2WIRE, MOTOR3_A, MOTOR3_B, HIGH);
AccelMotor motorBR(DRIVER2WIRE, MOTOR4_A, MOTOR4_B, HIGH);

#include <Pixy2.h>
Pixy2 pixy;

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

void loop() {
  int8_t res;

  res = pixy.line.getAllFeatures();

  if (res&LINE_BARCODE) {
    if (pixy.line.barcodes->m_code==0) {
      pixy.setLED(255, 0, 0);
    }
  } else {
    pixy.setLED(255, 255, 255);
  }
}
