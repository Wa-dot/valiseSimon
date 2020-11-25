#ifndef SIMON_CLASS_H
#define SIMON_CLASS_H

//Définission des classes.
class Led //Classe définissant, allumant et éteignant les leds
{
private:
  int pin;
  int etat;

public:
  int SetLed(int aPin);
  int Allumer();
  int Eteindre();
  int Rafraichir();
};


//**************************************************************************

class button //Classe définissant les boutons
{
private:
  int pin;
  //int change;
  //int renvoi;

public:
  int Setbutton(int bPin);
  int read();
  //int detectButton();
};



//**************************************************************************

class Buzzer //Classe définissant, allumant et éteignant les buzzers
{
private:
  int HSound;

public:
  int SetBuzzer(int heightSound);
  int tonal();
  int notonal();
};

//**************************************************************************


#endif // !SIMON_CLASS_H
