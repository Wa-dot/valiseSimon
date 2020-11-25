#include "simonClass.h"
#include <Arduino.h>
//**************************************************************************

int Led::SetLed(int aPin)
{
  pin = aPin;
  pinMode(aPin, OUTPUT);
  Eteindre(); 
  Serial.println("Le Done");
}
int Led ::Allumer()
{
  etat = HIGH;
  Rafraichir();
}
int Led::Eteindre()
{
  etat = LOW;
  Rafraichir();
}

int Led::Rafraichir()
{
  int trueState = etat;
  digitalWrite(pin, trueState);
}

//*********************************q******************************************

int button::Setbutton(int bPin)
{
  pin = bPin;
  pinMode(bPin, INPUT);
  Serial.print(pin);
  /*if (digitalRead(this->pin))
  {
    change = 1;
  }
  else
  {
    change = 0;
  }*/
  Serial.println("But Done");
}

int button::read()
{
  return digitalRead(this->pin);
  /*
  switch (change)
  {
    case 0:
      return digitalRead(this->pin);
      break;

    case 1:
      if (digitalRead(this->pin) == 0)
      {
        return 1;
      }
    default:
      Serial.print("Erreur button::read = ");
      Serial.println(change);
      break;
  }*/
}

//**************************************************************************

int Buzzer::SetBuzzer(int heightSound)
{
  pinMode(11, INPUT);
  HSound = heightSound;
  Serial.println("Buz Done");
}

int Buzzer::tonal()
{
  tone(11, HSound);//Pin 11
}

int Buzzer::notonal()
{
  noTone(11);
}

//******************************************************************
