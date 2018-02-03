
int pins[4];
int pins_en[2];
int i = 0;
int speed1 = 1;
unsigned long last = 0;
unsigned long period = 10000;

unsigned int sequence[4];

void setup() {
  // put your setup code here, to run once:

  pins[0] = 4;
  pins[1] = 5;
  pins[2] = 6;
  pins[3] = 7;
  
  pins_en[0] = 2;
  pins_en[1] = 3;

  sequence[0] = 10;
  sequence[1] = 6;
  sequence[2] = 5;
  sequence[3] = 9;
  
  for(int j = 0; j < 2; ++j) {
    pinMode(pins_en[j], OUTPUT);
    digitalWrite(pins_en[j], HIGH);
  }
  
  for(int j = 0; j < 4; ++j)
    pinMode(pins[j], OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  // wait for time to step
  unsigned long t = micros();
  
  Serial.println("the time is");
  Serial.println(t);
  
  if((last + period) > t) return;
  last = t;

  if(speed1 > 0)
  {
    i = (i + 1) % 4;
  }
  else if(speed1 < 0)
  {
    i = (i + 3) % 4;
  }
  
  for(int j = 0; j < 4; ++j)
  {
    digitalWrite(pins[j], (1 << j) & sequence[i]);
  }

}


