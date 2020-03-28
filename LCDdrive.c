#include <io16F870.h>

// LCD Driver module
// LCD module is connected as follows:
// RB0 - RB3 -> DB4 - DB7
// RB4 -> RS
// RB5 -> E

// define constants
#define RS RB4			// RS control line is bit 4 of port B
#define E  RB5			// E control line is bit 5 of port B 
#define LCD_INIT 0x33		// command to initialise LCD display
#define FOUR_BIT_MODE 0x32	// command to make the LCD transfer instructions and data in 4 bit chunks rather than 8
#define TWO_LINE_MODE 0x2c	// command to put the LCD in two-line mode
#define MOVE_AFTER_WRITE 0x06	// command to increment cursor position automatically
#define DISPLAY_INIT 0x0c	// command to turn display on, switch off the cursor and stop it blinking

#define WRITE_DELAY 200		// constant used to set write delay time (50 us required)
#define BIT_DELAY 2		// constant used to set pulse duration (a few us required)
#define INST_DELAY 1000		// constant used to det instruction delay (1.53 ms required)

// FUNCTION EXTERNAL FUNCTION REFERENCES


// FUNCTION ADVANCE DECLARATIONS
void LCD_write(unsigned char c, unsigned char rs);	// basic write function (instruction or data depending on rs)
void LCD_initialise(void);				// send initialisation commands to LCD
void LCD_clear(void);					// function to clear LCD display and home the cursor
void LCD_delay(unsigned int D);				// function to generate a delay 
void pulse_E(void);					// function to pulse E pin high then low
void LCD_putch(unsigned char c);			// character output routine
void LCD_puts(unsigned char *s);                        // Writes string s to LCD
void LCD_cursor (unsigned char x, unsigned char y );    // Moves cursor to x,y
void LCD_cursor_on (void);                              // Turn cursor on
void LCD_cursor_off (void);                             // Turn cursor off
void LCD_display_value (unsigned int value);           // Display a numeric value (0-999)

// Initialise LCD module
void LCD_initialise(void)
{

	// INITIALISE LCD
	LCD_write(LCD_INIT, 0);		// command to initialise LCD display, (RS = 0)
	LCD_delay(INST_DELAY);		// call function to wait until LCD has completed instruction

	// Set LCD for 4-bit mode
	LCD_write(FOUR_BIT_MODE, 0);	// command to set LCD in 4-bit mode, (RS = 0)
	LCD_delay(INST_DELAY);		// call function to wait until LCD has completed instruction

	// set LCD in up for two lines
	LCD_write(TWO_LINE_MODE, 0);	// command to set LCD for two lines, (RS = 0)
	LCD_delay(INST_DELAY);		// call function to wait until LCD has completed instruction

	// Select move after write	
	LCD_write(MOVE_AFTER_WRITE, 0);	// command to set LCD to increment cursor pos. automatically (RS = 0)
	LCD_delay(INST_DELAY);		// call function to wait until LCD has completed instruction

	// Configure display & cursor
	LCD_write(DISPLAY_INIT, 0);	// command to configure display, (RS = 0)
	LCD_delay(INST_DELAY);		// call function to wait until LCD has completed instruction
	
	LCD_clear();
}


	
// LCD_write() function is a basic write subroutine that could be an instruction or data
void LCD_write(unsigned char c, unsigned char rs)
{
	unsigned char temp;		// temporary variable to hold ms 4 bits of c
	unsigned char RSmask = 0;	// used to set or reset bit 4 of PORTB
	unsigned char TRISB_BAK;        // Current status of port B data direction register

	// INITIALISATION
	RP0 = 1;        		// get ready to write to data direction registers
	TRISB_BAK = TRISB;              // Get current state of TRISB register
	TRISB = 0x00;			// Set PORTB bits 0 - 7 as outputs
	RP0 = 0;		        // get ready to use ports

	if (rs == 1) RSmask = 0x10;	// set up bit mask for RS line

	// set MS 4 bits of c up on RB0 - RB3
	temp = c;
	PORTB = (temp >> 4) | RSmask;	// shift MS 4 bits into LS 4 bits, set/rst RS and present on PORTB

	// pulse E high then low
	pulse_E();			// call function to strobe data to LCD module

	// set LS 4 bits up on RB0 - RB3
	PORTB = (c & 0x0F) | RSmask;	// present LS 4 bits on RB0 - RB3 and set/rst RS line

	// pulse E high then low
	pulse_E();			// Call function to strobe data to LCD module
	
	PORTB = 0x10;			// set E high again  to avoid annoying LED flicker

	RP0 = 1;		        // get ready to write to data direction registers
	TRISB = TRISB_BAK;		// Restore state of TRISB register
	RP0 = 0;		        // get ready to use ports
}


//putch(c) function is character output routine
void LCD_putch(unsigned char c)
{
	LCD_write(c, 1);		// call basic LCD write routine with RS high (send data)
	LCD_delay(WRITE_DELAY);		// call function to wait until LCD has completed write operation
}

//LCD_clear() function is an instruction to clear all data from LCD display and return cursor to beginning of display
void LCD_clear(void)
{
	LCD_write(1, 0);		// call basic write function with instruction 1, (clear display) and RS 0, (write instruction)
	LCD_delay(INST_DELAY);		// Call function to wait until LCD has completed instruction
	LCD_write(2, 0);		// call basic write function with instruction 2, (cursor home) and RS = 0, (write instruction)
	LCD_delay(INST_DELAY);		// call function to wait until LCD has completed instruction
}

// function to pulse E pin high then low with a duration of a few usecs
void pulse_E(void)
{
	// set E high and wait a bit
	E = 1;		                // set E line high
	LCD_delay(BIT_DELAY);		// pulse delay for E

	// set E low and wait a bit
	E = 0;		                // set E line low
	LCD_delay(BIT_DELAY);		// pulse delay for E	
	
}

// Function to generate a delay
void LCD_delay(unsigned int D)
{
	unsigned int counter;		// variable used as delay loop counter
	for (counter = 0; counter < D; counter++);
}

// Function to write null terminated string to LCD display
void LCD_puts(unsigned char *s)
{
        unsigned char c = 255;
        unsigned char i = 0;
        while ((c > 0) && (i < 16)) //16 is the maximum length of the string
        {
          c = s[i];
          if (c != 0) LCD_putch(c);
          i++;
        }
}        

void LCD_cursor (unsigned char x, unsigned char y )
{
    if ( y==0 ) 
    {
        /* position for line 0     */
        y=0x80 ;
    }
    else 
    {
        /* position for line 1     */
        y=0xc0 ;
    }
    
    LCD_write( y+x, 0) ;
}

void LCD_cursor_on ( void )       // Turn cursor on
{
    LCD_write(0x0f, 0);
}

void LCD_cursor_off ( void )      // Turn cursor off
{
    LCD_write (0x0c, 0);
}

void LCD_display_value ( unsigned int value ) 
{
    unsigned char hunds, tens, units ;
    
    /*  first get the digits  */
    
    units = value % 10 ;
    value = value / 10 ;
    tens = value % 10 ;
    value = value / 10 ;
    hunds = value % 10 ;
    
    /*  now display them      */ 
    
    LCD_putch('0' + hunds ) ;
    LCD_putch('0' + tens);
    LCD_putch('0' + units);
}
