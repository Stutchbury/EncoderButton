/**
 * A basic example of enabling or disabling an EncoderButton.
 */
#include <EncoderButton.h>

/**
 * Instantiate an EncoderButton.
 * For Arduino Uno, the hardware interrupts are pins 2 & 3
 * For Teensy, you can use any digital pin.
 * 
 * I've set this us as an encoder only because I'm using the
 * button to toggle enable/disable.
 */
EncoderButton eb1(2, 3);


/** 
 * Instantiate the button to enable or disable the encoder above
 */
EncoderButton sw1(4);


/**
 * A function to handle the 'clicked' event
 * Can be called anything but requires EncoderButton& 
 * as its only parameter.
 * I tend to prefix with 'on' and suffix with the 
 * event type.
 */
void onSw1Clicked(EncoderButton& eb) {
  //Toggle enable for the encoder eb1
  //Note: this is not the EncoderButton 'eb' passed to the function.
  eb1.enabled() ? eb1.enable(false) : eb1.enable();
}

/**
 * A function to handle the 'encoder' event
 */
void onEb1Encoder(EncoderButton& eb) {
  //To show the increment is not changed while disabled.
  Serial.print("eb1 incremented by: ");
  Serial.println(eb.increment());
  Serial.print("eb1 position is: ");
  Serial.println(eb.position());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("EncoderButton Enable/Disable Example");

  //Link the event(s) to your function
  sw1.setClickHandler(onSw1Clicked);
  eb1.setEncoderHandler(onEb1Encoder);
}

void loop() {
  // put your main code here, to run repeatedly:
  // You must call update() for every defined EncoderButton.
  // This will update the state of the encoder & button and 
  // fire the appropriate events.
  sw1.update();
  eb1.update();
}
