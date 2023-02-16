
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "hardware/watchdog.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

static char serial_buffer[256];
static uint32_t auth_required = true; 

const char help_text[] = "h: Help \r\ns: Do SRAM operations and detect glitches\r\nf: Do flash operations and detect glitches\r\nl: Detect logic glitches \r\nr: Reset \r\n";

const char password[] = "swodrfish"; 

// blink led count <1 .. 2^32-1> times and leave it off 
void blinky(uint32_t count) {   
    switch ( (uint64_t)-1 & count) {
        case 0: break; 
        case (uint64_t) -1: do { sleep_ms(500);
        default: gpio_put(LED_PIN, 1); sleep_ms(500); gpio_put(LED_PIN, 0);
        } while(--count>0);
    }
}
void print_menu() {
    printf("%s", help_text);
}

void read_line() {
    memset(serial_buffer, 0, sizeof(serial_buffer));
    while(1) {
        int c = getchar();
        if(c == EOF) {
            return;
        }

        putchar(c);

        if(c == '\r') {
            return;
        }
        if(c == '\n') {
            continue;
        }
        if(strlen(serial_buffer) == 255) {
            return;
        }
        serial_buffer[strlen(serial_buffer)] = (char)c;
    }
}
bool flash_glitch() {
    printf("not implemented \r\n");
    return true; 
}
bool sram_glitch() {
    // first six digits of pi 
    volatile uint32_t fibonacci[] = {1,2,4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824};

    uint32_t sum,counter,i,j, big_loop=0; 

    while(1) {
    for (i=0; i<0x7BFA0; i++) {
        sum = 0; 
        for (j=0; j<(sizeof(fibonacci)/sizeof(uint32_t)-1); j++) {
            sum += fibonacci[j]; 
        }
        if (sum != 2147483647) {
            break; 
        }
    }

    big_loop++; 
    if (i != 0x7BFA0 ) {
        printf("Glitch sum is %d count is %d", sum, counter); 
        ++counter; 
    }
     if (big_loop > 10) {
        printf("Still alive! \r\n");
        big_loop=0; 
     }
    }
    return true; 
}

bool logic_glitch() {
    uint32_t i = 0; 
    uint32_t counter =0; 
    uint32_t big_loop = 0; 
    volatile uint32_t fruit =0x6170706c;
    while(1) {
    for (i=0; i<0x7BFA0; i++) {
        // pretty sure this won't happen 
        if (fruit == 0x6f726e67) {
            break; 
        }

    }
    // Some sacrificial instructions
    ++fruit;
    ++fruit; 
    ++fruit;
    fruit = 0x6170706c; 
    big_loop++; 
    if (i != 0x7BFA0 ) {
        printf("Glitch i is %d count is %d", i, counter); 
        ++counter; 
    }
     if (big_loop > 10) {
        printf("Still alive! \r\n");
        big_loop=0; 
     }
    }
    return true; 

}


bool reset() {
    printf("RESETTING \r\n");
    watchdog_enable(1, 1);
    while(1);

}


bool handle_command(char *command) {

    if(strcmp(command, "h") == 0 || strcmp(command, "help") == 0) {
        return false;
    }
    if (strcmp(command, "s" )==0) {
        return sram_glitch();
    }
    if(strcmp(command, "f")==0) {
        return flash_glitch(); 
    }
    if(strcmp(command, "l")==0) {
        return logic_glitch(); 
    }

    if(strcmp(command, "r")==0) {
        return reset(); 
    }

}


int main() {
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    // reset indication 

    blinky(3); 
    stdio_init_all();
while(!stdio_usb_connected()) {
    sleep_ms(100);
} 
    print_menu();
 while(1) {

    read_line();
    if(!handle_command(serial_buffer)) {
        print_menu();
    }
 }
}
