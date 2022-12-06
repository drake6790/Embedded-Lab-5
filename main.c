#include <msp430.h>

#define CALADC_15V_30C  *((unsigned int *)0x1A1A)

#define CALADC_15V_85C  *((unsigned int *)0x1A1C)

volatile float temp;
volatile float IntDegF;
volatile float IntDegC;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    TB0CCTL0 |= CCIE;
    TB0CCR0 = 65535;
    TB0CTL = TBSSEL__ACLK | MC__UP;


    ADCCTL0 |= ADCSHT_8 | ADCON;
    ADCCTL1 |= ADCSHP;
    ADCCTL2 &= ~ADCRES;
    ADCCTL2 |= ADCRES_2;
    ADCMCTL0 |= ADCSREF_1 | ADCINCH_12;
    ADCIE |=ADCIE0;


    PMMCTL0_H = PMMPW_H;
    PMMCTL2 |= INTREFEN | TSENSOREN | REFVSEL_0;
    __delay_cycles(400);

    __bis_SR_register(LPM0_bits | GIE);
    __no_operation();
}



// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            temp = ADCMEM0;
            // Celsius

            IntDegC = (temp-CALADC_15V_30C)*(85-30)/(CALADC_15V_85C-CALADC_15V_30C)+30;

            // Fahrenheit

            IntDegF = 9*IntDegC/5+32;
            break;
        default:
            break;
    }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer_B (void)
#else
#error Compiler not supported!
#endif
{
    ADCCTL0 |= ADCENC | ADCSC;
}
