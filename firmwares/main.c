

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#include "hardware/watchdog.h"


const uint LED_PIN = 25;

static char serial_buffer[256]={0};
static char passwordbuf[64]={0};
static volatile uint32_t auth_required = true; 

const char help_text[] = "a<pin>: Authenticate with a PIN code \r\ns<n>: save a flag to slot n* \r\np<n>: print a flag from slot n* \r\ne: Change PIN* \r\nb: Flag with lockdown mode\r\nr: Reset \r\n *(Auth required) \r\n";
const char defaultpassword[] = "swodrfish"; 

// blink led count <1 .. 2^32-1> times and leave it off 
void blinky(uint32_t count) {   
    switch ( (uint64_t)-1 & count) {
        case 0: break; 
        case (uint64_t) -1: do { sleep_ms(500);
        default: gpio_put(LED_PIN, 1); sleep_ms(500); gpio_put(LED_PIN, 0);
        } while(--count>0);
    }
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
bool __attribute__ ((noinline)) authenticate() {
    printf("Input password:");
    read_line();
    printf("\r\n");

    if(!memcmp(serial_buffer, passwordbuf, strlen(passwordbuf))){
        auth_required = false; 
        printf("ACCESS GRANTED\r\n");
        return true; 
    }
    else {
        printf("ACCESS DENIED\r\n");
        return false; 
    }
}
bool __attribute__ ((noinline)) print_flag() {
    if (auth_required != true) {
         printf("flag{R41ndr0ps_0n_r0535_and_g71tch3s_on_ch1ppi35}\r\n");
         blinky(10);
         return true; 
    }
    else {
        printf("ACCESS DENIED\r\n");
        return false; 
    }
}

bool __attribute__ ((noinline)) crypto_flag() {
    printf("Your random challenge is \"1334\" give a MAC code:\r\n"); 
    read_line(); 
    // More rounds means more security 
    uint32_t cryptorounds=0x42; //cafe; 
    uint32_t i= 0; 
    // Longer the key the securer it gets
    uint32_t key[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599 };
    
    uint32_t result=1334; 
    uint32_t sig=0;  
    for (i = 0; serial_buffer[i] != '\0'; ++i)
        sig = sig * 10 + serial_buffer[i] - '0';
    
    while (cryptorounds-->0) {
        result=result*key[cryptorounds%sizeof(key)]; 
    }
    if (sig == result) {
        printf("flag{m1l1t4ry_gr4d3_crypt0}\r\n");
        return true; 
    }
    else {
        printf("ACCESS DENIED with Signature %d!\r\n", sig);
    }
    return false; 
}
void print_menu() {
    printf("%s", help_text);
}

bool save_flag() {
    printf("not implemented \r\n");
    return true; 
}
void __attribute__ ((noinline)) update_password() {
    strcpy(passwordbuf, serial_buffer); 
}

bool __attribute__ ((noinline)) change_pin() {
    if (auth_required != true) {
        printf("Input password:");
        read_line();
        update_password(); 
        printf("...SUCCESS\r\n");
        auth_required = true; 
    }
    else {
        printf("Please do the needful.\r\n");
        return false; 
    }
    return true; 
}
bool reset() {
    printf("RESETTING \r\n");
    watchdog_enable(1, 1);
    while(1);

}
bool __attribute__ ((noinline)) raptor_flag() {
    uint32_t count = 0; 
    if (auth_required != false) {
        goto flag_label; 
    }
     // replace infinite loop with something better  TODO: get an egg from Dennis 
    while(1) {
        count++; 
    }
flag_label: 

    printf("flag{l34rn_t0_0p3n_d00rs after %d tries}\r\n", count);
    return true; 
}

bool __attribute__ ((noinline)) baby_flag() {
   
        // Somebody try to hacxs us, let's go unresponsive  
        while(auth_required == true) {
 
         }
 

   printf("flag{gl1tch_m3_b4by_0x1_m0ar_t1m3}\r\n");
   blinky(7); 
   return true;
}

bool __attribute__ ((noinline)) narnia_flag() {
    printf("flag{w1tch3s_g3t_gl1tches_l10ns_b34m_ion5}\r\n");
}
void cheat_io() {
   
    // Indicate that SW has been loaded  
    gpio_put(0, 1); sleep_ms(1); gpio_put(0, 0);
}


bool handle_command(char *command) {
    cheat_io();
    switch (*command) {


        case 's': return save_flag();
        case 'p': return print_flag();
        case '\n': return narnia_flag(); 
        case 'b': return baby_flag(); 
        case 'c': return crypto_flag(); 
        case 'g': return raptor_flag(); 
        case 'e': return change_pin();
        case 'a': return authenticate();
        case 'r': return reset();
        default: return false; 
    }

}


int main() {
    
    gpio_init(0);
    gpio_set_dir(0,GPIO_OUT);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    blinky(1); 
    cheat_io(); 
    
    strcpy(serial_buffer, defaultpassword); 
    update_password(); 
    
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

