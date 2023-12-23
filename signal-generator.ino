/*

  Signal Generator

  Based on Simple Waveform generator with Arduino Due

  See https://docs.arduino.cc/tutorials/due/simple-waveform-generator

*/

#include "waveforms.h"

#define BUTTON_A_PIN  2
#define BUTTON_B_PIN  3

#define POT_PIN      A0

//#define SERIAL_SPEED 9600

/* Sample for the 1Hz signal expressed in microseconds */
#define ONE_SAMPLE 1000000/WAVEFORM_SAMPLE_COUNT

volatile int wa, wb;

int i, potraw, sample, outa, outb;

static void onbuttona();
static void onbuttonb();

void setup() {
  wa = wb = 0;

  /* set the analog output resolution to 12 bit (4096 levels) */
  analogWriteResolution(12);

  /* set the analog input resolution to 12 bit */
  analogReadResolution(12);

  /* Interrupt attached to the button connected to pin 2 */
  attachInterrupt(BUTTON_A_PIN, onbuttona, RISING);

  /* Interrupt attached to the button connected to pin 3 */
  attachInterrupt(BUTTON_B_PIN, onbuttonb, RISING);

#ifdef SERIAL_SPEED
  Serial.begin(SERIAL_SPEED);
#endif

  i = 0;
}

void loop() {
  /* Read the the potentiometer and map the value between the maximum and the minimum sample available. */
  /* 1 Hz is the minimum freq for the complete wave. 170 Hz is the maximum freq for the complete wave.  */
  /* Measured considering the loop and the analogRead() time */
  potraw = analogRead(POT_PIN);
  sample = map(potraw, 0, 4095, 0, ONE_SAMPLE);
  sample = constrain(sample, 0, ONE_SAMPLE);

  outa = waveforms_table[wa][i];
  outb = waveforms_table[wb][i];
  
  /* write the selected waveform on DAC0 */
  analogWrite(DAC0, outa);

  /* write the selected waveform on DAC0 */
  analogWrite(DAC1, outb);

#ifdef SERIAL_SPEED
  //Serial.println(sample);
  Serial.print(outa);
  Serial.print(",");
  Serial.println(outb);
#endif

  i++;
  if (i >= WAVEFORM_SAMPLE_COUNT) {
    i = 0;
  }

  /* Hold the sample value for the sample time */
  delayMicroseconds(sample);
}

/* function hooked to the interrupt on digital pin 2 */
void onbuttona() {
  wa++;
  if (wa >= WAVEFORM_COUNT) {
    wa = 0;
  }
}

/* function hooked to the interrupt on digital pin 3 */
void onbuttonb() {
  wb++;
  if (wb >= WAVEFORM_COUNT) {
    wb = 0;
  }
}
