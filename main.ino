#include <ESP8266WiFi.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include <TridentTD_LineNotify.h>

#define LINE_TOKEN  "xxxx"


PZEM004Tv30 pzem1(4, 5); // Rx, Tx
PZEM004Tv30 pzem2(2, 0); // Rx, Tx
PZEM004Tv30 pzem3(13, 12); // Rx, Tx


char ssid[] = "xxxxx";
char pass[] = "xxxxx";
char auth[] = "xxxxx"; //Blynk Token

float voltage1, current1, power1, energy1, frequency1, pf1, va1;
float voltage2, current2, power2, energy2, frequency2, pf2, va2;
float voltage3, current3, power3, energy3, frequency3, pf3, va3;
float avgVoltage, avgCurrent, totalPower, totalEnergy, avgFrequency, avgPf, totalVa;

String lineMessage;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  LINE.setToken(LINE_TOKEN);
}

void loop() {
  voltage1 = pzem1.voltage();
  voltage1 = zeroIfNan(voltage1);
  current1 = pzem1.current();
  current1 = zeroIfNan(current1);
  power1 = pzem1.power();
  power1 = zeroIfNan(power1);
  energy1 = pzem1.energy() / 1000; //kwh
  energy1 = zeroIfNan(energy1);
  frequency1 = pzem1.frequency();
  frequency1 = zeroIfNan(frequency1);
  pf1 = pzem1.pf();
  pf1 = zeroIfNan(pf1);
  delay(100);

  voltage2 = pzem2.voltage();
  voltage2 = zeroIfNan(voltage2);
  current2 = pzem2.current();
  current2 = zeroIfNan(current2);
  power2 = pzem2.power();
  power2 = zeroIfNan(power2);
  energy2 = pzem2.energy() / 1000; //kwh
  energy2 = zeroIfNan(energy2);
  frequency2 = pzem2.frequency();
  frequency2 = zeroIfNan(frequency2);
  pf2 = pzem2.pf();
  pf2 = zeroIfNan(pf2);
  delay(100);

  voltage3 = pzem3.voltage();
  voltage3 = zeroIfNan(voltage3);
  current3 = pzem3.current();
  current3 = zeroIfNan(current3);
  power3 = pzem3.power();
  power3 = zeroIfNan(power3);
  energy3 = pzem3.energy() / 1000; //kwh
  energy3 = zeroIfNan(energy3);
  frequency3 = pzem3.frequency();
  frequency3 = zeroIfNan(frequency3);
  pf3 = pzem3.pf();
  pf3 = zeroIfNan(pf3);


  // Calculations

  avgVoltage = (voltage1 + voltage2 + voltage3) / 3;
  avgCurrent = (current1 + current2 + current3) / 3;
  totalPower = power1 + power2 + power3;
  totalEnergy = (energy1 + energy2 + energy3) / 1000; //kwh
  avgFrequency = (frequency1 + frequency2 + frequency3) / 3;

  if (pf1 == 0) {
    va1 = 0;
  } else {
    va1 = power1 / pf1;
  }

  if (pf2 == 0) {
    va2 = 0;
  } else {
    va2 = power2 / pf2;
  }

  if (pf3 == 0) {
    va3 = 0;
  } else {
    va3 = power3 / pf3;
  }

  totalVa = va1 + va2 + va3;

  if (totalVa == 0) {
    avgPf = 0;
  } else {
    avgPf = totalPower / totalVa;
  }

  //  testPrint();



  Blynk.run();
  //------sent data to blynk------

  Blynk.virtualWrite(V1, avgVoltage);
  Blynk.virtualWrite(V2, avgCurrent);
  Blynk.virtualWrite(V3, totalPower);
  Blynk.virtualWrite(V4, totalEnergy);
  Blynk.virtualWrite(V5, avgFrequency);
  Blynk.virtualWrite(V6, avgPf);
  Blynk.virtualWrite(V7, totalVa);



  Blynk.virtualWrite(V11, voltage1);
  Blynk.virtualWrite(V12, current1);
  Blynk.virtualWrite(V13, power1);
  Blynk.virtualWrite(V14, energy1);
  Blynk.virtualWrite(V15, frequency1);
  Blynk.virtualWrite(V16, pf1);

  Blynk.virtualWrite(V21, voltage2);
  Blynk.virtualWrite(V22, current2);
  Blynk.virtualWrite(V23, power2);
  Blynk.virtualWrite(V24, energy2);
  Blynk.virtualWrite(V25, frequency2);
  Blynk.virtualWrite(V26, pf2);

  Blynk.virtualWrite(V31, voltage3);
  Blynk.virtualWrite(V32, current3);
  Blynk.virtualWrite(V33, power3);
  Blynk.virtualWrite(V34, energy3);
  Blynk.virtualWrite(V35, frequency3);
  Blynk.virtualWrite(V36, pf3);


  //--------Line Notification
  //lineMessage = "\nแรงดัน:\n" + String(voltage) + " V" + "\n" + "กระแส " + String(current) + " A";  //ข้อความที่จะส่ง LINE
  //LINE.notify(lineMessage);


  delay(5000);
}


void printValue(String label, float value) {

  if (value != NAN) {
    Serial.print(label); Serial.println(value);
  } else {
    Serial.println("Error reading");
  }
}

float zeroIfNan(float v) {

  if (isnan(v)) {
    v = 0;
  }

  return v;
}

/*
void testPrint() {

  printValue("V1: ", voltage1);
  printValue("V2: ", voltage2);
  printValue("V3: ", voltage3);

  printValue("I1: ", current1);
  printValue("I2: ", current2);
  printValue("I3: ", current3);

  printValue("P1: ", power1);
  printValue("P2: ", power2);
  printValue("P3: ", power3);

  printValue("E1: ", energy1);
  printValue("E2: ", energy2);
  printValue("E3: ", energy3);

  printValue("F1: ", frequency1);
  printValue("F2: ", frequency2);
  printValue("F3: ", frequency3);

  printValue("PF1: ", pf1);
  printValue("PF2: ", pf2);
  printValue("PF3: ", pf3);

  Serial.println("------ Total ------");
  printValue("Average Voltage: ", avgVoltage);
  printValue("Average Current: ", avgCurrent);
  printValue("Total Power: ", totalPower);
  printValue("Average PF: ", avgPf);
  printValue("Total Energy: ", totalEnergy);

  Serial.println("<----- end ----->");
  Serial.println();

}
*/
