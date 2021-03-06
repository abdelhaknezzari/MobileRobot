// Prérequis savoir utiliser un timer 
#include <TimerOne.h> // Pour le timer
#include <NewPing.h>
#include <Servo.h> 

// Prérequis savoir utiliser les define
// Partie à modifier pour indiquer en define quel pin est branché où A et B à brancher sur 2 et 4
#define ENCODEURA_LEFT 2
#define ENCODEURB_LEFT 4


// Prérequis savoir utiliser les define
// Partie à modifier pour indiquer en define quel pin est branché où A et B à brancher sur 3 et 10
#define ENCODEURA_RIGHT 3
#define ENCODEURB_RIGHT 7


#define TRIGGER_PIN_RIGHT  10  // Arduino pin tied to trigger pin on the ultrasonic sensor   7.
#define ECHO_PIN_RIGHT      8  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define TRIGGER_PIN_LEFT  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_LEFT     13  // Arduino pin tied to echo pin on the ultrasonic sensor.

// prérequis : volatile => pour toute variable qui sera utilise dans les interruptions 

volatile int count_left =0; // comptage de tick d'encoder  qui sera incrémenté sur interruption " On change " sur l'interruption matériel 0 qui se fait sur le pin 2
volatile double speed_left =0; // vitesse du moteur 
volatile byte laststate_left =0;  // etat précédent de l'encodeur 


volatile int count_right =0; // comptage de tick d'encoder  qui sera incrémenté sur interruption " On change " sur l'interruption matériel 0 qui se fait sur le pin 2
volatile double speed_right =0; // vitesse du moteur 
volatile byte laststate_right =0;  // etat précédent de l'encodeur 

 
int RPWM_Output_RIGHT = 9; // Arduino PWM output pin 9; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output_RIGHT = 11; // Arduino PWM output pin 11; connect to IBT-2 pin 2 (LPWM)

int RPWM_Output_LEFT = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output_LEFT = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)


 
NewPing sonarLeft(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void stop() //Stop
{
    digitalWrite(RPWM_Output_RIGHT,LOW);
    digitalWrite(LPWM_Output_RIGHT,LOW);
    
    digitalWrite(RPWM_Output_LEFT,LOW);
    digitalWrite(LPWM_Output_LEFT,LOW);
}

void advance(char a) // En avant
{
//analogWrite (A,a); // Contrôle de vitesse en PWM
//digitalWrite(B,HIGH);

    int reversePWM = (int ) a;
    analogWrite(LPWM_Output_RIGHT, 0);
    analogWrite(RPWM_Output_RIGHT, reversePWM);
    
    
    analogWrite(LPWM_Output_LEFT, 0);
    analogWrite(RPWM_Output_LEFT, reversePWM);
    
    
}

void back_off (char a) // En arrière
{
//analogWrite (A,a);
//digitalWrite(B,LOW);
    int forwardPWM  = (int) a;
    analogWrite(LPWM_Output_RIGHT, forwardPWM);
    analogWrite(RPWM_Output_RIGHT, 0);
    
    analogWrite(LPWM_Output_LEFT, forwardPWM);
    analogWrite(RPWM_Output_LEFT, 0);
    
}

void setup()
{
// on initialise les entrées et sorties
 pinMode(ENCODEURA_LEFT, INPUT_PULLUP);
 pinMode(ENCODEURB_LEFT, INPUT_PULLUP);
 
 pinMode(ENCODEURA_RIGHT, INPUT_PULLUP);
 pinMode(ENCODEURB_RIGHT, INPUT_PULLUP);
 
 pinMode(RPWM_Output_RIGHT, OUTPUT);
 pinMode(LPWM_Output_RIGHT, OUTPUT);
 
 pinMode(RPWM_Output_LEFT, OUTPUT);
 pinMode(LPWM_Output_LEFT, OUTPUT);
 


attachInterrupt(INT0,counter_left, CHANGE); // on crée l'interruption sur changement sur la pin 2 => interruption 0, la routine counter va se faire toute seule sans l'appeler à chaque changement d'état sur le pin 2
attachInterrupt(INT1,counter_right, CHANGE); 


Timer1.initialize(100000); // On défini le timeur : 100000 microseconds ( 0.1 sec - or 10Hz )
Timer1.attachInterrupt( timerIsr ); // attach the service routine here la fonction timerIsr est une routine qui va se faire automatiquement 10* par secondes sans l'appeler

Serial.begin(9600); // Définit vitesse de transmission série
Serial.println("Execution du controle par clavier");
}

void loop()
{
  if(Serial.available()){
  char val = Serial.read(); //récupération des caractères sur le
  if(val != -1)             //port série
  {
    switch(val)
    {
    case 'z':// En avant si touche "z"
        advance (100); // en avant vitesse max
                          // le driver fonctionne à l'état bas en marche avant
        break;
    case 's':// En arrière si touche "s"
        back_off (100); // en arrière vitesse max
                      // le driver fonctionne à l'état haut en marche arrière
        break;
    case 'x': // arrêt si touche "x"
      stop();
      break;
    }
  }
  else stop();
}

//Serial.print("speed_left :");             
//Serial.print(speed_left); 
//
//Serial.print("speed_right :");             
//Serial.println(speed_right); 


 float sensorValue0 = analogRead(A0) / 650;  
 float sensorValue1 = analogRead(A4) / 650;  
 float sensorValue2 = analogRead(A3) / 650;  
 float sensorValue3 = analogRead(A2) / 650;                             
 float sensorValue4 = analogRead(A1) / 650;  
 float sensorValue5 = analogRead(A5) / 650;  
  
 
 Serial.print(" A0: ");Serial.print(sensorValue0);
 Serial.print(" A1: ");Serial.print(sensorValue1);
 Serial.print(" A2: ");Serial.print(sensorValue2);
 Serial.print(" A3: ");Serial.print(sensorValue3);
 Serial.print(" A4: ");Serial.print(sensorValue4);
 Serial.print(" A5: ");Serial.print(sensorValue5);
 Serial.println("");

//  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
//  unsigned int uS = sonarLeft.ping(); // Send ping, get ping time in microseconds (uS).
//  unsigned int uS2 = sonarRight.ping(); // Send ping, get ping time in microseconds (uS).
//  Serial.print("Ping Left: ");
//  Serial.print(sonarLeft.convert_cm(uS)); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
//  Serial.print("cm");
//  Serial.print("Ping Right: ");
//  Serial.print(sonarRight.convert_cm(uS2)); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
//  Serial.println("cm");

}


// Encoder counter 0

void counter_left()
{
  byte state=PIND;
  
  state|=B11101011;  // mask pour ne regarder que les changements sur 2 et 4 
  // Modifier le MASK  B01111011 par BXXXXXXXX mettre des 0 là où sont les pins utilisés par l'encodeur
  if( state!=laststate_left)
  {
    (((state&(1<<ENCODEURA_LEFT))>>ENCODEURA_LEFT)^((state&(1<<ENCODEURB_LEFT))>>ENCODEURB_LEFT))?count_left--:count_left++;
    laststate_left=state;
  }
  
 
}


void counter_right()
{
  byte state=PIND;
  
   state|=B01110111;  // mask pour ne regarder que les changements sur 3 et 7 
//  // Modifier le MASK  B01110111 par BXXXXXXXX mettre des 0 là où sont les pins utilisés par l'encodeur
  if( state!=laststate_right)
  {
    (((state&(1<<ENCODEURA_RIGHT))>>ENCODEURA_RIGHT)^((state&(1<<ENCODEURB_RIGHT))>>ENCODEURB_RIGHT))?count_right--:count_right++;
    laststate_right=state;
  }

  
}





// Timer event to calculate speed from counters

void timerIsr()
{
    speed_left=count_left; // on a le nombre de tick ( signé ) par unité de temps  = vitesse
    count_left=0; // On remet le compteur de tick à 0 
    
    
    speed_right=count_right; // on a le nombre de tick ( signé ) par unité de temps  = vitesse
    count_right=0; // On remet le compteur de tick à 0 
    
//    Serial.println("timerIsr");
}


