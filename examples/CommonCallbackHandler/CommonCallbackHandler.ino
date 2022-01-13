/**
 * An example of using a common callback for an event in the  EncoderButton library
 * Can be used for multi-function buttons (ie button actions are changed at runtime).
 * 
 * **NOTE**
 * I am using buttons as this works well on all devices but the same principle applies to 
 * encoders (multiple encoders work well on Teensies, not so much on UNOs.
 * See https://www.pjrc.com/teensy/td_libs_Encoder.html for why.
 * 
 * Also note: I am using an array to store the button (or encoder) instances but this is 
 * not required.
 * 
 */
#include <EncoderButton.h>

/**
 * Instatiate multiple EncoderButtons. We're using just buttons here but the same applies
 * to encoders or encoders+buttons.
 */
EncoderButton buttonRow[5] = { EncoderButton(28), EncoderButton(27), EncoderButton(26), EncoderButton(25), EncoderButton(24) };


/**
 * A common function to handle the 'clicked' event of all buttons.
 */
void onButtonRowClicked(EncoderButton& eb) {
  Serial.print("button ");
  Serial.print(eb.userId());
  Serial.print(" clickCount: ");
  Serial.println(eb.clickCount());
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("EncoderButton common callback handler example");

  //Loop through the array of encoder buttons.
  for ( uint8_t b=0; b<5; b++ ) {
    // Give each button a userId - this defaults to 0 so 
    // must be set if you want to identify a button in the callback.
    buttonRow[b].setUserId(b);
    // Set all buttons in the button row to use the same handler
    buttonRow[b].setClickHandler(onButtonRowClicked);
  }
}

void loop() {
  //Update all buttons/encoders
  for ( uint8_t b=0; b<5; b++ ) {
    buttonRow[b].update();
  }
}
