/**
 * Example using the EncoderButton library with a class method 
 * as a the callback function.
 * **NOTE** Only available for ESP8266/32 and Teensy
 */
#include <EncoderButton.h>

/**
 * Instatiate an EncoderButton.
 * For Teensy, you can use any digital pin.
 */
EncoderButton eb1(6,7,8);

/**
 * A noddy class for this example
 */
class Foo {

  public:
    /**
     * A class method to handle the 'clicked' event
     * Can be called anything but requires EncoderButton& 
     * as its only parameter.
     * 
      */
    void onEb1Clicked(EncoderButton& eb) {
      Serial.print("eb1 clickCount: ");
      Serial.println(eb.clickCount());
    }
    
    /**
     * A class method to handle the 'encoder' event
     */
    void onEb1Encoder(EncoderButton& eb) {
      Serial.print("eb1 incremented by: ");
      Serial.println(eb.increment());
      Serial.print("eb1 position is: ");
      Serial.println(eb.position());
    }

};

/**
 * Instantiate the class
 */
Foo foo;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("EncoderButton Class Method Callback Example");

  //Link the event(s) to your class method
  //Not quite as simple as a static function
  eb1.setClickHandler([&](EncoderButton &btn) {
    foo.onEb1Clicked(btn);
  });

  // And the encoder event
  eb1.setEncoderHandler([&](EncoderButton &btn) {
    foo.onEb1Encoder(btn);
  });

}

void loop() {
  // put your main code here, to run repeatedly:
  // You must call update() for every defined EncoderButton.
  // This will update the state of the encoder & button and 
  // fire the appropriate events.
  eb1.update();
}
