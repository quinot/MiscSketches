// TI 99/4A joystick tester
// ========================

// This sketch allows testing the correct operation of a pair of TI 99/4A joysticks
// (or, as the case may be, of a pair of Atari joysticks connected using a suitable
// adapter circuit).

// Pinout:
// Arduino | function     | Joysticks DE-9 plug
// --------+--------------+--------------------
//       2 | Joy 1 select | 7
//       3 | Joy 2 select | 2
//       4 | up    input  | 3
//       5 | down  input  | 8
//       6 | left  input  | 5
//       7 | right input  | 9
//       8 | fire  input  | 4

enum joystate {
  up    = 1,
  down  = 2,
  left  = 4,
  right = 8,
  fire  = 16
};

#define PIN_J1    2
#define PIN_J2    3
#define PIN_UP    4
#define PIN_DOWN  5
#define PIN_LEFT  6
#define PIN_RIGHT 7
#define PIN_FIRE  8

joystate poll(short j_pin) {
  digitalWrite(j_pin, 0);
  joystate res = (joystate) up    * !digitalRead(PIN_UP)
                          + down  * !digitalRead(PIN_DOWN)
                          + left  * !digitalRead(PIN_LEFT)
                          + right * !digitalRead(PIN_RIGHT)
                          + fire  * !digitalRead(PIN_FIRE);
  digitalWrite(j_pin, 1);
  return res;

}

#define SETUP
void setup() {
  for (short j = PIN_J1; j <= PIN_J2; j++) {
    pinMode(j, OUTPUT);
    digitalWrite(j, 1);
  }
  for (short j = PIN_UP; j <= PIN_FIRE; j++)
    pinMode(j, INPUT_PULLUP);
  Serial.begin(115200);

  Serial.println("Joysticks tester\n");
}

const char joy_chars[] PROGMEM = "*><v^ ";

char joy_char(joystate st) {
  int jidx = 5;
  while (st) {
    st >>= 1;
    jidx--;
  }
  return pgm_read_byte(joy_chars + jidx);
}

void loop() {

  // put your main code here, to run repeatedly:
  static char prev[2] = { 0xff, 0xff };
  joystate st[2] = { poll(PIN_J1), poll(PIN_J2) };
  char cur[2] = { joy_char(st[0]), joy_char(st[1]) };

  if (prev[0] != cur[0] || prev[1] != cur[1]) {
    Serial.print("Joy1: ");
    Serial.print(cur[0]);
    Serial.print(" Joy2: ");
    Serial.println(cur[1]);
    // Serial.write('\r');
  }
  prev[0] = cur[0]; prev[1] = cur[1];
}
