#include <avr/io.h>
#include <avr/interrupt.h>

//----------------------------------------
// Constants
//----------------------------------------
#define BLINK_TIME       10   // Delay in ms
#define BLINK_DELAY_TIME 20   // Delay in ms
#define DELAY_TIME       300  // Delay in ms

#define LED_BASE         4

//----------------------------------------
// Timer Global Variables
//----------------------------------------
// We'll use a simple flag that is set every 10 ms by TCA0 interrupt.
static volatile uint8_t tca_interrupt_flag = 0;

//----------------------------------------
// Timer Interrupt Service Routine
//----------------------------------------
ISR(TCA0_OVF_vect)
{
    // Clear TCA0 overflow flag
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;

    // Indicate the timer interrupt occurred
    tca_interrupt_flag = 1;
}

//----------------------------------------
// Initialize TCA0 to fire every ~10 ms
//----------------------------------------
static void timer_init_10ms(void)
{
    // Example for a 4 MHz system clock:
    //   Prescaler = 64 --> timer freq = 4,000,000 / 64 = 62,500 Hz
    //   Period = 625 counts -> ~10 ms interrupt
    //   So TCA0.SINGLE.PER = 624 -> ~10 ms
    // Adjust for your actual clock frequency as needed.

    TCA0.SINGLE.CTRLA = 0;              // Disable TCA before config
    TCA0.SINGLE.PER   = 624*4;           // ~10 ms at 4 MHz w/ prescaler=64
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;  // Enable overflow interrupt

    // Prescaler=64, enable TCA0
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;
}

//----------------------------------------
// Sleep-based delay with ~10 ms steps
//----------------------------------------
// Because TCA0 triggers an interrupt every 10 ms, we use that as our "tick".
static void delay_ms(uint16_t ms)
{
    // Convert desired delay to # of 10 ms ticks
    uint16_t ticksNeeded = (ms + 9) / 10;  // round up

    for (uint16_t i = 0; i < ticksNeeded; i++)
    {
        tca_interrupt_flag = 0;

        // 1) Configure the SLPCTRL for IDLE sleep mode.
        //    SMODE[2:0] = 0x0 (IDLE), SEN bit = 1
        //    (SLEEP_SMODE_IDLE_gc is typically defined as 0<<1, 
        //    but let's do it explicitly.)

        // Clear any previous sleep mode bits, then set IDLE + enable sleep
        SLPCTRL.CTRLA = (SLPCTRL.CTRLA & ~SLPCTRL_SMODE_gm)  // clear SMODE bits
                      | (0 << SLPCTRL_SMODE_gp)             // IDLE = 0x0 
                      | SLPCTRL_SEN_bm;                      // Sleep Enable bit

        // 2) Enable global interrupts to allow wake from TCA0 ISR
        sei();

        // 3) Execute the SLEEP instruction in a loop until the interrupt sets the flag
        while (!tca_interrupt_flag)
        {
            __asm__ __volatile__("sleep");
        }

        // 4) Optionally disable sleep if desired
        SLPCTRL.CTRLA &= ~SLPCTRL_SEN_bm;
    }
}

//----------------------------------------
// Main
//----------------------------------------
int main(void)
{
    // Configure PA4..PA7 as output
    PORTA.DIRSET = 0b11110000;

    // Initialize the 10 ms timer
    timer_init_10ms();

    // Enable global interrupts
    sei();

    while (1)
    {
        for (int i = 5; i >= 0; i--)
        {
            if (i == 1) 
            {
                PORTA.OUTSET = 0b11110000;
                delay_ms(BLINK_TIME);
                PORTA.OUTCLR = 0b11110000;
                delay_ms(BLINK_DELAY_TIME);
            }
            else if (i == 0)
            {
                PORTA.OUTSET = (1 << 4);
                delay_ms(BLINK_TIME);
                PORTA.OUTCLR = (1 << 4);
                delay_ms(BLINK_DELAY_TIME);
            }
            else
            {
                PORTA.OUTSET = (1 << (LED_BASE + i - 1));
                delay_ms(BLINK_TIME);
                PORTA.OUTCLR = (1 << (LED_BASE + i - 1));
                delay_ms(BLINK_DELAY_TIME);
            }
        }
        delay_ms(DELAY_TIME);
    }
    return 0; // not reached
}
