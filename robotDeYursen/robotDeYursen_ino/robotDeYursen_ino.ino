#include <NewPing.h>
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 


#define TRIGGER_PIN_RIGHT  3  // Arduino pin tied to trigger pin on the ultrasonic sensor   7.
#define ECHO_PIN_RIGHT     4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define TRIGGER_PIN_LEFT  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_LEFT     7  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define SERVO_WHEEL     9
int val = 15;
 
NewPing sonarLeft(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE); // NewPing setup of pins and maximum distance.




void setup()
{
  
  Serial.begin(9600); // Définit vitesse de transmission série
  myservo.attach(SERVO_WHEEL);  // attaches the servo on pin 9 to the servo object 
  
  
}


double getSonarDistance(NewPing sonar )
{
  delay(150);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  if(uS == 0)
  {
    uS = 5000;
    }
  return sonar.convert_cm(uS);
 }

double getSteeringAngle(double rightObstacle, double leftObstacle)
{

//   double threashouldDistance = 18.0;
//  
//   if(leftObstacle > threashouldDistance)  leftObstacle  = 100000;
//   if(rightObstacle > threashouldDistance) rightObstacle = 100000;
//   double angle = 90.0 * ( threashouldDistance/(threashouldDistance + leftObstacle ) - threashouldDistance/(threashouldDistance + rightObstacle )  ) + 90;

  double angle = 90 * ( exp(-0.057 * rightObstacle )  - exp(-0.057 * leftObstacle )  ) + 90 ;

  return angle;
   
 }

void loop()
{
  double anglLeft  =  getSonarDistance(sonarLeft);
  double anglRight =  getSonarDistance(sonarRight); 

  Serial.print("Ping Left: ");
  Serial.print(anglLeft); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.print("cm");
  Serial.print("Ping Right: ");
  Serial.print(anglRight ); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.print("cm");    
  
  double angle = getSteeringAngle(anglLeft,anglRight);
  
  Serial.print("  angle :");
  Serial.println(angle);
  
  myservo.write(angle ) ; 
  
}
