// c code for adc interfacing
// developed in code composer studio
#include "DSP28x_Project.h"  

__interrupt void adc_isr(void);
void Adc_Config(void);


// Global variables used in this example:
Uint16 LoopCount;
Uint16 ConversionCount;
Uint16 Voltage1[10];


main()
{

// Step 1.Initialize System Control:
   InitSysCtrl();

// Step 3 Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize the PIE control registers to their default state.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
   InitPieVectTable();

   EALLOW;  
   PieVectTable.ADCINT1 = &adc_isr;
   EDIS;  
// Step 4. Initialize all the Device Peripherals:
   InitAdc();  vvfadvadfvadvadvadv
   AdcOffsetSelfCal();adf zcx cv 
vavafv
// Step 5.  enable interrupts:
 ac c ad aIE
   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;   // Enable INT 1.1 in the PIE
   IER |= M_INT1;                       // Enable CPU Interrupt 1
   EINT;                                // Enable Global interrupt INTM
   ERTM;                                // Enable Global realtime interrupt DBGM

   LoopCount = 0;
   ConversionCount = 0;

// Configure ADC
    EALLOW;
    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;  // Enable non-overlap mode
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;    // ADCINT1 trips after AdcResults latch
    AdcRegs.INTSEL1N2.bit.INT1E     = 1;    // Enabled ADCINT1
    AdcRegs.INTSEL1N2.bit.INT1CONT  = 0;    // Disable ADCINT1 Continuous mode
    AdcRegs.INTSEL1N2.bit.INT1SEL   = 0;    // setup EOC1 to trigger ADCINT1 to fire
    AdcRegs.ADCSOC0CTL.bit.CHSEL    = 4;    // set SOC0 channel select to ADCINA4
    AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = 5;    // set SOC0 start trigger on EPWM1A, due to round-robin SOC0 converts first then SOC1
    AdcRegs.ADCSOC0CTL.bit.ACQPS    = 6;    // set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
    EDIS;

// Assumes ePWM1 clock is already enabled in InitSysCtrl();
   EPwm1Regs.ETSEL.bit.SOCAEN   = 1;        // Enable SOC on A group
   EPwm1Regs.ETSEL.bit.SOCASEL  = 4;        // Select SOC from CMPA on upcount
   EPwm1Regs.ETPS.bit.SOCAPRD   = 1;        // Generate pulse on 1st event
   EPwm1Regs.CMPA.half.CMPA     = 0x002D;   // Set compare A value
   EPwm1Regs.TBPRD              = 0x002D;   // Set period for ePWM1
   EPwm1Regs.TBCTL.bit.CTRMODE  = 0;        // count up and start

// Wait for ADC interrupt
   for(;;)
   {
      LoopCount++;
   }

}


__interrupt void  adc_isr(void)
{

  Voltage1[ConversionCount] = AdcResult.ADCRESULT0;


  // If 10 conversions have been logged, start over
  if(ConversionCount == 9)
  {
     ConversionCount = 0;
  }
  else ConversionCount++;

  AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;     //Clear ADCINT1 flag reinitialize for next SOC
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE

  return;
}


