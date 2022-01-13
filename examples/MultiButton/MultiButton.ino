/**
 * This example demostrates how to create multiple encoders or buttons.
 * 
 * ** NOTE ** 
 * This example uses a single encoder and multiple buttons because UNOs and other Arduinos 
 * with limited hardware interrupts will struggle to perform well (read: reliably) with 
 * multiple encoders. 
 * ** Teensys will handle multiple encoders without any issues *** (they have hardware 
 * interupts on all digital pins).
 * See https://www.pjrc.com/teensy/td_libs_Encoder.html for details.
 * 
 */
#include <EncoderButton.h>

/**
 * Instatiate an EncoderButton.
 * For Arduino Uno, the hardware interrupts are pins 2 & 3
 * For Teensy, you can use any digital pin.
 */
EncoderButton firstEncoderButton(2, 3, 4);
EncoderButton secondButton(14); //The first button is on the encoder!
//. . .
EncoderButton nthButton(15);


/**
 * A function to handle the first encoder 'encoder' event
 */
void onFirstEncoder(EncoderButton& eb) {
  Serial.print("first encoder incremented by: ");
  Serial.println(eb.increment());
  Serial.print("first encoder position is: ");
  Serial.println(eb.position());
}

/**
 * A function to handle the first encoder 'clicked' event
 */
void onFirstButtonClicked(EncoderButton& eb) {
  Serial.print("First button clickCount: ");
  Serial.println(eb.clickCount());
}


/**
 * A function to handle the second button 'clicked' event
 */
void onSecondButtonClicked(EncoderButton& eb) {
  Serial.print("Second button clickCount: ");
  Serial.println(eb.clickCount());
}

/**
 * A function to handle the second button 'clicked' event
 */
void onNthButtonClicked(EncoderButton& eb) {
  Serial.print("Nth button clickCount: ");
  Serial.println(eb.clickCount());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("EncoderButton multiple encoder/button example");

  //Link the event(s) to your function
  firstEncoderButton.setEncoderHandler(onFirstEncoder);
  firstEncoderButton.setClickHandler(onFirstButtonClicked);
  secondButton.setClickHandler(onSecondButtonClicked);
  nthButton.setClickHandler(onNthButtonClicked);



}

void loop() {
  // put your main code here, to run repeatedly:
  // You must call update() for every defined EncoderButton.
  firstEncoderButton.update();
  secondButton.update();
  nthButton.update();
}
