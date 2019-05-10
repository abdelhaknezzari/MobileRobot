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





void loop()
{

    
  delay(100);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonarLeft.ping(); // Send ping, get ping time in microseconds (uS).
  unsigned int uS2 = sonarRight.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping Left: ");
  Serial.print(sonarLeft.convert_cm(uS)); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.print("cm");
  Serial.print("Ping Right: ");
  Serial.print(sonarRight.convert_cm(uS2)); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.print("cm");    
  

  
  double anglLeft  = sonarLeft.convert_cm(uS);
  double anglRight = sonarRight.convert_cm(uS2);
  

  if(anglLeft > 15) anglLeft = 1000;
  if(anglRight > 15) anglRight = 1000;

  double angle = 90.0 * ( 15.0/(15.0 + anglLeft ) - 15.0/(15.0 + anglRight )  );
  if(angle < 0)
  {
    angle += 360;
  }
  
  Serial.print("  angle :");
  Serial.println(angle);
  
  myservo.write(angle ) ; 
  


}
