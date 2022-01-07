/**
 * A basic example of using the EncoderButton library.
 */
#include <EncoderButton.h>

/**
 * Instatiate an EncoderButton.
 * For Arduino Uno, the hardware interrupts are pins 2 & 3
 * For Teensy, you can use any digital pin.
 * Probably better to pick a more meaningful name than 'eb1'...
 * Encoder+button:
 * EncoderButton(byte encoderPin1, byte encoderPin2, byte switchPin);
 * Encoder only:
 * EncoderButton(byte encoderPin1, byte encoderPin2);
 * Button only:
 * EncoderButton(byte switchPin);
 */
EncoderButton eb1(2, 3, 4);


/**
 * A function to handle the 'clicked' event
 * Can be called anything but requires EncoderButton& 
 * as its only parameter.
 * I tend to prefix with 'on' and suffix with the 
 * event type.
 */
void onEb1Clicked(EncoderButton& eb) {
  Serial.print("eb1 clickCount: ");
  Serial.println(eb.clickCount());
}

/**
 * A function to handle the 'encoder' event
 */
void onEb1Encoder(EncoderButton& eb) {
  Serial.print("eb1 incremented by: ");
  Serial.println(eb.increment());
  Serial.print("eb1 position is: ");
  Serial.println(eb.position());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("EncoderButton Basic Example");

  //Link the event(s) to your function
  eb1.setClickHandler(onEb1Clicked);
  eb1.setEncoderHandler(onEb1Encoder);
}

void loop() {
  // put your main code here, to run repeatedly:
  // You must call update() for every defined EncoderButton.
  // This will update the state of the encoder & button and 
  // fire the appropriat events.
  eb1.update();
}
