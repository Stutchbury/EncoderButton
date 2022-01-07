/**
 * Example of using the EncoderButton long press repeat.
 */
#include <EncoderButton.h>

/**
 * Instatiate an EncoderButton.
 * For Arduino Uno, the hardware interrupts are pins 2 & 3
 * For Teensy, you can use any digital pin.
 */
EncoderButton button1(4); //Single arg means button only but will work the same with the button on an encoder


/**
 * A function to handle the 'clicked' event
 * Can be called anything but requires EncoderButton& 
 * as its only parameter.
 * I tend to prefix with 'on' and suffix with the 
 * event type.
 */
void onButton1LongPress(EncoderButton& eb) {
  Serial.print("button1 longPressCount: ");
  Serial.println(eb.longPressCount());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("EncoderButton Long Press Repeat Example");

  //Link the event(s) to your function
  button1.setLongPressHandler(onButton1LongPress, true); //Second arg means repeat the long click
}

void loop() {
  // put your main code here, to run repeatedly:
  // You must call update() for every defined EncoderButton.
  // This will update the state of the encoder & button and 
  // fire the appropriat events.
  button1.update();
}
