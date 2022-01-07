/**
 * Example of using the EncoderButton library to accelerate.
 */
#include <EncoderButton.h>

/**
 * Instatiate an EncoderButton.
 * For Arduino Uno, the hardware interrupts are pins 2 & 3
 * For Teensy, you can use any digital pin.
 */
EncoderButton eb1(2, 3); //Two args means encoder only but will work for an encoder with a button too.

int myValue = 0;


/**
 * A function to handle the 'encoder' event
 */
void onEb1Encoder(EncoderButton& eb) {
  myValue += eb.increment()*abs(eb.increment());
  Serial.print("eb1 incremented by: ");
  Serial.print(eb.increment());
  Serial.print(" accelerated to: ");
  Serial.print(eb.increment()*abs(eb.increment()));
  Serial.print(" myValue is now: ");
  Serial.println(myValue);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("EncoderButton Acceleration Example");

  //Link the event(s) to your function
  eb1.setEncoderHandler(onEb1Encoder);
  //Set the rate limit on your encoder.
  eb1.setRateLimit(200);
  //If set to 200 milliseconds, the increment() will be > 1 if the encoder rotation clicks more than 5 time/second
}

void loop() {
  // put your main code here, to run repeatedly:
  // You must call update() for every defined EncoderButton.
  // This will update the state of the encoder & button and 
  // fire the appropriate events.
  eb1.update();
}
