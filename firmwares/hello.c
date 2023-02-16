


#include "pico/stdlib.h"



const uint32_t LED_PIN = PICO_DEFAULT_LED_PIN;


// blink led count <1 .. 2^32-1> times and leave it off 
void blinky(uint32_t count) {   
    switch (((uint64_t)-1) & count) {
        case 0: break; 
        case (uint64_t)-1: do { sleep_ms(500);
        default: gpio_put(LED_PIN, 1); sleep_ms(500); gpio_put(LED_PIN, 0);
        } while(count-->0);
    }
}



int main() {
    volatile uint32_t locked = 42; 
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    // reset indication 
    blinky(3);
    // Evil locked loop
    while (locked == 42) {
        locked = 42;
    }
    // Delicious toasting functionality
    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
    }
}

