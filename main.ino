#include <EEPROM.h>

int trigPin = 3; /* Trig pin of the ultrasonic sensor */
int echoPin = 4; /* Echo pin of the ultrasonic sensor */

long lastDistance = 0; /* Last distance detected, useful for intrusion checking */
unsigned char intrusions = 0; /* Intrusions counter */
int IntEEPPos = 0; /* Intrusions EEPROM Position */

int silentMode = 1; /* Silent mode: If it is set 0 the LED on pin 13 will blink when an intrusion is detected */

void usage(){
  Serial.println("");Serial.println("");
  Serial.println("Frank's physical intrusion detector v0.1:");
  Serial.println("s - Toggle silent mode");
  Serial.println("c - Clear intrusions log");
  Serial.println("i - Show how many intrusions were detected");
  Serial.println("d - Show the last detected distance");
  Serial.println("h - Show this menu");
}

void setup(){

  Serial.begin(9600);       /* Beginning the Serial */
  usage();                  /* Show a brief usage */


  pinMode( trigPin, OUTPUT );
  pinMode( echoPin, INPUT );
  pinMode( 13, OUTPUT ); /* Setting the pin 13 (LED) to OUTPUT */
  intrusions = EEPROM.read(IntEEPPos); /* Reading the previously detected intrusions counter from the EEPROM */
}

void loop(){

  long duration, dist; /* Duration of the ultrasonic impulse and distance (cm) */

  /* Sending a 10us impulse */
  digitalWrite( trigPin, LOW );
  digitalWrite( trigPin, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( trigPin, LOW );

  duration = pulseIn( echoPin, HIGH );
  if( duration > 38000 ) /* If the duration is over 38ms the sensor is out of range */
    return;

  dist = 0.034 * duration / 2; /* Calculating the distance */

  if( dist != lastDistance && lastDistance - dist > 10 ){ /* Intrustion detected */
    int i;
    if(!silentMode) /* Blinking LED 13 if not in silent mode */
      for(i=0; i<20; i++){
        digitalWrite( 13, HIGH );
        delay( 30 );
        digitalWrite( 13, LOW );
        delay( 30 );
      }
    /* Updating the intrusions counter into memory and EEPROM */
    intrusions++;
    EEPROM.update(IntEEPPos, intrusions);
  }

  lastDistance = dist; /* Useful for detecting differences with the next distance */

  if(Serial.available()){ /* If there is user input */
    char c = Serial.read();
    switch( c ){

      case 's': /* Toggle silent mode */
        silentMode = silentMode ? 0 : 1;
        if( silentMode )
          Serial.println("Silent mode activated");
        else
          Serial.println("Silent mode deactivated");
      break;

      case 'i': /* Show how many intrusions have been detected */
        Serial.print("There have been detected ");
        Serial.print(intrusions);
        Serial.println(" intrusions");
      break;

      case 'd': /* Show the last detected distance, useful for debugging */
        Serial.print("The last detected distance was ");
        Serial.print(lastDistance);
        Serial.println(" cm");
      break;

      case 'c': /* Clear the intrusions counter. TODO: add security */
        intrusions = 0;
        EEPROM.update(IntEEPPos, intrusions);
        Serial.println("Intrusions log cleared");
      break;

      case 'h': /* Usage */
        usage();
      break;

      default:
        Serial.println("Unknown command");
      break;
    }
  }

  delay( 500 ); /* Delay some ms before detecting the distance again */
}
