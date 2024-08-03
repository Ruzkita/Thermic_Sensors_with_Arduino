#include <Wire.h> 
#include "ADS1X15.h"
#include <math.h>

ADS1115 ADS(0x48);
int i = 0;
float tension = 0;
float c1 = 23.476, c2 = 0.035;
int multi_ctr[4] = {0, 0, 0, 0};
float temperature[16];

void setup() {
  Serial.begin(115200);
  Wire.begin();
  ADS.begin();
  pinMode(51, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(45, OUTPUT);
}

int state_machine(int i){
  if (i == 0){      multi_ctr[0] = 0; multi_ctr[1] = 0; multi_ctr[2] = 0; multi_ctr[3] = 0;};
  if (i == 1){      multi_ctr[0] = 0; multi_ctr[1] = 0; multi_ctr[2] = 0; multi_ctr[3] = 1;};
  if (i == 2){      multi_ctr[0] = 0; multi_ctr[1] = 0; multi_ctr[2] = 1; multi_ctr[3] = 0;};
  if (i == 3){      multi_ctr[0] = 0; multi_ctr[1] = 0; multi_ctr[2] = 1; multi_ctr[3] = 1;};
  if (i == 4){      multi_ctr[0] = 0; multi_ctr[1] = 1; multi_ctr[2] = 0; multi_ctr[3] = 0;};
  if (i == 5){      multi_ctr[0] = 0; multi_ctr[1] = 1; multi_ctr[2] = 0; multi_ctr[3] = 1;};
  if (i == 6){      multi_ctr[0] = 0; multi_ctr[1] = 1; multi_ctr[2] = 1; multi_ctr[3] = 0;};
  if (i == 7){      multi_ctr[0] = 0; multi_ctr[1] = 1; multi_ctr[2] = 1; multi_ctr[3] = 1;};
  if (i == 8){      multi_ctr[0] = 1; multi_ctr[1] = 0; multi_ctr[2] = 0; multi_ctr[3] = 0;};
  if (i == 9){      multi_ctr[0] = 1; multi_ctr[1] = 0; multi_ctr[2] = 0; multi_ctr[3] = 1;};
  if (i == 10){     multi_ctr[0] = 1; multi_ctr[1] = 0; multi_ctr[2] = 1; multi_ctr[3] = 0;};
  if (i == 11){     multi_ctr[0] = 1; multi_ctr[1] = 0; multi_ctr[2] = 1; multi_ctr[3] = 1;};
  if (i == 12){     multi_ctr[0] = 1; multi_ctr[1] = 1; multi_ctr[2] = 0; multi_ctr[3] = 0;};
  if (i == 13){     multi_ctr[0] = 1; multi_ctr[1] = 1; multi_ctr[2] = 0; multi_ctr[3] = 1;};
  if (i == 14){     multi_ctr[0] = 1; multi_ctr[1] = 1; multi_ctr[2] = 1; multi_ctr[3] = 0;};
  if (i == 15){     multi_ctr[0] = 1; multi_ctr[1] = 1; multi_ctr[2] = 1; multi_ctr[3] = 1;};

  if (multi_ctr[0] == 0){digitalWrite(51, LOW);} else{digitalWrite(51, HIGH);};
  if (multi_ctr[1] == 0){digitalWrite(49, LOW);} else{digitalWrite(49, HIGH);};
  if (multi_ctr[2] == 0){digitalWrite(47, LOW);} else{digitalWrite(47, HIGH);};
  if (multi_ctr[3] == 0){digitalWrite(45, LOW);} else{digitalWrite(45, HIGH);};
}

int get_tension(){
  ADS.setGain(0);
  int16_t val_0 = ADS.readADC(0);
  int16_t val_1 = ADS.readADC(1);
  float f = ADS.toVoltage(1);
  int16_t tension_raw = val_0 - val_1;
  return tension = tension_raw*f;
}

float get_temperature(float tension, int i){
  float R1 = 3300, R2 = 1200, R3 = 100, R4 = 100;
  float current = tension/R1;
  float thermic_resistance = (5-current*(R1+R2+R3+R4))/current/1000;
  float eq_temperature = (log(c1)-log(thermic_resistance))/c2;
  return temperature[i] = eq_temperature;
}

void loop() {
  state_machine(i);
  delay(200);
  get_tension();
  get_temperature(tension, i);

  if (i == 15){
    int j = 0;
    for(j = 0; j <= 15; j++){
      if (j == 0){
        Serial.println("");
      }
      Serial.print("Temperature "); Serial.print(j); Serial.print(": "); Serial.print(temperature[j]); Serial.println("ºC");
    }
    i = 0;
  } 
  else {i++;};
}
