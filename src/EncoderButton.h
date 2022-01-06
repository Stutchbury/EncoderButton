/**
 * A lossless event based rotary encoder button wrapper for 
 * Paul Stoffregen's Encoder library 
 * https://www.pjrc.com/teensy/td_libs_Encoder.html
 * and Thomas Fredericks' Bounce2 library
 * https://github.com/thomasfredericks/Bounce2
 * and inspired by Lennart Hennigs Button2
 * https://github.com/LennartHennigs/Button2
 * 
 * Written for Teensy but tested on Arduino Uno and others.
 * 
 * It is important, but not essential, to use hardware interrupt pins (any 
 * pin on a Teensy, pins 2 & 3 on an UNO). The library will work on software 
 * interrupts but may not avoid loss of steps.
 * Do not use setRateLimit() with software interrupts.
 * See https://www.pjrc.com/teensy/td_libs_Encoder.html
 * for more details.
 * 
 * Works with Encoder+button, Encoder alone or just a Button.
 * 
 * Encoder can fire rotation callbacks when pressed or not and optionally 
 * in quadrature mode (default is once per click).
 * 
 * Button events are fired for both the encoder button and/or a standalone
 * momentary switch.
 * 
 */

#ifndef EncoderButton_h
#define EncoderButton_h

#include "Arduino.h"
#include <memory> //for std::unique_ptr

//Standing on the shoulders of giants
// http://www.pjrc.com/teensy/td_libs_Encoder.html
#ifndef Encoder_h
  #include <Encoder.h>
#endif

#ifndef Bounce2_h
  #include <Bounce2.h>
#endif

class EncoderButton {

  protected:
    /**
     * Callback functions receive an EncoderButton as their single argument
     */
    using CallbackFunction = void (*) (EncoderButton&);

  public:
    /**
     * Construct a rotary encoder with a button
     */
    EncoderButton(byte encoderPin1, byte encoderPin2, byte switchPin);

    /**
     * Construct a rotary encoder without a button
     */
    EncoderButton(byte encoderPin1, byte encoderPin2);

    /**
     * Construct a button only
     */
    EncoderButton(byte switchPin);


    /**
     * Read the position of the encoder and update the state of the button.
     * Call from loop() for each defined EncoderButton
     */
    void update();
    
    /** ***************************************************
     * set button callback handlers
     */
    /**
     * Fires when button state changes
     */
    void setChangedHandler(CallbackFunction f);
    
    /**
     * Fires after button is pressed down
     */
    void setPressedHandler(CallbackFunction f);
    
    /**
     * Fires after button is released.
     * Note: if the encoder is turned while pressed, then 
     * the 'encoder released' callback is fired instead.
     */
    void setReleasedHandler(CallbackFunction f);
    
    /**
     * Fires after button is clicked (when pressed duration
     * is less than setLongClickDuration() (default 750ms)
     * Note: the number of multi clicks can be read from
     * EncoderButton.clickCount() in the callback so you are not 
     * limited to double or triple clicks - any number can be actioned.
     * If double or triple click callbacks are set, this will not be
     * fired for those events.
     */
    void setClickHandler(CallbackFunction f);
    
    /**
     * Fires after button is double clicked
     * Determined by setMultiClickInterval() (default 250ms)
     * Syntactic sugar for click handler + clickCount == 2
     */
    void setDoubleClickHandler(CallbackFunction f);
    
    /**
     * Fires when button is triple clicked
     * Syntactic sugar for click handler + clickCount == 3
     */
    void setTripleClickHandler(CallbackFunction f);

    /**
     * Fires after button is long clicked (when pressed duration
     * is greater than setLongClickDuration(ms) (default 750ms)
     * See also long press handler
     */
    void setLongClickHandler(CallbackFunction f);
    
    /**
     * Fired *while* button is long pressed. By default, this fires
     * once but can fire every setLongClickDuration by passing 'true'
     * as a second argument.
     * Use longPressCount() in callback to read the number of times fired
     */
    void setLongPressHandler(CallbackFunction f, bool repeat=false);
    
    /** ***************************************************
     * set encoder callback handlers
     */

    /**
     * Fired when the encoder is turned. By default is fired once
     * per 'click' but can fire full quadrature encoding if 
     * useQuadPrecision(bool) is set to 'true'
     */
    void setEncoderHandler(CallbackFunction f);

    /**
     * Fired when the encoder is turned while pressed
     */
    void setEncoderPressedHandler(CallbackFunction f);

    /**
     * Fired after encoder is released *and turned* (if not turned 
     * the button 'releaded' handler is fired)
     */
    void setEncoderReleasedHandler(CallbackFunction f);
    
    /** ***************************************
     * Fired after the encoder or button has been idle for 
     * setIdleTimeout(ms) (default 10 seconds)
     */
    void setIdleHandler(CallbackFunction f);    

    
    /** ***************************************
     *  button setup
     */

    /**
     * Default is set in the Bounce2 library (currently 10ms)
     */
    void setDebounceInterval(unsigned int intervalMs);

    /**
     * Set the interval in ms between double, triple or
     * multi clicks
     */
    void setMultiClickInterval(unsigned int intervalMs);

    /**
     * Set the ms that defines a lonf click. Long pressed callback
     * will be fired at this interval if repeat is set to true via the
     * setLongPressHandler()
     */
    void setLongClickDuration(unsigned int longDurationMs);

    /**
     * Choose whether to repeat the long press callback
     * (default is 'false')
     */
    void setLongPressRepeat(bool repeat=false);
    
    /** ***************************************
     *  encoder setup
     */

    /**
     * Encoder callbacks are normally fired on every loop() but for MPG
     * style encoders this can fire a huge number of events (that may 
     * swamp a serial connection).
     * The encoder interupts will sitll be called but this will limit 
     * the call back firing to every set ms - read the 
     * EncoderButton.increment() for lossless counting of encoder.
     * Set to zero (default) for no rate limit.
     */
    void setRateLimit(long ms);

    /**
     * Quadrature encoders have four states for each 'click' of the 
     * rotary switch. By default we only fire once per click.
     * Set this to 'true' if you want four events per click.
     * Affects pressed+turning too.
     */
    void useQuadPrecision(bool prec);

    /**
     * Reset the counted position of the encoder. 
     */
    void resetPosition(long pos = 0);
    
    /**
     * Reset the counted pressed position of the encoder. 
     */
    void resetPressedPosition(long pos = 0);    

    /**
     * Set the idle timeout in ms (default 10000) 
     */
    void setIdleTimeout(unsigned int timeoutMs);
    
    /** ***************************************
     *  button state
     */
     
    /**
     * Directly get the current button state from Bounce2
     */
    bool buttonState();

    /**
     * Directly get the duration of the button current state from Bounce2
     */
    unsigned long currentDuration();

    /**
     * Directly get the duration of the button previous state from Bounce2
     */
    unsigned long previousDuration();

    /**
     * The number of multi-clicks that have been fired in the clicked event
     */
    unsigned char clickCount();

    
    /**
     * The number of times the long press handler has  been fired in the 
     * button pressed event
     */
    uint8_t longPressCount();
    
    /** ***************************************
     *  encoder state
     */

    /**
     * Returns a positive (CW) or negative (CCW) integer. Is normally 1 or -1 but if your 
     * loop() has lots of processing, your Arduino is slow or you setRateLimit()
     * this will report the actual number of increments made by the encoder since
     * the last encoder handler event.
     */
    int16_t increment();

    /**
     * The current position of the encoder. Can be reset by resetPosition()
     */
    long position();

    /**
     * The current position of the encoder when pressed. Can be reset by resetPressedPosition()
     */
    long pressedPosition();
    

    /** ***************************************
     * Returns the number of ms since any event was fired for this encoder/button
     */
    unsigned long msSinceLastEvent();



  protected:
    
    CallbackFunction changed_cb = NULL;
    CallbackFunction pressed_cb = NULL;
    CallbackFunction released_cb = NULL;
    CallbackFunction click_cb = NULL;
    CallbackFunction long_click_cb = NULL;
    CallbackFunction double_click_cb = NULL;
    CallbackFunction triple_click_cb = NULL;
    CallbackFunction long_press_cb = NULL;
    //Encoder
    CallbackFunction encoder_cb = NULL;
    CallbackFunction encoder_pressed_cb = NULL;
    CallbackFunction encoder_released_cb = NULL;
    //Common
    CallbackFunction idle_cb = NULL;


  private:
    std::unique_ptr<Encoder> encoder;   
    std::unique_ptr<Bounce> bounce;
    boolean haveButton = false;
    boolean haveEncoder = false;
    uint8_t positionDivider = 4;
    int32_t encoderPosition  = 0;
    int32_t currentPosition  = 0;
    int32_t currentPressedPosition  = 0;
    int encoderIncrement  = 0;
    bool encodingPressed = false;
    unsigned char _buttonState = HIGH;
    unsigned int multiClickInterval = 250;
    unsigned int longClickDuration = 750;
    bool clickFired = true;
    unsigned char clickCounter = 0;
    unsigned int longPressCounter = 0;
    unsigned long lastEventMs = millis();
    unsigned long idleTimeout = 10000;
    bool idleFlagged = false;
    bool previousState = LOW;
    unsigned char prevClickCount = 0;
    bool repeatLongPress = false;

    ulong rateLimit = 0;
    ulong rateLimitCounter = 0;    



};

#endif


