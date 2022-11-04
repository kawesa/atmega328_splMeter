#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "adc.h"
#include "audioget.h"
#include <SoftwareSerial.h>

#define rx 7
#define tx 6
#define dbVOut 6



SoftwareSerial esp(rx, tx);

#define UART_BAUD_RATE 9600
int16_t audioSPL = 0;

void setup() {
  Serial.begin(115200);
  esp.begin(115200);
  pinMode(dbVOut, OUTPUT);


  sei();
}

void loop() {
  // put your main code here, to run repeatedly:


  for (int x = 0; x < 50; x++) {
    audioSPL = audioSPL + get_audiospl();
  }

  audioSPL = (audioSPL / 50);

  Serial.println(audioSPL);
  String dbV = "{dbValue:" + String(audioSPL) + "}\n";

  Serial.println(dbV);
  esp.print(dbV);

 



  audioSPL = 0;
  delay(1000);
}

int16_t get_audiospl() {

  //get current reference voltage
  double refv = acd_getrealvref();

  audioget_init();

  int16_t audiorms = 0;
  int16_t audiospl = 0;
  uint16_t i = 0;
  double spectrum[AUDIOGET_SAMPLES / 2 + 1] = { 0 };

  //get samples
  audioget_getsamples();
  //compute fft
  audioget_computefft();
  //apply weighting
  audioget_doweighting();
  //get spectrum
  audioget_getmagnitude(&spectrum[0]);

  //get rms and spl
  audiorms = audioget_getrmsval();
  float rmsvolt = 0;
  if (audiorms <= 0)
    rmsvolt = AUDIOGET_VOLTREF;
  else
    rmsvolt = adc_getvoltage(audiorms, refv);
  audiospl = audioget_getspl(rmsvolt, AUDIOGET_VOLTREF, AUDIOGET_DBREF);

  // //output spl
  return audiospl;
  // Serial.println(audiospl);
}