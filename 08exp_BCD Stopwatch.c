#include <avr/io.h>
#include <util/delay.h>

// Function declarations
void initialize(void);
unsigned char read_switches(void);
void write_leds(unsigned char x);
void write_upper_ssd(unsigned char n);
void write_lower_ssd(unsigned char n);
unsigned char interpret_switches(unsigned char x, unsigned char *p, unsigned char *num);
//unsigned char decode_switches(unsigned char x);

// decode BCD values to seven-segment display pattern
unsigned char BCD2SSEG(unsigned char);

// executing assembly instruction
void delay_ms(int duration);

int main(void)
{
		unsigned char x,y,z,n;
		unsigned char b_cnt;
		unsigned char sec1,sec10,sseg1,sseg10;
		b_cnt = 0;
		n = 0;
	  sec1 = 0;
		sec10 = 0;

		//initialize ports
		initialize();

		while(1)
		{
			//Read switches and save to x
			x= read_switches();

			//Convert switch pattern to LED pattern
			//y = interpret_switches(x, &z, &n);

			//Write x to the LEDs
			//write_leds(y);

	        // test if SW1 is pressed,
			if (x == 0xFE)
			{
				// clear both digits
			  sec1 = 0;
				sec10 = 0;
				b_cnt = 1;
			}
			else if (x == 0xFD)
			{
				b_cnt = 0;
				/*if (b_cnt == 0)
				{
					b_cnt = 1;
				}
				else b_cnt = 1;*/
			}
			else ;

	        // test if SW2 is pressed,
			if (b_cnt == 1)
			{
		        // delay 1 second _delay_ms(1000);
		        if (sec1>9)
		        {
		            sec1 = 0;
		            sec10++;
		            if (sec10>9)
					{
		            	sec1 = 0;
		                sec10 = 0;
					}
		        }

		        //decode lower digit to seven-segments and display
		        sseg1 = BCD2SSEG(sec1);
				write_lower_ssd(sseg1);

		        //decode upper digit to seven-segments and display
				sseg10= BCD2SSEG(sec10);
				write_upper_ssd(sseg10);

				// increment stop watch sec1++;
				sec1++;
			}

			// delay 0.5 seconds
			delay_ms(1000);


		}
		return 0;
}

// Init Ports
void initialize(void)
{
	//initialize input port (PORT D)
	DDRD = 0x00;
	PORTD = 0xFF;

	//Init output port (PORT B)
	DDRB = 0xFF;
	PORTB = 0xFF;

	//Init output port (PORT A)
	DDRA = 0xFF;
	DDRC = 0xFF;
	//PORTA = 0xFF;


    //decode lower digit to seven-segments and display
	write_lower_ssd(BCD2SSEG(0));

    //decode upper digit to seven-segments and display
	write_upper_ssd(BCD2SSEG(0));
}

// Read from port D (switch port)
unsigned char read_switches(void)
{
	unsigned char y;
	/*
	int duration = 50;
    for(int i = 0; i < duration; i++){
        for(int i = 0; i < 400; i++){
            asm("nop" ::) ;
        }
    }*/
	//save input register value to x
	y = PIND;

	return y;
}

void write_leds(unsigned char x)
{
	PORTB = x;
}

void write_upper_ssd(unsigned char n)
{

	PORTC = n;
}

void write_lower_ssd(unsigned char n)
{

	PORTA = n;
}

/*
unsigned char interpret_switches(unsigned char x, unsigned char *p, unsigned char *num)
{
	unsigned char val,dsp,index;

	val = 0xFF;
	index = 7;
	switch(x)
	{
		case 0xFE:
			val = 0xFE;
			dsp = 0xFC;
			index = 0;
			break;
		case 0xFD:
			val = 0xFC;
			dsp = 0xC0;
			index = 1;
			break;
		case 0xFB:
			val = 0xF8;
			dsp = 0x9E;
			index = 2;
			break;
		case 0xF7:
			val = 0xF0;
			dsp = 0xCE;
			break;
		case 0xEF:
			val = 0xE0;
			dsp = 0xE2;
			break;
		case 0xDF:
			val = 0xC0;
			dsp = 0x6E;
			break;
		case 0xBF:
			val = 0x80;
			dsp = 0x7E;
			break;
		case 0x7F:
			val = 0x00;
			dsp = 0xC8;
			break;
		default:
			val = 0xFF;
			dsp = 0x00;
	};
	dsp = ~dsp;
	*p = dsp;
	*num = index;

	return val;
}*/

/*unsigned char decode_switches(unsigned char x)
{
	unsigned char dsp;

	switch(x)
	{
		case 0xFE:
			dsp = 0xFC;
			//break;
		case 0xFD:
			dsp = 0x60;
			break;
		case 0xFB:
			dsp = 0xDA;
			break;
		case 0xF7:
			dsp = 0xF2;
			break;
		case 0xEF:
			dsp = 0x66;
			break;
		case 0xDF:
			dsp = 0xB6;
			break;
		case 0xBF:
			dsp = 0xBE;
			break;
		case 0x7F:
			dsp = 0xE0;
			break;
		default:
			dsp = 0xFF;
	};
	dsp = ~dsp;

	return dsp;
}*/

void delay_ms(int duration) {
	unsigned char x;
    for(int i = 0; i < duration; i++){
        for(int i = 0; i < 400; i++){
			x= read_switches();
			if(x != 0xFF) return;
            asm("nop" ::) ;
        }
    }
}

unsigned char BCD2SSEG(unsigned char x) {
	unsigned char dsp;

    switch (x)
    {
	      case 0x00: // 0
	          dsp =  0xFC;
	          break;
	      case 0x01:// 1
	          dsp =  0xC0;  //Hex value = 1
	          break;
	      case 0x02:// 2
	          dsp =  0x9E;
	          break;
	      case 0x03:// 3
	          dsp =  0xCE;
	          break;
	      case 0x04:// 4
	          dsp =  0xE2;
	          break;
	      case 0x05:// 5
	          dsp =  0x6E;
	          break;
	      case 0x06:// 6
	          dsp =  0x7E;
	          break;
	      case 0x07:// 7
	          dsp =  0xC8;
	          break;
	      case 0x08:// 8
	          dsp =  0xFE;
	          break;
	      case 0x09:// 9
	          dsp =  0xEE;
	          break;
	      default:
	      	dsp =  0x00; // turn off all LEDs
	 }
	dsp = ~dsp;

	return dsp;
}