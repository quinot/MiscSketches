// TIL311+74HC595 test
// TIL311 Fritzing part: https://forum.fritzing.org/t/til311-hexadecimal-display-with-logic/1023

#define SH_CLK PIN_PB0
#define ST_CLK PIN_PB1
#define DATA PIN_PB2
#define BLANK PIN_PB3

void setup() {
  pinMode(SH_CLK, OUTPUT);
  pinMode(ST_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(BLANK, OUTPUT);
  digitalWrite(BLANK, 0);
}

int step = 0;
#define STEP_DURATION 256

#define STEP(val, slow) do {                     \
  digitalWrite(ST_CLK, LOW);               \
  shiftOut(DATA, SH_CLK, MSBFIRST, (val)); \
  digitalWrite(ST_CLK, HIGH);              \
  delay(STEP_DURATION / (2- slow));                              \
} while (0)

void loop() {
  unsigned char j;

  for (j = 0; j < 0x10; j++)
    STEP(j, 1);

  for (; j; j--) {
    STEP(j - 1, 0);
    for (unsigned int k = 0; k < 2; k++) {
      digitalWrite(BLANK, 1);
      delay(STEP_DURATION / 4);
      digitalWrite(BLANK, 0);
      delay(STEP_DURATION / 4);
    }
  }
}
