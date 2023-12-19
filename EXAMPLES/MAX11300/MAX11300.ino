// ====================================================
//  This is an example sketch for the MAX11300 library
//  
//  20231212 HHMI Janelia Steve Sawtelle
//
// ====================================================

// toggle two digital outputs, ramp an analog signal up and down, and read back into ADC
// connect pin 2 (DAC) to pin 3 (ADC) to read back DAC output

#include <MAX11300.h>
#include <SPI.h>

MAX11300 maxIO(30, 6) ;   // pin 6 will be chip select and 30 will initiate ADC conversions
// change this to suit your setup


// ====================
//  ===  S E T U P  === 
//  ===================
void setup()
{  
    Serial.begin(115200);       // USB serial startup
    while( !Serial);   // wait for connection (comment out if not monitoring serial)

    Serial.println("MAX11300 Example");

    SPI.begin();

    maxIO.begin(); // start up maxim chip
    
    maxIO.DACreference(DACInternal);   // use internal reference for DAC       
                   
    maxIO.digitalRange(0, 4.0);   // set up digital output voltage on channel 0 to 4.0 volts
    if(maxIO.pinMode( 0, digitalOut) == false) Serial.println("bad 0");  // set ch 0 as digital out
    maxIO.digitalWrite(0, HIGH);    //  set it high 
    
    maxIO.digitalRange(1, 8.0);   // set up digital output voltage on channel 1 to 8.0 volts
    if( maxIO.pinMode( 1, digitalOut) == false) Serial.println("bad 1");  
    maxIO.digitalWrite(1, LOW);
    
    maxIO.pinMode( 2, analogOut);     // make pin 2 an analog out
    maxIO.DACrange( 2, DACZeroTo10);  // with 0 to 10 volt range
    
    if( maxIO.pinMode(3, analogIn) == false) Serial.println("bad 3");  
    maxIO.setADCmode( MAX_ADC_CONTINUOUS);  // pin 3 will be analog in, run continuous 
    maxIO.ADCreference(3, ADCInternal);
    if( maxIO.ADCrange( 3, MAX_ADC_RANGE_0_10) == false) Serial.println("ADC range error"); 
    maxIO.setConversionRate(MAX_RATE_400);
   
        
    Serial.print( maxIO.readInternalTemp());  // show internal temperature
    Serial.println(" deg C");
    
    Serial.print("maxid: ");                 // and the chip ID
    Serial.println( maxIO.getID(), HEX );  
}

int16_t dacval = 0;

// ===========================
//  ===  M A I N  L O O P  === 
//  ==========================

// toggle two digital outputs and 
void loop()
{ 
   Serial.print("0 hi, 1 lo, DAC: 0x");
   Serial.print(dacval, HEX);      // show current DAC value
   Serial.print(" = ");
   Serial.print(10.0 * (float)dacval/4095.0);
   Serial.println( "V");
   maxIO.digitalWrite(0, true);   // toggle digital outputs
   maxIO.digitalWrite(1, false);   
   
   maxIO.analogWrite(2, dacval);  // and output it
   dacval += 128;
   if( dacval > 4095 ) dacval = 0;   // start again
   
   delay(2000);  // slow enough to read with a DVM
 
   Serial.print("0 lo, 1 hi, ADC: 0x");  
   Serial.println( maxIO.analogRead(3), HEX );
   maxIO.digitalWrite(0, false);   // toggle again
   maxIO.digitalWrite(1, true);
   
   delay(2000);
}
