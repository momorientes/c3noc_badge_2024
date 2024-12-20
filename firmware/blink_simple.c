#include <avr/io.h>
#include <util/delay.h>

#define BLINK_TIME 10 // Delay in milliseconds
#define BLINK_DELAY_TIME 20 // Delay in milliseconds
#define DELAY_TIME 300 // Delay in milliseconds

#define LED_BASE 4

int main(void) {
    // configure PA4-7 as output
    PORTA.DIRSET = 0b11110000;
    while(1){ 
        for(int i=5; i >= 0; i--) {
            if(i == 1) {
                PORTA.OUTSET = 0b11110000;
                _delay_ms(BLINK_TIME);
                PORTA.OUTCLR = 0b11110000;
                _delay_ms(BLINK_DELAY_TIME);
            } else if(i == 0) {
                PORTA.OUTSET = (1 << 4);
                _delay_ms(BLINK_TIME);
                PORTA.OUTCLR = (1 << 4);
                _delay_ms(BLINK_DELAY_TIME);
            } else {
                PORTA.OUTSET = (1 << (LED_BASE+i-1));
                _delay_ms(BLINK_TIME);
                PORTA.OUTCLR = (1 << (LED_BASE+i-1));
                _delay_ms(BLINK_DELAY_TIME);
            }
            
        }
        _delay_ms(DELAY_TIME);
    }

    return 0; // This line will never be reached
}
