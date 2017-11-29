#define CS  PA4
#define SCK PA5
#define SO PB6//PA7

class MAX6675
{
    public:
        MAX6675(int8_t SCK, int8_t CS, int8_t SO);

        double readCelsius(void);
        double readFahrenheit(void);
        double readFarenheit(void) { return readFahrenheit(); }
    private:
        int8_t sck, so, cs;
        uint8_t spiread(void);
};

MAX6675::MAX6675(int8_t SCK, int8_t CS, int8_t SO)
{
    sck = SCK;
    cs = CS;
    so = SO;
    
    pinMode(cs, OUTPUT);
    pinMode(sck, OUTPUT); 
    pinMode(so, INPUT);
  
    digitalWrite(cs, HIGH);
}

double MAX6675::readCelsius(void)
{
    uint16_t v;
  
    digitalWrite(cs, LOW);
    delayMicroseconds(1);
    v = spiread();
    v <<= 8;
    v |= spiread();
    digitalWrite(cs, HIGH);
  
    if (v & 0x4) {
      return NAN; 
    }
    v >>= 3;

    return v*0.25;
}

double MAX6675::readFahrenheit(void) {
  return readCelsius() * 9.0/5.0 + 32;
}

byte MAX6675::spiread(void) { 
  int i;
  byte d = 0;

  for (i=7; i>=0; i--)
  {
    digitalWrite(sck, LOW);
    delayMicroseconds(1);
    if (digitalRead(so)) {
      d |= (1 << i);
    }

    digitalWrite(sck, HIGH);
    delayMicroseconds(1);
  }

  return d;
}

MAX6675 thermocouple(SCK, CS, SO);

void setup()
{  
    delay(500);
}

void loop()
{
    Serial.print("C = "); 
    Serial.println(thermocouple.readCelsius());
    Serial.print("F = ");
    Serial.println(thermocouple.readFahrenheit());

    delay(250);
}
