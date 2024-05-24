// Test behaviour of a CD4027B JK flip-flop

#include <LibPrintf.h>

// Arduino pins        FF1 pins  FF2 pins

// Inputs

#define PIN_SET   3 // FF1:  9   FF2: 7
#define PIN_J     4 // FF1: 10   FF2: 6
#define PIN_K     5 // FF1: 11   FF2: 5
#define PIN_RESET 6 // FF1: 12   FF2: 4
#define PIN_CLOCK 7 // FF1: 13   FF2: 3

// Outputs

#define PIN_Qi    8 // FF1: 14   FF2: 2
#define PIN_Q     9 // FF1: 15   FF2: 1

bool checkOutputs(int q, int qi, bool quiet, char *fail_msg) {
  int actual_q = digitalRead(PIN_Q);
  int actual_qi = digitalRead(PIN_Qi);
  bool pass = (q == actual_q && qi == actual_qi);

  if (!(pass && quiet)) {
    printf("expect Q=%d Qi=%d ", q, qi);
  }

  if (pass)
    return true;

  printf("!! %s: got Q=%d Qi=%d !! ", fail_msg, actual_q, actual_qi);
  return false;
}

bool checkState(int q, bool quiet, char *fail_msg) {
  return checkOutputs(q, !q, quiet, fail_msg);
}

void pulse(int pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
}

void pulseClock() {
  int q, qi;

  digitalWrite(PIN_CLOCK, HIGH);
  delayMicroseconds(1);
  q = digitalRead(PIN_Q);
  qi = digitalRead(PIN_Qi);
  digitalWrite(PIN_CLOCK, LOW);
  delayMicroseconds(1);

  checkOutputs(q, qi, true, "Unexpected change on clock falling edge");
}

void setState(int q) {
  if (q)
    pulse(PIN_SET);
  else
    pulse(PIN_RESET);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(PIN_SET,   OUTPUT);
  pinMode(PIN_J,     OUTPUT);
  pinMode(PIN_K,     OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);

  pinMode(PIN_Qi,    INPUT);
  pinMode(PIN_Q,     INPUT);
}

#define CSI "\x1b["

bool reportResult(bool pass) {
  if (pass)
    printf(CSI "32m" "PASS" CSI "0m" "\n");
  else
    printf(CSI "30;41m" "FAIL" CSI "0m" "\n");
  return pass;
}
bool testTransition(int j, int k, int q, int expected_q) {
  printf("J=%d K=%d Q=%d -> ", j, k, q);
  setState(q);
  if (!checkState(q, true, "Failed to check pre-state"))
    return false;

  digitalWrite(PIN_J, j);
  digitalWrite(PIN_K, k);
  pulseClock();
  
  return reportResult(checkState(expected_q, false, "Unexpected state after clock"));
}

void runTests() {
  printf("\n\n**** Starting test series ****\n\n");

  // Rule 1: J=1 K=X Q=0 => Q=1
  testTransition(1, 0, 0, 1);
  testTransition(1, 1, 0, 1);

  // Rule 2: J=X K=0 Q=1 => Q=1
  testTransition(0, 0, 1, 1);
  testTransition(1, 0, 1, 1);

  // Rule 3: J=0 K=X Q=0 => Q=0
  testTransition(0, 0, 0, 0);
  testTransition(0, 1, 0, 0);

  // Rule 4: J=X K=1 Q=1 => Q=0
  testTransition(0, 1, 1, 0);
  testTransition(1, 1, 1, 0);

  // Rule 5: no output change on clock falling edge

  // Rule 6: SET

  printf("SET -> ");
  setState(1);
  reportResult(checkState(1, false, "Failed to SET state"));

  // Rule 7: RESET

  printf("RESET -> ");
  setState(0);
  reportResult(checkState(0, false, "Failed to RESET state"));

  // Rule 8: SET+RESET

  printf("SET+RESET -> ");
  digitalWrite(PIN_SET, HIGH);
  digitalWrite(PIN_RESET, HIGH);
  delayMicroseconds(1);
  reportResult(checkOutputs(1, 1, false, "Unexpected state on SET+RESET"));

  // Cleanup

  digitalWrite(PIN_SET, LOW);
  digitalWrite(PIN_RESET, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  printf("\n\nPress return to start test...");
  while (!(Serial.available() && Serial.read() == '\n')) ;

  runTests();
}
