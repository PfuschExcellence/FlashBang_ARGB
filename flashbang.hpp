#include <stdbool.h>

// class RGB;
class LED;

union UnionARGBType;

class RGB {
private:
  int red;
  int green;
  int blue;

public:
  RGB(int red, int green, int blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
  }
  RGB() : RGB(255, 255, 255) {}

  bool operator==(RGB const &rhs) const {
    return (rhs.red == this->red) && (rhs.blue == this->blue) &&
           (rhs.green == this->green);
  }
};
class SimpleARGB {
public:
  void put(RGB led, bool overwrite, bool force) {}
};

class ComplicatedARGB {
public:
  void put(LED leds[], int length, bool overwrite, bool force) {}
};

const RGB BLACK = RGB(0, 0, 0);

class LED {
private:
  RGB rgb;
  bool on;

public:
  LED() {
    this->rgb = RGB();
    this->on = false;
  }
  bool is_on() { return this->on; }

  bool is_off() { return !is_on(); }

  bool displays_black() { return is_off() | rgb == RGB{0, 0, 0}; }

  void set_state(bool on) { this->on = on; }

  void turn_off() { this->on = false; }

  void turn_on() { this->on = true; }

  RGB get_color() { return this->rgb; }

  RGB get_color_or(RGB rgb) {
    if (this->on) {
      return this->rgb;
    } else {
      return rgb;
    }
  }

  void set_color(RGB rgb, bool set_on = true) {
    this->rgb = rgb;
    if (set_on) {
      this->on = true;
    }
  }

  void set_color(int red, int green, int blue, bool set_on = true) {
    set_color(RGB(red, green, blue), set_on);
  }
};

enum EnumARGBType { SIMPLE_ARGB, COMPLICATED_ARGB };

union UnionARGBType {
  SimpleARGB s;
  ComplicatedARGB c;
};

class ARGBType {
private:
  EnumARGBType e;
  UnionARGBType u;

public:
  void put(LED leds[], int length, bool overwrite) {
    if (e == EnumARGBType::SIMPLE_ARGB) {
      u.s.put(leds[0].get_color_or(BLACK), overwrite,
              false); // false if we dont acutally wont to write (forcing the
                      // function in the chache)

      for (int i = 0; i < length; i++) {
        u.s.put(leds[i].get_color_or(BLACK), overwrite, true);
      }

    } else {
      u.c.put(leds, length, overwrite, false);
      // false if we dont acutally want to write (forcing the
      // function in the chache)

      // u.c.put(leds, length, overwrite);
    }
  }
};

template <int LENGTH> class LEDStripe {
private:
  int pin;
  ARGBType type;
  bool overwrite;

  // malloc gives back a pointer so this would have to be a pointer too
  // thats why we use generics
  LED leds[LENGTH];

  void _clear_off() {
    for (int i = 0; i < LENGTH; i++) {
      this->leds[i].set_color(BLACK);
      this->leds[i].turn_off();
    }
  }

  void _clear_keep() {
    for (int i = 0; i < this->LENGTH; i++) {
      this->leds[i].set_color(BLACK);
    }
  }

  void _set_all_on(RGB rgb) {
    for (int i = 0; i < LENGTH; i++) {
      this->leds[i].set_color(rgb);
      this->leds[i].turn_on();
    }
  }

  void set_all_keep(RGB rgb) {
    for (int i = 0; i < LENGTH; i++) {
      this->leds[i].set_color(rgb);
    }
  }

public:
  LEDStripe(int pin, ARGBType type, bool overwrite = true) {
    this->pin = pin;
    this->type = type;
    this->overwrite = overwrite;

    // malloc ... pointers....
    for (int i = 0; i < LENGTH; i++) {
      leds[i] = LED();
    }
  };

  int length() { return LENGTH; }

  // writes everything black
  // if set_off is enabled: additionally turn off each led
  void clear(bool set_off = false) {
    // programmed like this to avoid branching in every iteration of the for
    // loop
    if (set_off) {
      this->_clear_off();
    } else {
      this->_clear_keep();
    }
  }
  void put() { this->type.put(this->leds, LENGTH, this->overwrite); }

  void set_all(RGB rgb, bool set_on = true) {
    if (set_on) {
      this->_set_all_on(rgb);
    } else {
      this->_set_all_keep(rgb);
    }
  }
};
