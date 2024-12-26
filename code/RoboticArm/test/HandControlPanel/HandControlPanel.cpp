#include <HandControlPanel.h>

HandControlPanel::HandControlPanel(void (*button_0_isr)(), void (*button_1_isr)())
{
    this->switch_0_pin = SWITCH_0;
    this->switch_1_pin = SWITCH_1;
    this->button_0_pin = BUTTON_0;
    this->button_1_pin = BUTTON_1;
    this->led_0_pin = LED_0;
    this->led_1_pin = LED_1;
    this->button_0_isr = button_0_isr;
    this->button_1_isr = button_1_isr;
    this->button_0_mode = FALLING;
    this->button_1_mode = FALLING;
}

HandControlPanel::HandControlPanel(uint8_t switch_0_pin, uint8_t switch_1_pin, uint8_t button_0_pin, uint8_t button_1_pin, uint8_t led_0_pin, uint8_t led_1_pin,
void (*button_0_isr)(), void (*button_1_isr)(), uint8_t button_0_mode, uint8_t button_1_mode)
{
    this->switch_0_pin = switch_0_pin;
    this->switch_1_pin = switch_1_pin;
    this->button_0_pin = button_0_pin;
    this->button_1_pin = button_1_pin;
    this->led_0_pin = led_0_pin;
    this->led_1_pin = led_1_pin;
    this->button_0_isr = button_0_isr;
    this->button_1_isr = button_1_isr;
    this->button_0_mode = button_0_mode;
    this->button_1_mode = button_1_mode;
}

HandControlPanel::~HandControlPanel() {}

void HandControlPanel::initialize()
{
    pinMode(switch_0_pin, INPUT);
    pinMode(switch_1_pin, INPUT);
    pinMode(button_0_pin, INPUT);
    pinMode(button_1_pin, INPUT);
    pinMode(led_0_pin, OUTPUT);
    pinMode(led_1_pin, OUTPUT);

    FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(&int_led_color, 1);
    FastLED.setBrightness(RGB_BRIGHTNESS);

    attachInterrupt(button_0_pin, button_0_isr, button_0_mode);
    attachInterrupt(button_1_pin, button_1_isr, button_1_mode);
}

int8_t HandControlPanel::readSwitch(uint8_t switch_num)
{
    if (switch_num == 0) {
        return digitalRead(switch_0_pin);
    } else if (switch_num == 1) {
        return digitalRead(switch_1_pin);
    }
    return -1;
}

int8_t HandControlPanel::readButton(uint8_t button_num)
{
    if (button_num == 0) {
        return digitalRead(button_0_pin);
    } else if (button_num == 1) {
        return digitalRead(button_1_pin);
    }
    return -1;
}

void HandControlPanel::toggleExtLED(uint8_t led_num)
{
    int8_t read = readExtLED(led_num);
    if (read == -1) return;
    setExtLED(led_num, !read);
}

void HandControlPanel::setExtLED(uint8_t led_num, uint8_t val)
{
    if (led_num == 0) {
        digitalWrite(led_0_pin, val);
    } else if (led_num == 1) {
        digitalWrite(led_1_pin, val);
    }
}

int8_t HandControlPanel::readExtLED(uint8_t led_num)
{
    if (led_num == 0) {
        return digitalRead(led_0_pin);
    } else if (led_num == 1) {
        return digitalRead(led_1_pin);
    }
    return -1;
}

void HandControlPanel::toggleIntLED()
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

void HandControlPanel::setIntLED(uint8_t val)
{
    if (val == 1) {
        int_led_color = CRGB(255, 255, 255);
        FastLED.show();
    } else if (val == 0) {
        int_led_color = CRGB(0, 0, 0);
        FastLED.show();
    }
}

int8_t HandControlPanel::readIntLED()
{
    if (int_led_color.red != 0 || int_led_color.green != 0 || int_led_color.blue != 0) {
        return 1;
    }
    return 0;
}

void HandControlPanel::setIntLEDColor(uint8_t red, uint8_t green, uint8_t blue)
{
    int_led_color = CRGB(red, green, blue);
    FastLED.show();
}

 CRGB HandControlPanel::getIntLEDColor()
 {
    return int_led_color;
 }