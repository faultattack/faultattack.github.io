////// EMFI FLASHTEST + SRAMTEST + EEPROMTEST /////
#include <EEPROM.h>

int flash_alive = 1; 
int sram_alive = 1;
int eeprom_alive = 1;
int memory_fail = 0;  // Flash, Sram or Eeprom fail

const int FLASH_ROUNDS =  1;
const int SRAM_ROUNDS =  1;
const int EEPROM_TEST_SIZE = 1024; // ATmega328P has 1kB EEPROM, 1024 bytes

const PROGMEM uint16_t intSet10_flash[] = {1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000};
const int intSet10_sram[] = {1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000, 1000, 2000, 3000, 4000, 5000};


// The setup function runs once when you press reset or power the board.
void setup() 
{  
  pinMode(A1, OUTPUT); // A1 need to be connected to thr RB PicoEMP Genio0 if Fast trigger is used.

  pinMode(7, OUTPUT); // Test Alive-led, it is near the battery holder and should blinking.

  // LEDs from left to right. 
	pinMode(11, OUTPUT); // FLASH-fail led
	pinMode(5, OUTPUT);  // SRAM-fail led
	pinMode(6, OUTPUT);  // EEPROM-fail led
  pinMode(10, OUTPUT); // HW-boot & Fail-led
  pinMode(3, OUTPUT);  // HW-boot & Fail-led
  pinMode(9, OUTPUT);  // HW-boot & Fail-led

  eeprom_write();
}

void flash_test(void)
{
  volatile long flash_intsum = 0;
  
  for (int i=0; i<FLASH_ROUNDS; i++)
  {
    if (flash_alive)
    {     
      flash_intsum = 0;
      for (int j=0;j<100;j++)
      {
        for (int i=0; i<50; i++)
          flash_intsum = flash_intsum + pgm_read_word_near(intSet10_flash+i); 
      }
      flash_alive = (flash_intsum==15000000); // flash_alive = 0 if failed
    }
  }
}

void sram_test(void)
{
  volatile long sram_intsum = 0;

  for (int i=0; i<SRAM_ROUNDS; i++)
  {
    if (sram_alive)
    {     
      sram_intsum = 0;
      for (int j=0;j<100;j++)
      {
        for (int i=0; i<50; i++)
          sram_intsum = sram_intsum + intSet10_sram[i]; 
      } 
      sram_alive = (sram_intsum==15000000); // sram_alive = 0 if failed
    }
  }
}

void eeprom_write(void)
{
  for (int i=0; i<EEPROM_TEST_SIZE; i++)  
  {
    EEPROM.update(i, 1); // Writes only if necessary to save lifetime (.update)
  }
}

void eeprom_test(void)
{
  long eeprom_sum = 0;
  
  for (int i=0; i<EEPROM_TEST_SIZE; i++)
  {
    eeprom_sum += EEPROM.read(i); 
  }
  if (eeprom_sum != EEPROM_TEST_SIZE)
  {
    eeprom_alive = 0;
  }
}

void loop() 
{   
  digitalWrite(10, HIGH); // HW-boot & fail-led
  digitalWrite(3, HIGH);
  digitalWrite(9, HIGH);
  delay(100); 
  digitalWrite(10, LOW);
  digitalWrite(3, LOW);
  digitalWrite(9, LOW); 
  delay(100); 

  while (!memory_fail){     
    digitalWrite(7, HIGH);   // Turn the Alive-led on.
    digitalWrite(A1, HIGH);  // EM-pulse. A1 need to be connected to the PicoEMP Genio0 for the Fast trigger. EM-pulse can be given also manually. 
    flash_test();
    digitalWrite(7, LOW);    // Turn the Alive-led off.
    digitalWrite(A1, LOW);   // EM-pulse low.
    sram_test();
    eeprom_test();

    if (flash_alive == 0)
    {
        digitalWrite(11, HIGH); // Turn Flash fail led ON
        memory_fail = 1;  // Jump out
    }
    if (sram_alive == 0)
    {
        digitalWrite(5, HIGH); // Turn SRAM fail led ON
        memory_fail = 1;  // Jump out
    }
    if (eeprom_alive == 0)
    {
        digitalWrite(6, HIGH);
        memory_fail = 1;
    }
  }
}