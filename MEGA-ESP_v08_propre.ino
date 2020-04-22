int getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}

byte Heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};



//=========================================debut Muoton"Black Sheep" Alexandros Pantelidis Avril 2020=================================
byte tete1[8] =
{
  0b00011,
  0b00100,
  0b01010,
  0b10000,
  0b11111,
  0b00011,
  0b00011,
  0b00010
};

byte tete2[8] =
{
  0b11000,
  0b11100,
  0b10001,
  0b11110,
  0b10001,
  0b11000,
  0b11111,
  0b10001
};

byte tete3[8] =
{
  0b00000,
  0b00000,
  0b10011,
  0b01110,
  0b10001,
  0b00100,
  0b10001,
  0b01110
};

byte tete4[8] =
{
  0b00000,
  0b00000,
  0b00000,
  0b10000,
  0b01000,
  0b01000,
  0b11100,
  0b10110
};

byte bas1[8] =
{
  0b00010,
  0b00010,
  0b00011,
  0b00001,
  0b00001,
  0b00000,
  0b00000,
  0b00000
};

byte bas2[8] =
{
  0b01011,
  0b01100,
  0b00111,
  0b01000,
  0b11111,
  0b01001,
  0b01001,
  0b01001
};

byte bas3[8] =
{
  0b00011,
  0b11001,
  0b00000,
  0b10100,
  0b11111,
  0b00010,
  0b00010,
  0b00010
};

byte bas4[8] =
{
  0b10001,
  0b01001,
  0b00101,
  0b10011,
  0b11110,
  0b00100,
  0b00100,
  0b00100
};
//=========================================Fin mouton=================================
//=========================================Librarie utilisées=========================
#include <EEPROM.h>
#include <DHT.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include  <virtuabotixRTC.h>  //Library used
#include <ResponsiveAnalogRead.h>
//=========================================Pin definie=================================
#define RELAY1 24
#define POMPES 22
#define LAMPES 26
#define moteurA_1 38
#define moteurA_2 40
#define moteurB_1 36
#define moteurB_2 34
#define DS1302_GND_PIN 33
#define DS1302_VCC_PIN 31
#define Tbox A2
#define DHTPIN 32     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define TdsSensorPin A8
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 3 // sum of sample point
#define ONE_WIRE_BUS 7
//=========================================Constantes utiilisées============================
unsigned long currentMillis1 = 0;
unsigned long currentMillis4 = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis = 0;
unsigned long previousMillis3 = 0;
const long interval = 10000;
unsigned long periodtds = 3600000; // 1h
const long nettoyage = 10000;
const int chipSelect = 49;
unsigned long startMillis1;
unsigned long startMillis;
String message = "";
bool messageReady = false;
String message1 = "";
bool messageReady1 = false;


int analogBuffer1[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp1[SCOUNT];
int analogBufferIndex1 = 0, copyIndex1 = 0;

int analogBuffer[SCOUNT];  
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0 , tdsValue = 0;

int analogBuffer2[SCOUNT]; 
int analogBufferTemp2[SCOUNT];
int analogBufferIndex2 = 0, copyIndex2 = 0;
float averageVoltage2 = 0;

int val;
int addr = 0;

const byte numChars = 64;
char receivedChars[numChars];
char tempChars[numChars];
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;
boolean newData = false;

int ledstate;
int pompestate;

int int1;
int int2;
int int3;
int int4;
int int5;
int int6;
int int7;

int minteau ;
int maxteau ;
int timearro;

float  temperature;
float    humi;
float  temp;
float   tbox;
int tdsval;

int OnHour;
int OnMin;           //set hour and minutes for timer

int OffHour;
int OffMin;

int valeur_brute;

bool freshtds = false;
bool cleanup = false;
bool perioff = true;
//=========================================Fonction declaré============================

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
ResponsiveAnalogRead analog(Tbox, true);
virtuabotixRTC myRTC(35, 37, 39); //If you change the wiring change the pins here also
//=========================================Fonction demmarage ============================
void LedState() {

  val = EEPROM.read(addr);
  if (val == 1)
  {
    digitalWrite(LAMPES, HIGH);

  }
  if (val == 2)
  {
    digitalWrite(LAMPES, LOW);
  }
  maxteau = EEPROM.read(2);
  minteau = EEPROM.read(3);
  OnHour = EEPROM.read(4);

  OnMin = EEPROM.read(5);
  OffHour = EEPROM.read(6);
  OffMin = EEPROM.read(7);
  tdsval = 10 * EEPROM.read(8);

  timearro = EEPROM.read(9);

  Serial.println(maxteau);
  Serial.println(minteau);
  Serial.println(OnHour);
  Serial.println(OnMin);
  Serial.println(OffHour);
  Serial.println(OffMin);
  Serial.println(tdsval);
  Serial.println(timearro);

}

//=========================================Executé au demmarage============================
void setup() {
  Serial.begin(9600);
  delay(1);
  Serial1.begin(115200);
  delay(1);
  Serial.print("Initializing SD card...");
  delay(1);

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");
  dht.begin();
  sensors.begin();
  pinMode(TdsSensorPin, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(POMPES, OUTPUT);
  pinMode(LAMPES, OUTPUT);
  pinMode(Tbox, INPUT);
  pinMode(moteurA_1, OUTPUT);
  pinMode(moteurB_1, OUTPUT);
  pinMode(moteurA_2, OUTPUT);
  pinMode(moteurB_2, OUTPUT);
  digitalWrite(POMPES, HIGH);
  digitalWrite(moteurA_1, LOW);
  digitalWrite(moteurA_2, LOW);
  digitalWrite(moteurB_1, LOW);
  digitalWrite(moteurB_2, LOW);
  digitalWrite(DS1302_GND_PIN, LOW);
  pinMode(DS1302_GND_PIN, OUTPUT);
  digitalWrite(DS1302_VCC_PIN, HIGH);
  pinMode(DS1302_VCC_PIN, OUTPUT);

  LedState();
  startMillis1 = millis();
  startMillis = millis();
  lcd.init();
  lcd.createChar(0, Heart);
  lcd.createChar(1, tete1);
  lcd.createChar(2, tete2);
  lcd.createChar(3, tete3);
  lcd.createChar(4, tete4);
  lcd.createChar(5, bas1);
  lcd.createChar(6, bas2);
  lcd.createChar(7, bas3);
  lcd.createChar(8, bas4);
  lcd.backlight();
  lcd.setCursor(0, 0);

  lcd.print("Alex's Hydro Box");
  lcd.setCursor(3, 1);
  lcd.print("Bienvenue ^^");
  delay(5000);
  lcd.clear();
  //myRTC.setDS1302Time(0, 8, 19, 2, 7, 4, 2020);

}


//=========================================Set cycle arrosage /on/off============================


void setpompe() {
  timearro = EEPROM.read(9);

  currentMillis1 = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis1 - startMillis >= (1000 * 60 * timearro) ) {
    digitalWrite(POMPES, !digitalRead(POMPES));  //if so, change the state of the LED.  Uses a neat trick to change the state
    startMillis = currentMillis1;  //IMPORTANT to save the start time of the current LED state.
  }
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}



void parseData() {

  char * strtokIndx; // this is used by strtok() as an index

  //strtokIndx = strtok(tempChars,",");      // get the first part - the string
  //strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  strtokIndx = strtok(tempChars, ","); // this continues where the previous call left off
  integerFromPC = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  int1 = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  int2 = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  int3 = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  int4 = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  int5 = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  int6 = atoi(strtokIndx);


}

//=========================================fonction recu depuis nodemcu============================

void showParsedData() {
  if (integerFromPC  == 1) {
    digitalWrite(LAMPES, HIGH);
    EEPROM.update(addr, 1);
    integerFromPC  = 250;
  }
  if (integerFromPC  == 0) {
    digitalWrite(LAMPES, LOW);
    EEPROM.update(addr, 2);
    integerFromPC  = 250;
  }

  if (integerFromPC  == 4) {
    digitalWrite(POMPES, HIGH);
    integerFromPC  = 250;
  }
  if (integerFromPC  == 5) {
    digitalWrite(POMPES, LOW);
    integerFromPC  = 250;
  }
  if (integerFromPC  == 9 && int1 != 0 ) {


    int  timearro = int1;

    EEPROM.update(9, timearro);
    integerFromPC  = 250;
    int1 = 0;
  }
}

void formulair () {
  if (integerFromPC  == 6 && int1 != 0 && int2 != 0) {
    int maxteau = int1;
    int minteau = int2;
    EEPROM.update(2, maxteau);
    EEPROM.update(3, minteau);
    Serial.println(maxteau);
    Serial.println(minteau);
    integerFromPC  = 250;
    int1 = 0;
    int2 = 0;
  }
}
void formulair1 () {
  if (integerFromPC  == 7 && int3 != 0 ) {
    int OnHour = int3;
    int OnMin = int4;
    int OffHour = int5;
    int OffMin = int6;
    EEPROM.update(4, OnHour);
    EEPROM.update(5, OnMin);
    EEPROM.update(6, OffHour);
    EEPROM.update(7, OffMin);
    Serial.println(OnHour);
    Serial.println(OnMin);
    Serial.println(OffHour);
    Serial.println(OffMin);
    integerFromPC  = 250;
  }
}
void tdsva () {
  if (integerFromPC  == 8 && int1 != 0 ) {
    int tdsval = int1;

    EEPROM.update(8, tdsval);

    Serial.println(tdsval * 10);

    integerFromPC  = 250;
  }
}

void settds() {
  int tdsval = 10 * EEPROM.read(8);   //10* valeur epprom car max 256 impossible de stocker 1000 comme valeur mais 100*10 c'est possible

  unsigned long currentMillis3 = millis();
  if (tdsValue  <= tdsval && tdsValue != 0 && freshtds == false && pompestate == 1 && tdsValue > 150) {
    
  currentMillis4 = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis4 - startMillis >= 8000 ) {
    digitalWrite(moteurB_1, !digitalRead(moteurB_1));  //if so, change the state of the LED.  Uses a neat trick to change the state
    startMillis1 = currentMillis4;  //IMPORTANT to save the start time of the current LED state.
  
  }
  previousMillis3 = currentMillis3;
    cleanup = true;
  }
  if (tdsValue  >= tdsval && cleanup == true ) {

    digitalWrite(moteurB_1, LOW);
    digitalWrite(moteurB_2, HIGH);

    freshtds = true;
    cleanup = false;
    perioff = false;

  }
  if (currentMillis3 - previousMillis3 > nettoyage && perioff == false ) {

    digitalWrite(moteurB_1, LOW);
    digitalWrite(moteurB_2, LOW);
    perioff = true;
  }

  if (currentMillis3 - previousMillis3 > periodtds && perioff == true) {
    Serial.print("freshtds false");
    freshtds = false;
  }
}
void loop() {

  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    showParsedData();
    newData = false;
  }

  state();
  sensor();
  tdsva();
  setpompe();
  myRTC.updateTime();
  settds();
  formulair();
  formulair1();
  timmer();
  data();
  flcd();
  average();
  setwater();
}

void state() {
  maxteau = EEPROM.read(2);
  minteau = EEPROM.read(3);

  OnHour = EEPROM.read(4);
  OnMin = EEPROM.read(5);
  OffHour = EEPROM.read(6);
  OffMin = EEPROM.read(7);

  tdsval = 10 * EEPROM.read(8);
  timearro = EEPROM.read(9);
  analog.update();
  ledstate = digitalRead(LAMPES);
  pompestate = digitalRead(POMPES);
}

void sensor() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  humi = dht.readHumidity();
  temp = dht.readTemperature();

  valeur_brute = analog.getValue();
  tbox = valeur_brute * (5.0 / 1023.0 * 100.0);
}

void setwater() {
  if (temperature >= maxteau && temperature != -127 && temperature != 0)
  {
    digitalWrite(RELAY1, HIGH);

  }
  else if (temperature <= minteau && temperature != -127 && temperature != 0)
  {
    digitalWrite(RELAY1, LOW);

  }
  else {
    ;
  }
}

void average() {
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U) //every 40 milliseconds,read the analog value from the ADC
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin); //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U)
  {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value
  }
}



void recorddata() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > 1000) {
    previousMillis = currentMillis;
    //Serial.print("Current Date / Time: ");

    //  Serial.print(myRTC.dayofmonth);             //You can switch between day and month if you're using American system && myRTC.minutes == OnMin
    // Serial.print("/");
    // Serial.print(myRTC.month);
    // Serial.print("/");
    //  Serial.print(myRTC.year);
    //  Serial.print(" ");
    //  Serial.print(myRTC.hours);
    // Serial.print(":");
    //Serial.print(myRTC.minutes);
    //  Serial.print(":");
    // Serial.println(myRTC.seconds);
    // Serial.print("Humidity: ");
    // Serial.print(humi);
    // Serial.println(" %");
    // Serial.print("Temp air: ");
    //Serial.print(temp);
    // Serial.println(" Celsius");
    // Serial.print("Temp H2O ;");
    // Serial.print(temperature);
    //  Serial.println(" Celsius");
    // Serial.print("TDS Value:");
    // Serial.print(tdsValue, 0);
    //  Serial.println("ppm");
    //Serial.print("Temp box: ");
    //  Serial.println(tbox, 1);
    // Serial.println(" Celsius");

    File dataFile = SD.open("datalog1.txt", FILE_WRITE);
    delay(1);
    dataFile.println(temperature);
    delay(1);
    dataFile.close();

  }
}

void timmer() {
  if (myRTC.hours == OnHour && myRTC.minutes == OnMin ) {
    EEPROM.update(addr, 1);
    digitalWrite(LAMPES, HIGH);
    delay(5);
  }
  else if (myRTC.hours == OffHour && myRTC.minutes == OffMin ) {  // add && myRTC.minutes == OnMin   ;    (myRTC.hours == OffHour && myRTC.minutes == OffMin){
    EEPROM.update(addr, 2);
    digitalWrite(LAMPES, LOW);
    delay(5);
  }
}

void data() {
  if (integerFromPC  == 3) {

    Serial1.print("<");
    Serial1.print(ledstate);
    Serial1.print(",");
    Serial1.print(pompestate);
    Serial1.print(",");
    Serial1.print(tdsValue, 0);
    Serial1.print(",");
    Serial1.print(temp, 2);
    Serial1.print(",");
    Serial1.print(temperature, 2);
    Serial1.print(",");
    Serial1.print(humi, 2);
    Serial1.print(",");
    Serial1.print(tbox, 2);
    Serial1.print(",");
    Serial1.print(tdsval);
    Serial1.print(",");
    Serial1.print(OnHour);
    Serial1.print(",");
    Serial1.print(OnMin);
    Serial1.print(",");
    Serial1.print(OffHour);
    Serial1.print(",");
    Serial1.print(OffMin);
    Serial1.print(",");
    Serial1.print(maxteau);
    Serial1.print(",");
    Serial1.print(minteau);
    Serial1.print(",");
    Serial1.print(timearro);
    Serial1.print(">");
    Serial1.println("");
  }
}


void flcd() {
  lcd.setCursor(0, 0);
  lcd.print("T Eau:");
  lcd.print(temperature, 1);
  lcd.print((char)223);
  lcd.setCursor(12, 0);
  lcd.print("Hum:");
  lcd.print(humi, 0);
  lcd.print(" % ");
  lcd.print("AP Grow Box V1");
  lcd.print(" ");
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.write(byte(4));

  lcd.setCursor(0, 1);
  lcd.print("T Air:");
  lcd.print(temp, 1);
  lcd.print((char)223);
  lcd.setCursor(12, 1);
  lcd.print("TDS:");
  lcd.print(tdsValue, 0);
  lcd.print(" ppm ");
  lcd.print("T Box:");
  lcd.print(tbox, 1);
  lcd.print((char)223);
  lcd.print(" ");
  lcd.write(byte(5));
  lcd.write(byte(6));
  lcd.write(byte(7));
  lcd.write(byte(8));
  lcd.scrollDisplayLeft();
}
