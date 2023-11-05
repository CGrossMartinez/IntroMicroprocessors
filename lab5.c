/**
 * CDA 3331C: Introduction to Microprocessors
 * Dr. Bassem Alhalabi, FAU EE512, Boca Raton, FL
 * Contributors: Pablo Pastran (2015), David Wilson (2020)
 * Skeleton Program for Lab 5 in C
 *
 * Instructions:
 *  Run this program as is to verify correct hardware connections.
 *  Explore the program and see the effect of switches on pins
 *  P2.3-5. Make the appropriate changes to the program as per the
 *  lab manual.
 *  SW-321 = 000: Counter Resets to 00
 *  SW-321 = 001: Right Digit Cycles 0-9
 *  SW-321 = 010: Left Digit Cycles 0-9
 *  SW-321 = 011: Right and Left Digits Both Hold Values (Preset)
 *  SW-321 = 100: Counter Cycles Up from the Preset Value to 90
 *      Repeatedly
 *  SW-321 = 101: Recall the Preset Value
 *  SW-321 = 110: Recall the Preset Value
 *  SW-321 = 111: Counter Cycles Down from the Preset Value to 10
 *      Repeatedly
 */

#include <msp430.h>

// Display configuration (segment control values for each digit)
// Explanation:
//  0x3F = 0b00111111 -> Segments a-f are on, segment g is off
int LEDS[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x01,
              0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

int l_digit = 0, r_digit = 0, l_preset = 0, r_preset = 0;
int switches = 0, flag = 0;

int main(void)
{
    // Note: We do not stop the watchdog timer.

    // BCSCTL2: Basic Clock System Control 2
    //  Configure Main (MCLK) and Sub-Main Clocks (SMCLK)
    BCSCTL2 |= DIVS_2;      // SMCLK Divider: DIVS_0-3

    // WDTCTL: Watch Dog Timer Control
    //  WDT_MDLY_0_5: Use WDT as interval clock (0.5 ms)
    //      Source: SMCLK (DIVS_2 -> 1 ms)
    //  WDT_MDLY_32, WDT_MDLY_8, WDT_MDLY_0_5, WDT_MDLY_0_064
    WDTCTL = WDT_MDLY_0_5;
    IE1 |= WDTIE;   // Enable WDT interrupt

    P1DIR = 0xFF;   // Port 1 all output
    P1OUT = 0x00;   // Port 1 default output low
    P2DIR = 0x03;   // Port 2 all inputs except P2.0 and P2.1

    __enable_interrupt();

    while (1)
    {
        switches = ~P2IN;   // Active low

        // Check switches for 000 -> Counter resets to 00
        if (((switches & BIT5) != BIT5) && ((switches & BIT4) != BIT4) &&
            ((switches & BIT3) != BIT3))
        {
            l_digit = 0;
            r_digit = 0;
        }
        // Check switches for 001 -> Right digit cycles 0-9
        if (((switches & BIT5) != BIT5) && ((switches & BIT4) != BIT4) &&
            ((switches & BIT3) == BIT3))
        {
            r_digit++;
            if (r_digit >= 10) {r_digit = 0;}
        }
        // Check switches for 010 -> Left digit cycles 0-9
        if (((switches & BIT5) != BIT5) && ((switches & BIT4) == BIT4) &&
            ((switches & BIT3) != BIT3))
        {
            l_digit++;
            if (l_digit >= 10) {l_digit = 0;}
        }
        // Check switches for 011 -> Hold/set preset
        if (((switches & BIT5) != BIT5) && ((switches & BIT4) == BIT4) &&
            ((switches & BIT3) == BIT3))
        {
            l_preset = l_digit;
            r_preset = r_digit;
        }
        // Check switches for 100 -> Count up from preset repeating
        if (((switches & BIT5) == BIT5) && ((switches & BIT4) != BIT4) &&
            ((switches & BIT3) != BIT3))
        {
            // TODO: Modify this section
            //  Delete this rotating pattern
            if (l_digit <= 9) {l_digit = 10;}
            if (r_digit <= 9) {r_digit = 10;}

            l_digit--;
            if (l_digit <= 9) {l_digit = 16;}

            r_digit++;
            if (r_digit >= 17) {r_digit = 10;}
        }
        // Check switches for 101 -> Recall preset
        if (((switches & BIT5) == BIT5) && ((switches & BIT4) != BIT4) &&
            ((switches & BIT3) == BIT3))
        {
            // TODO: Modify this section
        }
        // Check switches for 110 -> Recall preset
        if (((switches & BIT5) == BIT5) && ((switches & BIT4) == BIT4) &&
            ((switches & BIT3) != BIT3))
        {
            // TODO: Modify this section
        }
        // Check switches for 111 -> Count down from preset repeating
        if (((switches & BIT5) == BIT5) && ((switches & BIT4) == BIT4) &&
            ((switches & BIT3) == BIT3))
        {
            // TODO: Modify this section
        }

        // Counting speed
        __delay_cycles(500000);
    }
}

// WDT Interrupt Service Routine
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    // This executes with each WDT interrupt. The frequency controls
    // display flickering. Each digit is displayed one at a time.
    //
    // Note: Both digits must be turned off to avoid aliasing.

    P1OUT = 0x00;
    P2OUT = 0x00;

    __delay_cycles (100);
    if (flag) {P2OUT = BIT0; P1OUT = LEDS[r_digit]; flag = !flag;}
    else {P2OUT = BIT1; P1OUT = LEDS[l_digit]; flag = !flag;}
    __delay_cycles (100);
}
