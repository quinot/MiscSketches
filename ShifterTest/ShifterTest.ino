// 74HC595 test
// Resistor network Fritzing part: https://forum.fritzing.org/t/resistor-array/2079/45

#define SH_CLK PIN_PB0
#define ST_CLK PIN_PB1
#define DATA PIN_PB2

void setup() {
  pinMode(SH_CLK, OUTPUT);
  pinMode(ST_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

}

int step = 0;

#define STEP(val) do {                     \
  digitalWrite(ST_CLK, LOW);               \
  shiftOut(DATA, SH_CLK, MSBFIRST, (val)); \
  digitalWrite(ST_CLK, HIGH);              \
  delay(1000);                              \
} while (0)

void loop() {
   STEP(0xff); STEP(0); STEP(0xff); STEP (0); STEP(0xff);
   delay(1000);

   for (unsigned char j = 1; j; j <<= 1)
    STEP(j);

   for (unsigned char j = 1; j; j <<= 1)
    STEP(~j);

   for (unsigned char j = 0xff; j; j <<= 1)
    STEP(j);

   for (unsigned char j = 0xff; j; j <<= 1)
    STEP(~j);

   for (unsigned char j = 0xff; j; j >>= 1)
    STEP(j);

   for (unsigned char j = 0x80; j; j >>= 1)
    STEP(j);
}
