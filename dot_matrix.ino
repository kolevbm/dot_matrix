/*
  ********NOT THE ORIGINAL version********* 
  

  
  Basic code for using Maxim MAX7219/MAX7221 with Arduino.



  Wire the Arduino and the MAX7219/MAX7221 together as follows:



  | Arduino   | MAX7219/MAX7221 |

  | --------- | --------------- |

  | MOSI (11) | DIN (1)         |

  | SCK (13)  | CLK (13)        |

  | I/O (7)*  | LOAD/CS (12)    |



    * - This should match the LOAD_PIN constant defined below.

  

  For the rest of the wiring follow the wiring diagram found in the datasheet.

  

  Datasheet: http://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf


  
  Author:  Nicholas Dobie <nick@nickdobie.com> 

  Date:    30 December 2013

  License: WTFPL (http://www.wtfpl.net/)

 */

//#include <SPI.h>

// What pin on the Arduino connects to the LOAD/CS pin on the MAX7219/MAX7221

#define LOAD_PIN 7
#define MOSI 11
#define MISO 12
#define SCK 13

/**

 * Transfers data to a MAX7219/MAX7221 register.

 * 

 * @param address The register to load data into

 * @param value   Value to store in the register

 */
byte row[9]= {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80, 0x00};

void maxTransfer(uint8_t address, uint8_t value) {
  // Ensure LOAD/CS is LOW
  digitalWrite(LOAD_PIN, LOW);
  // Send the register address

  byteout(address);
 
   // Send the value
  byteout(value);
  // Tell chip to load in data
 digitalWrite(LOAD_PIN, HIGH);
}

void setup() {
  // Set load pin to output
  pinMode(LOAD_PIN, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  Serial.begin(9600);
  // Reverse the SPI transfer to send the MSB first  
  //SPI.setBitOrder(MSBFIRST);  
  // Start SPI
 // SPI.begin();
  // Run test
  // All LED segments should light up
  maxTransfer(0x0F, 0x01);
  Serial.println("All LED segments should light up");
  delay(1000);
  maxTransfer(0x0F, 0x00);
  Serial.println("All LED segments should light down");
  //mode select
  maxTransfer(0x09, 0x00);
  Serial.println("mode select");
  // Use lowest intensity
  maxTransfer(0x0A, 0x00);
  Serial.println("Use lowest intensity");
  // Only scan one digit
  maxTransfer(0x0B, 0x07);
   Serial.println("Only scan one digit");
  // Turn on chip
 maxTransfer(0x0C, 0x01);
 Serial.println("Turn on chip");
}

void loop() {
  // Loop through each code
 
  for (int j = 0; j < 8; j++)
  {
   
  for (int i = 0; i < 9; i++)
  {  
    maxTransfer(j+1, row[i]);
    delay(50);
  }
}
}
void byteout(uint8_t data) {
  
    digitalWrite(SCK,LOW);// Ensure SCK is low
    for (int i = 0; i < 8; i++)         // Loop through each bit
    {
      
        if ((data & 0x80))      // Check if next bit is a 1
        {
            digitalWrite(MOSI, HIGH);                // If a 1, pull SO high //send MOSI high
        }
        else
        {
            digitalWrite(MOSI, LOW);                // If a 0, pull SO low // send MOSI low
        }
        delayMicroseconds(1);
        
        digitalWrite(SCK,HIGH);      // Bring SCK high to latch bit
        delayMicroseconds(1);
        digitalWrite(SCK,LOW);       // Bring SCK low for next bit
        data = data << 1;           // Shift byte left for next bit
        
        
    }
}


 // end of byteout(...)


byte bytein()
{
    
    byte retval;    // Return value

    retval = 0;
    digitalWrite(SCK,LOW);                        // Ensure SCK is low
    for (int i = 0; i < 8; i++)         // Loop through each bit
    {
        retval = retval << 1;       // Shift byte left for next bit
        digitalWrite(SCK,HIGH);                 // Bring SCK high
        if (digitalRead(MISO))                   // Check if next bit is a 1
        {
            retval |= 0x01;         // If a 1, set next bit to 1
        }
        else
        {
            retval &= 0xFE;         // If a 0, set next bit to 0
        }
        digitalWrite(SCK,LOW);                    // Bring SCK low for next bit
    }

    return retval;
} // end of bytein(void)
 


