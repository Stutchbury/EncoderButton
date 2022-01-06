/**
 * A basic example of using the EncoderButton library.
 */
#include <EncoderButton.h>

//Probably better to pick a more meaningful name than 'eb1'...
EncoderButton eb1(6, 7, 8);

/**
 * A function to handle the 'clicked' event
 * Can be called anything but requires EncoderButton& 
 * as its only parameter.
 * I tend to prefix with 'on' and suffix with the 
 * event type.
 */
void onEb1Clicked(EncoderButton& eb) {
  Serial.printf("eb1 clickCount: %i, \n", eb.clickCount());
}

/**
 * A function to handle the 'encoder' event
 */
void onEb1Encoder(EncoderButton& eb) {
  Serial.printf("eb1 incremented by: %i \n", eb.increment());
  Serial.printf("eb1 position is: %i \n", eb.position());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
