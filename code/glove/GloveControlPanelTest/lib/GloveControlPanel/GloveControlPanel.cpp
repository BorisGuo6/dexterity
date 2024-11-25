#include <GloveControlPanel.h>

GloveControlPanel::GloveControlPanel(void (*button_0_isr)(), void (*button_1_isr)())
{
    this->switch_pin = SWITCH_0;
    this->button_0_pin = BUTTON_0;
    this->button_1_pin = BUTTON_1;
    this->led_0_pin = LED_0;
    this->led_1_pin = LED_1;
    this->button_0_isr = button_0_isr;
    this->button_1_isr = button_1_isr;
    this->button_0_mode = FALLING;
    this->button_1_mode = FALLING;
}

GloveControlPanel::GloveControlPanel(uint8_t switch_pin, uint8_t button_0_pin, uint8_t button_1_pin, uint8_t led_0_pin, uint8_t led_1_pin,
void (*button_0_isr)(), void (*button_1_isr)(), uint8_t button_0_mode, uint8_t button_1_mode)
{
    this->switch_pin = switch_pin;
    this->button_0_pin = button_0_pin;
    this->button_1_pin = button_1_pin;
    this->led_0_pin = led_0_pin;
    this->led_1_pin = led_1_pin;
    this->button_0_isr = button_0_isr;
    this->button_1_isr = button_1_isr;
    this->button_0_mode = button_0_mode;
    this->button_1_mode = button_1_mode;
}

GloveControlPanel::~GloveControlPanel() {}

void GloveControlPanel::initialize()
{
    pinMode(switch_pin, INPUT);
    pinMode(button_0_pin, INPUT);
    pinMode(button_1_pin, INPUT);
    pinMode(led_0_pin, OUTPUT);
    pinMode(led_1_pin, OUTPUT);

    FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(&int_led_color, 1);
    FastLED.setBrightness(RGB_BRIGHTNESS);

    attachInterrupt(button_0_pin, button_0_isr, button_0_mode);
    attachInterrupt(button_1_pin, button_1_isr, button_1_mode);
}

int8_t GloveControlPanel::readSwitch()
{
    return digitalRead(switch_pin);
}

int8_t GloveControlPanel::readButton(uint8_t button_pin)
{
    if (button_pin == button_0_pin) {
        return digitalRead(button_0_pin);
    } else if (button_pin == button_1_pin) {
        return digitalRead(button_1_pin);
    }
    return -1;
}

void GloveControlPanel::toggleExtLED(uint8_t led_pin)
{
    int8_t read = readExtLED(led_pin);
    if (read == -1) return;
    digitalWrite(led_pin, !read);
}

void GloveControlPanel::setExtLED(uint8_t led_pin, uint8_t val)
{
    if (led_pin == led_0_pin) {
        digitalWrite(led_0_pin, val);
    } else if (led_pin == led_1_pin) {
        digitalWrite(led_1_pin, val);
    }
}

int8_t GloveControlPanel::readExtLED(uint8_t led_pin)
{
    if (led_pin == led_0_pin) {
        return digitalRead(led_0_pin);
    } else if (led_pin == led_1_pin) {
        return digitalRead(led_1_pin);
    }
    return -1;
}

void GloveControlPanel::toggleIntLED()
{
    if (readIntLED() == 1) {
        int_led_on_color = int_led_color;
        int_led_color = CRGB(0, 0, 0);
        FastLED.show();
        return;
    }
    if (int_led_on_color.red != 0 || int_led_on_color.green != 0 || int_led_on_color.blue != 0) {
        int_led_color = int_led_on_color;
        FastLED.show();
    }
}

void GloveControlPanel::setIntLED(uint8_t val)
{
    if (val == 1) {
        int_led_color = CRGB(255, 255, 255);
        FastLED.show();
    } else if (val == 0) {
        int_led_color = CRGB(0, 0, 0);
        FastLED.show();
    }
}

int8_t GloveControlPanel::readIntLED()
{
    if (int_led_color.red != 0 || int_led_color.green != 0 || int_led_color.blue != 0) {
        return 1;
    }
    return 0;
}

void GloveControlPanel::setIntLEDColor(uint8_t red, uint8_t green, uint8_t blue)
{
    int_led_color = CRGB(red, green, blue);
    FastLED.show();
}

 CRGB GloveControlPanel::getIntLEDColor()
 {
    return int_led_color;
 }