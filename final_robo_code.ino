#include <DigitalIO.h>
#include <PsxControllerHwSpi.h>
#include <Servo.h>
const byte PIN_PS2_ATT = 10;
PsxControllerHwSpi<PIN_PS2_ATT> psx;
boolean connected = false;
byte lxOld, lyOld, rxOld, ryOld;
PsxButtons oldButtons;
Servo servoMotor; // Create a Servo object
Servo servomotor2; // create object 2
int motorPin = 3; // Connect ESC signal wire to pin 3
void setup() {
delay(300);
Serial.begin(115200);
// Attach servo to its pin
servoMotor.attach(2); // Servo connected to pin 2
servomotor2.attach(3); // servo connected to pin 3
// Initialize ESC (Electronic Speed Controller)
pinMode(motorPin, OUTPUT);
digitalWrite(motorPin, LOW); // Ensure the ESC is off initially
delay(1000); // Wait for ESC to initialize
}
void loop() {
if (!connected) {
config();
} else {
if (!psx.read()) {
Serial.println("Disconnected");
connected = false;
} else {
readController();
}
}
delay(16);
}
void readController() {
PsxButtons buttons = psx.getButtonWord();
if (oldButtons != buttons) {
bool buttonState[16];
oldButtons = buttons;
for (int i = 0; i < 16; i++) {
buttonState[i] = (buttons & 1);
buttons >>= 1;
}
handleButtonChange(buttonState);
}
byte lx, ly;
psx.getLeftAnalog(lx, ly);
if (lx != lxOld || ly != lyOld) {
handleLeftAnalogChange(lx, ly);
lxOld = lx;
lyOld = ly;
}
byte rx, ry;
psx.getRightAnalog(rx, ry);
if (rx != rxOld || ry != ryOld) {
handleRightAnalogChange(rx, ry);
rxOld = rx;
ryOld = ry;
}
// Print joystick values to Serial Monitor for debugging
Serial.print("Left Analog: ");
Serial.print("X = "); Serial.print(lx); Serial.print(", Y = "); Serial.println(ly);
Serial.print("Right Analog: ");
Serial.print("X = "); Serial.print(rx); Serial.print(", Y = "); Serial.println(ry);
Serial.println();
}
void handleButtonChange(bool buttonState[]) {
// Add code to handle button presses here
}
void handleLeftAnalogChange(byte lx, byte ly) {
// Add code to handle left analog stick movements here
int servoAngle2 = map(lx, 0, 255, 0, 180);
servoMotor.write(servoAngle2);
}
void handleRightAnalogChange(byte rx, byte ry) {
// Add code to handle right analog stick movements here
int servoAngle = map(rx, 0, 255, 0, 180);
servomotor2.write(servoAngle);
int throttle = map(ry, 0, 255, 1000, 2000);
throttle = constrain(throttle, 1000, 2000);
analogWrite(motorPin, throttle);
}
void config() {
if (psx.begin()) {
Serial.println("Connected");
delay(300);
if (!psx.enterConfigMode()) {
Serial.println("Cannot enter config mode");
} else {
if (!psx.enableAnalogSticks()) {
Serial.println("Cannot enable analog sticks");
}
if (!psx.enableAnalogButtons()) {
Serial.println("Cannot enable analog buttons");
}
if (!psx.exitConfigMode()) {
Serial.println("Cannot exit config mode");
}
}
connected = true;
}
}
