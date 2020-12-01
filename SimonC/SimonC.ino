#include "i2c-lib/Client/VB_I2C.cpp" //inclure l'annexe
#include "simonClass.h"              //annexe pour les classes

//...
VbI2C vbi2c = new VbI2C(0x6); // adresse i²c du module
Wire.onReceive(handleReceive);
Wire.onRequest(handleRequest);
vbi2c->setCallback(handleData);
//...

int party_code = 0; //variable ou est stocké le code partie
int difficulty = 2; //variable de la difficultée   ****Modifié****(0)

int stateGame; //Etat du jeu: Création du tableau réponse, Montrer les couleurs, attendre une réponse ou traiter les informations

const int duree_pas = 500; //Durée d'allumage des leds

bool simonErreur = 0; //Erreur envoyé à la valise

bool simonGood = 0; //Réussi envoyé à la valise

int error = 0; //Nombre d'erreur sur le Simon: 3chances

int dataTab[5];        //Tableau contenant les nombres aléatoires, les couleurs
int dataColor[5];      //tableau contenant les couleurs qui s'afficheront
int alea;              //Si level 1: 3, level 2: 4 et level 3: 4:  >   Nombre de case utilisées dans le tabeau data
int bon = 0;           //Si la séquencce est bonne
int memory[5];         //récupération des buttons
int i;                 //Traitement du tableaux dataTab
int y = 0;             //Traitement du tableau memory
int aleaMSequence = 1; //Chekpoint servant à atraiter les couleurs les unes après les autres

int ReadButton; //Button lu
int bump = 0;   //Si le boutton est déjà appuyé

int trueButton = 0; //Pour gérer la récupération des données et l'adapter à la difficultté

//Millis
int count = 0;

//Définissions des Leds
Led ledRouge;
Led ledBleu;
Led ledVert;
Led ledJaune;

Led InWork;
Led End;
Led OutWork;

//définissions boutons
button redButton;    //Previusly :A3
button blueButton;   //Previusly :A0
button greenButton;  //Previusly :A2
button yellowButton; //Previusly :A1

//Définissions des buzzer
Buzzer BuzzRed;
Buzzer BuzzBlue;
Buzzer BuzzGreen;
Buzzer BuzzYellow;
Buzzer ErrorBuz;

Buzzer BD6;
Buzzer BC6;
Buzzer BA6;
Buzzer BF6;
Buzzer BE6;
Buzzer BB5;

// Block obligatoire  (librairie)

VbI2C *vbi2c;

void handleRequest()

{

  vbi2c->requestEvent();
}

void handleReceive(int bytes)

{

  vbi2c->receiveEvent();
}

void handleData()
{
  Serial.println("DATA RECEIVED");
  SERVER_DATA_T packet = vbi2c->getData();
  if (packet->dataType == SERVER_DATA_TYPE::ABORT_GAME)
  {
  }
  else if (packet->dataType == SERVER_DATA_TYPE::START)
  {
    uint8_t difficulty = packet->data[0];
  }
}

// Fin du block obligatoire.

//Remet à 0 les variables dont on a besoin pour traiter les informations (la mémoires, le stategame...)
void start()
{
  bon = 0;

  for (int t = 0; t < 5; t++)
  {
    memory[t] = 0; //remise à 0 du tableau
  }
  y = 0;
  stateGame = 1;
  Serial.println("void begin");
}

void setup()
{

  // …

  vbi2c = new VbI2C(0x1);

  Wire.onReceive(handleReceive);

  Wire.onRequest(handleRequest);

  vbi2c->setCallback(handleData);
  //...

  //Initialisation du port série
  Serial.begin(115200);
  Serial.println("Begin");

  //Définissions des Leds
  ledRouge.SetLed(9);
  ledBleu.SetLed(6);
  ledVert.SetLed(8);
  ledJaune.SetLed(7);

  OutWork.SetLed(4);
  InWork.SetLed(5);
  End.SetLed(3);

  InWork.Allumer();
  End.Eteindre();
  OutWork.Eteindre();

  //Définission des Buzzers
  BuzzRed.SetBuzzer(220);
  BuzzBlue.SetBuzzer(330);
  BuzzGreen.SetBuzzer(392);
  BuzzYellow.SetBuzzer(262);
  ErrorBuz.SetBuzzer(155);

  BD6.SetBuzzer(1175);
  BC6.SetBuzzer(1047);
  BA6.SetBuzzer(1760);
  BF6.SetBuzzer(1397);
  BE6.SetBuzzer(1319);
  BB5.SetBuzzer(988);

  redButton.Setbutton(A3);
  blueButton.Setbutton(A0);
  yellowButton.Setbutton(A1);
  greenButton.Setbutton(A2);

  randomSeed(analogRead(A0));
  //Activation de la séquence:
  stateGame = 0; //remettre à 4: Tps d'attente
  Serial.println("Init done");
}

void loop()
{

  switch (stateGame) //Dans quel Etat le jeu est
  {
  case 0:
    Serial.println("Random");
    Random(difficulty);
    stateGame = 4; //On repart dans le loop pour afficher les valeurs du tableau avec les leds
    break;
  case 1:
    Serial.println("Case 1 Affichage");
    showSequence(dataTab[i]); //Afficher les couleurs
    stateGame = 2;
    break;

  case 2:
    Serial.println("Case 2 Récupération");
    pushBouton(redButton, 4);
    pushBouton(yellowButton, 2);
    pushBouton(greenButton, 3);
    pushBouton(blueButton, 1);

    if (count >= 700)
    {
      count =0;
      stateGame = 1;
    }

    break;

  case 3:
    Serial.println("Case 3 Traitement");
    testSequence();
    break;

  case 4:
    Serial.println("Case 4 Waiting");
    if (redButton.read() == 0 || blueButton.read() == 0 || yellowButton.read() == 0 || greenButton.read() == 0)
    {
      stateGame = 1;
      for (int q = 0; q <= 3; q++)
      {
        ledVert.Allumer();
        ledRouge.Allumer();
        ledJaune.Allumer();
        ledBleu.Allumer();
        delay(500);
        ledVert.Eteindre();
        ledRouge.Eteindre();
        ledJaune.Eteindre();
        ledBleu.Eteindre();
      }
      delay(10);
    }
    break;

  case 5:
    Serial.println("END");

    ledVert.Allumer();
    delay(100);
    ledRouge.Allumer();
    delay(100);
    ledJaune.Allumer();
    delay(100);
    ledBleu.Allumer();
    delay(100);
    ledVert.Eteindre();
    delay(100);
    ledRouge.Eteindre();
    delay(100);
    ledJaune.Eteindre();
    delay(100);
    ledBleu.Eteindre();
    delay(100);
    break;

  default:
    Serial.println("Error 101");
    Serial.println(stateGame);
    delay(10000);
    //stateGame = 1;
    break;
  }
}

//Sert à remplir le tableau qui donnera les bonnes valeurs; il sera comparé avecle tableau réponse.
int Random(int dif)
{
  switch (dif) //Test de la difficulté, donnée par la valise: Donne le nombre de case utilisé par le tab
  {
  case 1:
    alea = 3;
    Serial.println("Alea = 3");
    //trueButton = 1;
    break;

  case 2:
    alea = 4;
    Serial.println("Alea = 4");
    //trueButton = 2;
    break;

  case 3:
    alea = 5;
    Serial.println("Alea = 5");
    //trueButton = 3;
    break;
  }
  for (int i = 0; i < alea; i++) //boucle pour remplir le tableau
  {
    dataTab[i] = random(1, 5);

    Serial.print("random = ");
    Serial.print(i);
    Serial.print("---->");
    Serial.println(dataTab[i]);
  }
  //&&&&&&
  if (difficulty == 1)
  {
    for (int i = 0; i < alea; i++)
    {
      if (dataTab[i] == 1)//réponse bleu
      {
        dataColor[i] = 4;//couleur montrée vert
      }
      else if (dataTab[i] == 2)//jaune
      {
        dataColor[i] = 3;//rouge
      }
      else if (dataTab[i] == 3)//vert
      {
        dataColor[i] = 2;//bleu
      }
      else if (dataTab[i] == 4)//rouge
      {
        dataColor[i] = 1;//jaune
      }
    }
  }

  else if (difficulty == 2)
  {
    for (int i = 0; i < alea; i++)
    {
      if (dataTab[i] == 1)//réponse bleu
      {
        dataColor[i] = 3;//couleur montrée vert
      }
      else if (dataTab[i] == 2)//jaune
      {
        dataColor[i] = 1;//bleu
      }
      else if (dataTab[i] == 3)//vert
      {
        dataColor[i] = 4;//rouge
      }
      else if (dataTab[i] == 4)//rouge
      {
        dataColor[i] = 2;//jaune
      }
    }
  }

  else if (difficulty == 3)
  {
    for (int i = 0; i < alea; i++)
    {
      if (dataTab[i] == 1)//bleu
      {
        dataColor[i] = 2;//jaune
      }
      else if (dataTab[i] == 2)//jaune
      {
        dataColor[i] = 4;//rouge
      }
      else if (dataTab[i] == 3)//vert
      {
        dataColor[i] = 1;//bleu
      }
      else if (dataTab[i] == 4)//rouge
      {
        dataColor[i] = 3;//vert
      }
    }
  }

  //&&&&&}
}
//Enoncé des couleurs
int showSequence(int Tab)
{
  for (int i = 1; i <= aleaMSequence; i++) //On regarde chaque case du tableau
  {
    OnButtonAndBuz(dataColor[i - 1]); //On lit les couleurs pour les afficher    &&&
    delay(duree_pas);                 //Temps de mise en route des couleurs
    OffButtonAndBuz();                //Extinction des couleurs
  }
}

int pushBouton(button TReadButton, int nbbutton) //On lit chaque bouton pour voir s'ils sont positif
{
  ReadButton = TReadButton.read();
  Serial.print("ReadButton=");
  Serial.println(ReadButton);
  count++;
  if (ReadButton == 0)
  {
    count = 0;
    bump = nbbutton;
    OnButtonAndBuz(bump);
  }

  else if (ReadButton == 1 && nbbutton == bump)
  {
    OffButtonAndBuz();
    memory[y] = bump;
    bump = 0;
    Serial.print("Coucou");
    y++;
  }

  Serial.print("y:");
  Serial.println(y - 1);
  Serial.print("TabM:");
  Serial.println(memory[y - 1]);
  Serial.println("---------------------------------------------------");
  Serial.print("aleaMSequence = ");
  Serial.println(aleaMSequence);

  if (y == aleaMSequence && y != 0)
  {
    stateGame = 3;
  }
  else
  {
    stateGame = 2;
  }
}

//-------------------------------------------------------------------------------
int testSequence() //On regarde
{
  for (int n = 1; n <= aleaMSequence || n <= 5; n++)
  {
    Serial.print("n=");
    Serial.println(n);
    if (memory[n - 1] == dataTab[n - 1])
    {
      Serial.println("memory[n] == dataTab[n]");
      Serial.println(memory[n]);
      Serial.println(dataTab[n]);
      bon++;
      if (bon == aleaMSequence)
      {
        Serial.println("'''''''''''''''''''''''''''''''''''''''''''''''''''''''''");
        Serial.println(aleaMSequence);
        reussi(bon);
        return;
      }
    }

    else if (memory[n] == 0)
    {
      Serial.println("memory = 0");
      start();
      error++;
      Error(error);
      return;
    }
    else
    {
      Serial.println("*************************************");
      Serial.println("BAD");
      Serial.print(memory[n]);
      Serial.print(" != ");
      Serial.println(dataTab[n]);
      error++;
      Error(error);
      return;
    }
  }
  Serial.println("End testsequence");
}

//-------------------------------------------------------------

int Error(int error)
{
  if (/*valiseError == 0//Quand la valise est branché*/ error < 3)
  {
    aleaMSequence = 1;

    Serial.print("error:");
    Serial.println(error);
    OnButtonAndBuz(6);
    //simonErreur=1;

    //...
    CLIENT_DATA message = {};
    message.dataType = CLIENT_DATA_TYPE::GAMEOVER;
    message.data[0] = 126;
    message.data[1] = 88;
    vbi2c->sendData(&message);
    //...
    start();
    return;
  }
  else if (/*valiseError == 1//Quand la valise est branché*/ error == 3)
  {
    End.Eteindre();
    OutWork.Allumer();
    InWork.Eteindre();
    OnButtonAndBuz(5);
    stateGame = 5;

    return;
  }
  else
  {
    Serial.print("Erreur 908 :");
    Serial.println(error);
  }
}

int reussi(int bien)
{
  Serial.println("Reussi");
  if (bien == alea)
  {
    OnButtonAndBuz(7);
    Serial.println("SimonGood END");
    InWork.Eteindre();
    OutWork.Eteindre();
    End.Allumer();
    stateGame = 5; //Etat de pause de la valise
    ///...
    CLIENT_DATA message = {};
    message.dataType = CLIENT_DATA_TYPE::SUCCESS;
    message.data[0] = 126;
    message.data[1] = 88;
    vbi2c->sendData(&message);
    //...
    return;
  }
  else
  {
    {
      Serial.println("Else reussi");
      aleaMSequence++;
      start();
      return;
    }
  }
}

//---------------------------------------------------------------------

int OnButtonAndBuz(int color)
{
  switch (color)
  {
  case 0:
    Serial.println("Show sequence 0, erreur");
    return;

  case 1:
    BuzzBlue.tonal();  //classe allumer le buzzer
    ledBleu.Allumer(); //classe allumer la couleur
    Serial.println("Show sequence bleu");
    break;

  case 2:
    BuzzYellow.tonal(); //classe allumer le buzzer
    ledJaune.Allumer(); //classe allumer la couleur
    Serial.println("Show sequence jaune");
    break;

  case 3:
    BuzzGreen.tonal(); //classe allumer le buzzer
    ledVert.Allumer(); //classe allumer la couleur
    Serial.println("Show sequence vert");
    break;

  case 4:
    BuzzRed.tonal();    //classe allumer le buzzer
    ledRouge.Allumer(); //classe allumer la couleur
    Serial.println("Show sequence rouge");
    break;

  case 5: //game over
    ledBleu.Allumer();
    ErrorBuz.tonal();
    delay(100);
    ErrorBuz.notonal();

    ledJaune.Allumer();
    ErrorBuz.tonal();
    delay(100);
    ErrorBuz.notonal();
    ledJaune.Eteindre();

    ledVert.Allumer();
    ErrorBuz.tonal();
    delay(100);
    ErrorBuz.notonal();
    ledVert.Eteindre();

    ledRouge.Allumer();
    ErrorBuz.tonal();
    delay(100);
    ErrorBuz.notonal();
    ledRouge.Eteindre();

    delay(100);
    ErrorBuz.tonal();
    ledBleu.Allumer();
    ledJaune.Allumer();
    ledVert.Allumer();
    ledRouge.Allumer();
    delay(900);
    ledBleu.Eteindre();
    ledJaune.Eteindre();
    ledVert.Eteindre();
    ledRouge.Eteindre();
    ErrorBuz.notonal();
    break;

  case 6: //Erreur
    ErrorBuz.tonal();
    ledBleu.Allumer();
    ledJaune.Allumer();
    ledVert.Allumer();
    ledRouge.Allumer();
    delay(200);
    ledBleu.Eteindre();
    ledJaune.Eteindre();
    ledVert.Eteindre();
    ledRouge.Eteindre();
    ErrorBuz.notonal();
    delay(100);
    ErrorBuz.tonal();
    ledBleu.Allumer();
    ledJaune.Allumer();
    ledVert.Allumer();
    ledRouge.Allumer();
    delay(200);
    ledBleu.Eteindre();
    ledJaune.Eteindre();
    ledVert.Eteindre();
    ledRouge.Eteindre();
    ErrorBuz.notonal();
    delay(500);
    break;

  case 7: //Win
    int i = 1;
    for (int t = 50; t < 21 * 50; t = t + 50)
    {
      tone(11, t); //pin 11
      ledBleu.Allumer();
      delay(10);
      ledJaune.Allumer();
      delay(10);
      ledRouge.Allumer();
      delay(10);
      ledVert.Allumer();
      delay(10);
      noTone(11);
      ledBleu.Eteindre();
      delay(10);
      ledJaune.Eteindre();
      delay(10);
      ledRouge.Eteindre();
      delay(10);
      ledVert.Eteindre();
    }
    break;
  }
}

int OffButtonAndBuz()
{
  ledBleu.Eteindre();  //classe éteindre la couleur
  ledJaune.Eteindre(); //classe éteindre la couleur
  ledVert.Eteindre();  //classe éteindre la couleur
  ledRouge.Eteindre(); //classe éteindre la couleur
  BuzzBlue.notonal();  //classe éteindre le buzzer, on s'en fiche duquel on met, il s'agit du même pin
  delay(300);
}
