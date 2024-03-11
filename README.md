# FlashBang ARGB

## Aim
The aim of this project is to make a **simple extendible and highly performant** ARGB-Driver that is suitible for single-core microcontroller operation with other services running (for example [ESPAsyncWebserver](https://github.com/me-no-dev/ESPAsyncWebServer))

## Motivation

We are developing this library for all of you frustrated with the other libraries. All other LED-Driver Libraries for Arduinos seem to have  the common bugs, that they are not resistent to interrupts by other "processes" or caching problems.


## Usage

params: 
 - pin D0
 - length of 12 
 - Type of addressable leds
 - wether we want to always overwrite the stripe (true) or if we only overwrite the leds up to last updated led (false)
 - specifies the (fast_update behavior)[#FastUpdateBehavior]
```arduino
LEDStripe<12> led_stripe_one =  LEDStripe(D0, WRGB, true); 


void loop(){
    led_stripe_one.set_all(red_v, green_v, blue_v);
    led_stripe_one[3].set(red_v, green_v, blue_v);
    led_stripe_one[11].set(red_v, green_v, blue_v);

    led_stripe_one.put();

    led_sripte_one.clear();
}
```

---

**NOTE:** calling ```put()``` does not clear the buffer. So it can be called repeateadly and will always yield the same result.

---


## FastUpdateBehavior

FastUpdateBehaviour is a struct containing a ```delta_t``` field and a UpdateBehavior enum with the following variants:
- skip: If delta t is to small, just continue without doing anything
- block: If delta t is to small, block until enough time is elapsed
- anyways: Write data to the stripe in any case 
