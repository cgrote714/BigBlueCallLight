const int pinAnalogComBit = 0;
const int pinPwmLight = 3;
const int pinInButton = 2;
const int numpresses = 4;
const int resettime = 400;
const int debounce = 30;
int sensitivity;
int presses;
long lastpress;
int pr0;
int pr1;
int pr2;
int lightmode;
long laston;
boolean combit;
long time1;
const int light1 = 255;
const int wait1 = 5000;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinInButton, INPUT_PULLUP);
  pinMode(pinPwmLight, OUTPUT);
  analogWrite(pinPwmLight, 255);
  
  Serial.begin(9600);
  time1=millis();
  pr1=analogRead(pinAnalogComBit);
  pr2=analogRead(pinAnalogComBit);
  while (millis()-time1<10000)
    {
      if (analogRead(pinAnalogComBit)<pr1){pr1=analogRead(pinAnalogComBit);}
      if (analogRead(pinAnalogComBit)>pr2){pr2=analogRead(pinAnalogComBit);}
    }
  pr0 = (pr1+pr2)/2;
  sensitivity=abs(pr1-pr2)*2;
  presses = 0;
  analogWrite(pinPwmLight, 0);
  delay(200);
  for (int i = 0; i < numpresses; i++)
  {
  analogWrite(pinPwmLight, 255);
  delay(100);
  analogWrite(pinPwmLight, 0);
  delay(200);
  } 
  LightOff();
  
}

void loop() {
  Serial.println(digitalRead(pinInButton));
  
  // toggle light on & off
  CheckCB();
  //if(combit==1){analogWrite(pinPwmLight, 255);} else {analogWrite(pinPwmLight,0);}
  
  if (presses>0 && millis()-lastpress>resettime)
  {
    presses = 0;
  }
  
  if (combit==1)
  {
    Pressed();
    //Serial.print(" Presses:");
    //Serial.println(presses);
    while (combit==1)
    {
      CheckCB();
      //delay(debounce);
    }
    delay(debounce);
  }
  while(lightmode==1 && combit==0)
  {
    CheckCB();
    if(digitalRead(pinInButton)==0){LightOff();break;}
    // first level escalation
    if(millis()-laston>wait1){Blink();}
  } 
}

void Blink()
{
  while(combit==0 && lightmode==1)
  {
    CheckCB();
    analogWrite(pinPwmLight,255);
    delayCB(200);
    analogWrite(pinPwmLight,0);
    delayCB(200);
  }
}

void delayCB(int iWait)
{
  time1=millis();
  while (millis()<time1+iWait)
  {
    CheckCB();
    if (combit==1){LightOff();break;}
    if(digitalRead(pinInButton)==0){LightOff();break;}
  }
}

void CheckCB()
{
  if (abs(analogRead(pinAnalogComBit)-pr0)>sensitivity)
    {combit=1;}
  else
    {combit=0;} 
}

void Pressed()
{
  presses +=1;
  lastpress=millis();
  if (presses>=numpresses) {LightOn();} else {if (lightmode==1){LightOff();}}
}

void LightOn()
{
  analogWrite(pinPwmLight, light1);
  lightmode = 1;
  laston = millis();
}

void LightOff()
{
  analogWrite(pinPwmLight, 0);
  lightmode = 0;
  presses = 0;
  laston = 0;
}
 

