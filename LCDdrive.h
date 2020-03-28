

// FUNCTION ADVANCE DECLARATIONS
void LCD_initialise(void);				// send initialisation commands to LCD
void LCD_clear(void);					// function to clear LCD display and home the cursor
void LCD_putch(unsigned char c);			// character output routine
void LCD_puts(unsigned char *s);                        // Write string s to LCD
void LCD_cursor (unsigned char x, unsigned char y );    // Move cursor to x,y
void LCD_cursor_on (void);                              // Turn cursor on
void LCD_cursor_off (void);                             // Turn cursor off
void LCD_display_value (unsigned int value);           // Display a numeric value (0-999)
