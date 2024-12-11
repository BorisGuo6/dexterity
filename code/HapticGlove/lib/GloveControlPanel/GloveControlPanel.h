#ifndef _GLOVECONTROLPANEL_H_
#define _GLOVECONTROLPANEL_H_

#include <Arduino.h>
#include <FastLED.h>
#include <haptic_glove_pins.h>

#define INT_BLUE 0,0,255
#define INT_RED 255,0,0
#define INT_PINK 193,28,132
#define INT_ORANGE 255,102,0
#define INT_PURPLE 190,0,255
#define INT_GREEN 0,255,0

/**
 * Class to provide access to the switch, buttons, and LEDs (including the internal LED) available om the control glove
 */
class GloveControlPanel {
    public:

        /**
         * Default constructor which sets the following default parameters:
         * 
         * switch_pin = SWITCH_0 (9)
         * 
         * button_0_pin = BUTTON_0 (10)
         * 
         * button_1_pin = BUTTON_1 (11)
         * 
         * led_0_pin = LED_0 (12)
         * 
         * led_1_pin = LED_1 (13)
         * 
         * button_0_mode = FALLING
         * 
         * button_1_mode = FALLING
         * 
         * @param button_0_isr Function pointer to the ISR to be run when button 0 is pressed
         * @param button_1_isr Function pointer to the ISR to be run when button 1 is pressed
         */
        GloveControlPanel(void (*button_0_isr)(), void (*button_1_isr)());

        /**
         * Overloaded constructor for specifying parameters other than the defaults
         */
        GloveControlPanel(uint8_t switch_pin, uint8_t button_0_pin, uint8_t button_1_pin, uint8_t led_0_pin, uint8_t led_1_pin,
        void (*button_0_isr)(), void (*button_1_isr)(), uint8_t button_0_mode, uint8_t button_1_mode);
      
        ~GloveControlPanel();

        /**
         * Configures pins, the internal LED, and ISRs. This function must be run before calling any other methods.
         */
        void initialize();

        /**
         * @return The current digital value (0 or 1) of the switch
         */
        int8_t readSwitch();

        /**
         * Read the current digital value on the pin corresponding to `button_num`
         * @param button_num 0 for button 0, 1 for button 1
         * @return The button's current digital value (0 or 1), or -1 if the value of `button_num` is invalid
         */
        int8_t readButton(uint8_t button_num);

       /**
         * Toggle (external) LED number `led_num`
         * @param led_num 0 for LED 0, 1 for LED 1
         */
        void toggleExtLED(uint8_t led_num);

        /**
         * Set the digital value of LED number `led_num` to `val`
         * @param led_num 0 for LED 0, 1 for LED 1
         * @param val Digital value (0 or 1) to which to set the LED
         */
        void setExtLED(uint8_t led_num, uint8_t val);

        /**
         * Read the current digital value on the pin corresponding to `led_num`
         * @param led_num 0 for LED 0, 1 for LED 1
         * @return The LED's current digital value (0 or 1), or -1 if the value of `led_num` is invalid
         */
        int8_t readExtLED(uint8_t led_num);

         /**
         * Toggle the internal LED, preserving the color it has when on
         */
        void toggleIntLED();

        /**
         * Set the internal LED to on or off
         * @param val 0 for completely off (R, G, B = 0, 0, 0) or 1 for completely on (R, G, B = 255, 255, 255)
         */
        void setIntLED(uint8_t val);

        /**
         * Set the RGB color value of the internal LED
         * @param red Red value from 0-255
         * @param green Green value from 0-255
         * @param blue Blue value from 0-255
         */
        void setIntLEDColor(uint8_t red, uint8_t green, uint8_t blue);
        
        /**
         * @return The current on-off state of the internal LED, where 0 is completely off and 1 is on to any extent
         */
        int8_t readIntLED();

        /**
         * @return The current color value of the internal LED
         */
        CRGB getIntLEDColor();

    private:
        uint8_t switch_pin;
        uint8_t button_0_pin;
        uint8_t button_1_pin;
        uint8_t led_0_pin;
        uint8_t led_1_pin;
        void (*button_0_isr)();
        void (*button_1_isr)();
        uint8_t button_0_mode;
        uint8_t button_1_mode;
        CRGB int_led_color = CRGB(0, 0, 0);
        CRGB int_led_on_color = CRGB(255, 255, 255);
};

#endif