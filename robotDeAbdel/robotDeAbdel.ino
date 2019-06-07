// Prérequis savoir utiliser un timer 
#include <TimerOne.h> // Pour le timer
#include <NewPing.h>
#include <Servo.h> 

// Prérequis savoir utiliser les define
// Partie à modifier pour indiquer en define quel pin est branché où A et B à brancher sur 2 et 4
#define ENCODEURA_LEFT 2
#define ENCODEURB_LEFT 4


// Prérequis savoir utiliser les define
// Partie à modifier pour indiquer en define quel pin est branché où A et B à brancher sur 3 7
#define ENCODEURA_RIGHT 3
#define ENCODEURB_RIGHT 5 //7

//Arduino pin tied to trigger pin on the ultrasonic sensor  
#define TRIGGER_PIN_RIGHT  10  // 
#define ECHO_PIN_RIGHT      11 //8  // 
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

volatile boolean onlyOnce = false;

 
int RPWM_Output_RIGHT = 9; // Arduino PWM output pin 9; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output_RIGHT = 8; //11; // Arduino PWM output pin 11; connect to IBT-2 pin 2 (LPWM)

int RPWM_Output_LEFT = 7; //5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
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

getStateEncoder(B00000011, B10000011);

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


 double sensorValue0 = analogRead(A0) ;  
 double sensorValue1 = analogRead(A1) ;  
 double sensorValue2 = analogRead(A2) ;  
 double sensorValue3 = analogRead(A3) ;                             
 double sensorValue4 = analogRead(A4) ;  
 double sensorValue5 = analogRead(A5) ;  
 double sensorValue6 = analogRead(A6) ;    
 double sensorValue7 = analogRead(A7) ; 
  
// Serial.print(" A0: ");Serial.print(getReflectanceValue(sensorValue0,'0'));
// Serial.print(" A1: ");Serial.print(getReflectanceValue(sensorValue1,'1'));
// Serial.print(" A2: ");Serial.print(getReflectanceValue(sensorValue2,'2'));
// Serial.print(" A3: ");Serial.print(getReflectanceValue(sensorValue3,'3'));
// Serial.print(" A4: ");Serial.print(getReflectanceValue(sensorValue4,'4'));
// Serial.print(" A5: ");Serial.print(getReflectanceValue(sensorValue5,'5'));
// Serial.print(" A6: ");Serial.print(getReflectanceValue(sensorValue6,'6'));
// Serial.print(" A7: ");Serial.print(getReflectanceValue(sensorValue7,'7'));
// Serial.println("");

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


boolean getReflectanceValue(double value,char ledName)
{
 
    switch(ledName)
    {
    case '0':// En avant si touche "z"
        if(value < 668.0) return true;
        else return false;
         break; 
       
    case '1':// En arrière si touche "s"
        if(value < 668.0) return true;
        else return false;
         break; 
    case '2': 
        if(value < 668.0) return true;
        else return false;
        break; 
        
    case '3': 
      if(value < 668.0) return true;
      else return false;
      break; 
        
     case '4': 
        if(value < 668.0) return true;
        else return false;
        break; 
        
     case '5': 
        if(value < 668.0) return true;
        else return false;
        break; 
        
      case '6': 
        if(value < 668.0) return true;
        else return false;
        break; 
        
      case '7': 
        if(value < 668.0) return true;
        else return false;
        break;    
        
    }
    
    

}


void printBinaryByte(byte value)
{
    Serial.print("B");
    for (byte mask = 0x80; mask; mask >>= 1) 
    {
        Serial.print((mask & value) ? '1' : '0');
    }
    Serial.println();
}



byte getStateEncoder(byte reg1, byte reg2)
{

     byte mask1 = B00000000;
     
     mask1 = bitSet(mask1,0);
     mask1 = bitSet(mask1,1);
     
     reg1 &= mask1;
     reg2 &= mask1;
     
     reg2 <<= 4;
     
     printBinaryByte(reg1 | reg2 );
     return reg1 | reg2 ;

}


void counter_left2()
{
  byte state =getStateEncoder(PIND,PINH);

  int encA_left = 0;
  int encB_left = 0;
  
  if( state!=laststate_left)
  {
    (((state&(1<<encA_left))>>encA_left)^((state&(1<<encB_left))>>encB_left))?count_left--:count_left++;
    laststate_left=state;
  }
 
}



void counterCommoun()
{


}


void counter_right2()
{
  byte state =getStateEncoder(PIND,PINH);
  
  int encA_right = 1;
  int encB_right = 1;
  
  if( state!=laststate_right)
  {
    (((state&(1<<encA_right))>>encA_right)^((state&(1<<encB_right))>>encB_right))?count_right--:count_right++;
    laststate_right=state;
  }

  
}




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


