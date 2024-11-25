#ifndef _GLOVECONTROLPANEL_H_
#define _GLOVECONTROLPANEL_H_

#include <Arduino.h>
#include <FastLED.h>
#include <haptic_glove_pins.h>

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
         * @return The current digital value (0 or 1) on the switch's pin
         */
        int8_t readSwitch();

        /**
         * Read the current digital value on `button_pin`
         * @param button_pin Pin number for the button
         * @return `button_pin`'s current digital value (0 or 1), or -1 if the value of `button_pin` is invalid
         */
        int8_t readButton(uint8_t button_pin);

        /**
         * Toggle the (external) LED connected to `led_pin`
         * @param led_pin Pin number for the LED
         */
        void toggleExtLED(uint8_t led_pin);

        /**
         * Set the digital value of `led_pin` to `val`
         * @param led_pin Pin number for the LED
         * @param val Digital value (0 or 1) to which to set the LED
         */
        void setExtLED(uint8_t led_pin, uint8_t val);

        /**
         * Read the current digital value on `led_pin`
         * @param led_pin Pin number for the LED
         * @return `LED_pin`'s current digital value (0 or 1), or -1 if the value of `LED_pin` is invalid
         */
        int8_t readExtLED(uint8_t led_pin);

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