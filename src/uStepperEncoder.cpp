
/* At initialition setup the SPI hardware protocal to communicate with SSI interface */

uStepperEncoder::uStepperEncoder(void)
{
	/* CS as OUTPUT */
	DDRD |= (1<<CS);

	/* Set MOSI (PB3) and SCK (PB5) as Output */
	DDRB = (1<<DIN)|(1<<CLK);

	/* 
	*  SPE   = 1: SPI enabled
	*  MSTR  = 1: Master
	*  SPR0  = 1 & SPR1 = 0: fOSC/16 = 1Mhz
	*  CPOL  = 1: Idle at HIGH
	*  CPHA  = 0: Sample at leading edge
	*/
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPOL);
}


float uStepperEncoder::getAngle(void){

	uint16_t value = 0; uint8_t stats = 0;

	PORTD &= ~(1<<CS);  // Set CS LOW
	delayMicroseconds(1);

	// Write dummy and read the incoming 8 bits
	value = this.SPI(0x00);
	value <<= 8;

	// Write dummy and read the incoming 8 bits
	value |= this.SPI(0x00);

	// Write dummy and read the incoming 8 bits
	stats = this.SPI(0x00);

	PORTD |= (1<<CS);  // Set CS HIGH

	return value;

}

uint8_t uStepperEncoder::SPI(uint8_t data){

  SPDR = data;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));    
  
  return SPDR;

}