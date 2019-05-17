#include <NewPing.h>
#include <Servo.h> 
 



#define TRIGGER_PIN_RIGHT  3  // Arduino pin tied to trigger pin on the ultrasonic sensor   7.
#define ECHO_PIN_RIGHT     4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define TRIGGER_PIN_LEFT  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_LEFT     7  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define SERVO_WHEEL     9
#define SERVO_WHEEL2     5
int val = 15;
 
NewPing sonarLeft(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo myservo;  // create servo object to control a servo 
Servo myservo2;  // create servo object to control a servo 

double lastLeftDistance;
double lastRightDistance;
double threashouldDistance = 18.0;





double getSonarDistanceRight( )
{
    double anglRight =  getSonarDistance(sonarRight); 
    
    if(anglRight <= 0.0 )
    {
       anglRight = lastLeftDistance; 
   
    } else
    {
      lastLeftDistance = anglRight;
    }
    
    return anglRight;

 }


double getSonarDistanceLeft( )
{
  double anglLeft  =  getSonarDistance(sonarLeft);
  
      if(anglLeft <= 0.0 )
    {
       anglLeft = lastLeftDistance; 
   
    } else
    {
      lastLeftDistance = anglLeft;
    }
    
    return anglLeft;
    

 }
 
 

double getSonarDistance(NewPing sonar )
{
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
//  if(uS == 0)
//  {
//    uS = 5000;
//    }
  return sonar.convert_cm(uS);
 }


double functionAngle1(double rightObstacle, double leftObstacle)
{
   return   (   exp(-0.057 * rightObstacle ) *  functionAngle3(rightObstacle, leftObstacle)  -  exp(-0.057 * leftObstacle ) + 1  )  ;

  
}


double functionAngle3(double rightObstacle, double leftObstacle)
{
   return  1 -  exp( - 0.9 * abs(rightObstacle - leftObstacle) )   ;

  
}




double functionAngle2(double rightObstacle, double leftObstacle)
{
   return   threashouldDistance/(threashouldDistance + leftObstacle ) - threashouldDistance/(threashouldDistance + rightObstacle )   + 1;

}



double getSteeringAngle(double rightObstacle, double leftObstacle)
{

   
   if(leftObstacle > threashouldDistance)  leftObstacle  = 100000.0;
   if(rightObstacle > threashouldDistance) rightObstacle = 100000.0;
   
   
  return 90.0*functionAngle1(leftObstacle,rightObstacle);
   
 }
 
 
 
 
boolean getReflectanceValue(double value,char ledName)
{
 
    switch(ledName)
    {
    case '0':// En avant si touche "z"
        if(value < 668.0) return false;
        else return true;
        break; 
       
    case '1':// En arrière si touche "s"
        if(value < 668.0) return false;
        else return true;
         break; 
    case '2': 
        if(value < 668.0) return false;
        else return true;
        break; 
        
    case '3': 
      if(value < 668.0) return false;
      else return true;
      break; 
        
     case '4': 
        if(value < 668.0) return false;
        else return true;
        break; 
        
     case '5': 
        if(value < 668.0) return false;
        else return true;
        break; 
        
      case '6': 
        if(value < 668.0) return false;
        else return true;
        break; 
        
      case '7': 
        if(value < 668.0) return false;
        else return true;
        break;    
        
    }
    

}


 
 void setup()
{
  
  Serial.begin(9600); // Définit vitesse de transmission série
  myservo.attach(SERVO_WHEEL);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(SERVO_WHEEL2);  // attaches the servo on pin 9 to the servo object 
  
}
 

void loop()
{
  double anglLeft  =  getSonarDistanceLeft();
  double anglRight =  getSonarDistanceRight(); 

//  Serial.print("Ping Left: ");
//  Serial.print(anglLeft); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
//  Serial.print("cm");
//  Serial.print("Ping Right: ");
//  Serial.print(anglRight ); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
//  Serial.print("cm");    
//  
  double angle = getSteeringAngle(anglLeft,anglRight);
  
//  Serial.print("  angle :");
//  Serial.println(angle);
  
  
  
 double sensorValue0 = analogRead(A0) ;  
 double sensorValue1 = analogRead(A1) ;  
 double sensorValue2 = analogRead(A2) ;  
 double sensorValue3 = analogRead(A3) ;                             
 double sensorValue4 = analogRead(A4) ;  
 double sensorValue5 = analogRead(A5) ;  
 double sensorValue6 = analogRead(A6) ;    
 double sensorValue7 = analogRead(A7) ; 
  
 Serial.print(" A0: ");Serial.print(getReflectanceValue(sensorValue0,'0'));
 Serial.print(" A1: ");Serial.print(getReflectanceValue(sensorValue1,'1'));
 Serial.print(" A2: ");Serial.print(getReflectanceValue(sensorValue2,'2'));
 Serial.print(" A3: ");Serial.print(getReflectanceValue(sensorValue3,'3'));
 Serial.print(" A4: ");Serial.print(getReflectanceValue(sensorValue4,'4'));
 Serial.print(" A5: ");Serial.print(getReflectanceValue(sensorValue5,'5'));
 Serial.print(" A6: ");Serial.print(getReflectanceValue(sensorValue6,'6'));
 Serial.print(" A7: ");Serial.print(getReflectanceValue(sensorValue7,'7'));
 Serial.println(""); 
  
  
  
  
  
//  myservo.write(angle ) ; 
//   myservo2.write(angle ) ; 
  
}
