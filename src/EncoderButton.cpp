/** 
 * 
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2022 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EncoderButton.h"


EncoderButton::EncoderButton(byte encoderPin1, byte encoderPin2, byte switchPin )
  : encoder(new Encoder(encoderPin1, encoderPin2)), bounce(new Bounce()) {
  haveEncoder = true;
  haveButton = true;    
  // Top tip From PJRC's Encoder - without this delay the
  // long-press doesn't fire on first press.
  // allow time for a passive R-C filter to charge
  // through the pullup resistors, before reading
  // the initial state
  pinMode(switchPin, INPUT_PULLUP); //Set pullup first
  delayMicroseconds(2000); //Delay
  bounce->attach(switchPin, INPUT_PULLUP); //then attach button
}

EncoderButton::EncoderButton(byte encoderPin1, byte encoderPin2 )
  : encoder(new Encoder(encoderPin1, encoderPin2)) {
  haveEncoder = true;
}

EncoderButton::EncoderButton(byte switchPin )
  : bounce(new Bounce()) {
  haveButton = true;    
  pinMode(switchPin, INPUT_PULLUP); //Set pullup first
  delayMicroseconds(2000); //Delay
  bounce->attach(switchPin, INPUT_PULLUP); //then attach button
}

void EncoderButton::update() {
  if ( _enabled ) {
    //button update (fires pressed/released callbacks)
    if ( haveButton && bounce->update() ) {
      lastEventMs = millis();
      idleFlagged = false;    
      if (changed_cb != NULL) changed_cb (*this);
      _buttonState = bounce->read();
      if ( bounce->fell() ) {
        previousState = HIGH;
        if (pressed_cb != NULL) pressed_cb (*this);
      } else if (bounce->rose() ) {
        if ( encodingPressed ) {
          encodingPressed = false;
          prevClickCount = 0;
          clickCounter = 0;
          if (encoder_released_cb != NULL) encoder_released_cb (*this);
        } else {
          if ( previousState == HIGH ) {
            clickFired = false;
            clickCounter++;
          }
          if (released_cb != NULL) released_cb (*this);
        }
        previousState = LOW;
      }
    }
    //encoder udate (fires encoder rotation callbacks)
    if ( haveEncoder &&  (millis() - rateLimitCounter) >= rateLimit ) {
      long newPosition = floor(encoder->read()/positionDivider);
      if (newPosition != encoderPosition) {
        encoderIncrement = (newPosition - encoderPosition); 
        encoderPosition = newPosition;
        idleFlagged = false;    
        lastEventMs = millis();
        if ( _buttonState == HIGH ) {
          currentPosition += encoderIncrement;
          if (encoder_cb != NULL) encoder_cb (*this);
        } else {
          encodingPressed = true;
          currentPressedPosition += encoderIncrement;
          if (encoder_pressed_cb != NULL) encoder_pressed_cb (*this);
        }
      }
      rateLimitCounter = millis();
    }
    //fire long press callbacks
    if (haveButton && !encodingPressed && LOW == bounce->read() ) {
      if ( bounce->currentDuration() > (longClickDuration * (longPressCounter+1)) ) {
        lastEventMs = millis();
        if ( (repeatLongPress || longPressCounter == 0 ) && long_press_cb != NULL) {
          long_press_cb (*this);
        }
        longPressCounter++;
      }
    }
    //fire button click callbacks
    if ( haveButton && !clickFired && _buttonState == HIGH && bounce->currentDuration() > multiClickInterval ) {
      clickFired = true;
      if ( bounce->previousDuration() > longClickDuration ) {
        clickCounter = 0;
        prevClickCount = 1;
        longPressCounter = 0;
        if (long_click_cb != NULL) long_click_cb (*this);
      } else {
        prevClickCount = clickCounter;
        if (clickCounter == 3 && triple_click_cb != NULL) {
          triple_click_cb (*this);
        } else if ( clickCounter == 2 && double_click_cb != NULL) {
          double_click_cb (*this);
        } else {
          if (click_cb != NULL) click_cb (*this);
        }
        clickCounter = 0;
      }
    }
    //fire idle timeout callback
    if ( !idleFlagged && idle_cb != NULL && msSinceLastEvent() > idleTimeout ) {
      idleFlagged = true;
      idle_cb (*this);
    }
  }
}

void EncoderButton::setChangedHandler(CallbackFunction f) { changed_cb = f; }

void EncoderButton::setPressedHandler(CallbackFunction f) { pressed_cb = f; }

void EncoderButton::setReleasedHandler(CallbackFunction f) { released_cb = f; }

void EncoderButton::setClickHandler(CallbackFunction f) { click_cb = f; }

void EncoderButton::setLongClickHandler(CallbackFunction f) { long_click_cb = f; }

void EncoderButton::setDoubleClickHandler(CallbackFunction f) { double_click_cb = f; }

void EncoderButton::setTripleClickHandler(CallbackFunction f) { triple_click_cb = f; }

void EncoderButton::setLongPressHandler(CallbackFunction f, bool repeat /*=false*/) {
  long_press_cb = f;
  repeatLongPress = repeat;
}

void EncoderButton::setEncoderHandler(CallbackFunction f) { encoder_cb = f; }

void EncoderButton::setEncoderPressedHandler(CallbackFunction f) { encoder_pressed_cb = f; }

void EncoderButton::setEncoderReleasedHandler(CallbackFunction f) { encoder_released_cb = f; }

void EncoderButton::setIdleHandler(CallbackFunction f) { idle_cb = f; }

void EncoderButton::setDebounceInterval(unsigned int intervalMs) { bounce->interval(intervalMs); }

void EncoderButton::setMultiClickInterval(unsigned int intervalMs) { multiClickInterval = intervalMs; }

void EncoderButton::setLongClickDuration(unsigned int longDurationMs) { longClickDuration = longDurationMs; }

void EncoderButton::setLongPressRepeat(bool repeat /*=false*/) { repeatLongPress = repeat; }

void EncoderButton::setRateLimit(long ms) { rateLimit = ms; }

void EncoderButton::useQuadPrecision(bool prec) { positionDivider = (prec?1:4); }

void EncoderButton::resetPosition(long pos) {
  encoder->readAndReset();
  encoderPosition = 0;
  currentPosition = pos;
}

void EncoderButton::resetPressedPosition(long pos) {
  encoder->readAndReset();
  encoderPosition = 0;
  currentPressedPosition = pos;  
}

void EncoderButton::setIdleTimeout(unsigned int timeoutMs) { idleTimeout = timeoutMs; }

void EncoderButton::setUserId(unsigned int identifier) { _userId = identifier; }

void EncoderButton::setUserState(unsigned int s) { _userState = s; }

bool EncoderButton::buttonState() { return bounce->read(); }

unsigned char EncoderButton::clickCount() { return prevClickCount; }

unsigned long EncoderButton::currentDuration() { return bounce->currentDuration(); }

unsigned long EncoderButton::previousDuration() { return bounce->previousDuration(); }

uint8_t EncoderButton::longPressCount() { return longPressCounter+1; }

bool EncoderButton::isPressed() { return buttonState() == LOW; }

int16_t EncoderButton::increment() { return encoderIncrement; }

long EncoderButton::position() { return currentPosition; }

long EncoderButton::pressedPosition() { return currentPressedPosition; }

unsigned long EncoderButton::msSinceLastEvent() { return millis() - lastEventMs; }

unsigned int EncoderButton::userId() { return _userId; }

unsigned int EncoderButton::userState() { return _userState; }

bool EncoderButton::enabled() { return _enabled; }

void EncoderButton::enable(bool e) { 
  _enabled = e;
  if ( e == true ) {
    //Reset the encoder so we don't trigger an event
    encoder->write(encoderPosition*positionDivider);
  }
}


