#include<io16f870.h>
#include"LCDdrive.h"
void del_t(unsigned int t);
void put_data(unsigned char c);
void put_address(unsigned char c);
int read_adc0 ( char c ) ;

void putst(const unsigned char *str);
void del(int t);   
unsigned char get(void);
#pragma vector=0x04   // set inerrupt vector
__interrupt void my_ISR(void);
//const unsigned char message[]={"hello Ali5"};

unsigned char message[20];
unsigned char i,j,x;
int a,c;
void main (void)
 {
 /*select bank1*/
 
        RP0=1;
        OPTION=0x07;
        TRISA=0xff;  //make port a 0utput
        TRISC=0xc0;
        TRISB=0x00;  //make port b output
        ADCON1=0x80;
        SPBRG=25;                                     // BAUD RATE
        TXSTA=0x64; // 9-bit mode
        RCSTA=0xd8; // 9-bit mode with address detect					
         INTCON=0xc0; //enable globle interrupt 
         PIE1=0x20;  // enable peripheral inerrupt   
     /* select bank0*/
   LCD_initialise();  
    
     RP0=0;
  
    
while(1)
{RC4=0;

 
c=(read_adc0(0x05)*.0048876); 
a=(read_adc0(0x0d)*.0048876);
LCD_cursor (0,1 );
 LCD_puts("L.S:"); LCD_display_value(a);

LCD_cursor (9,1 );

 LCD_puts("P.S:");LCD_display_value(c);
del(1);
if(x==1){RC4=1;del_t(1);
if(message[0]=='P'){put_address(0xff);put_data(c);}else 
{put_address(0xff);put_data(a);}
del_t(1);RC4=0; x=0;
LCD_clear();
i=0;
while(message[i]!='5')
{
LCD_putch(message[i]);

i=i+1;
}
}
  
 
  
 
  }}
 __interrupt void my_ISR(void)
  
 {unsigned char address;
      i=0;
    GIE=0;//RCIF=0;
   //TXIF=0;RCIF=0;
   address=get();
  if (address==0xf1){ ADDEN=0;//del(2);
do 
{
 message[i]=get();
//PORTB=reseived_message[i];del_t(1);
i=i+1;
}
while(!(message[i-1]=='5'));
x=1;
}
  ADDEN=1;//RCIF=1;
  GIE=1;
  }
  
  void del_t(unsigned int t)      //delay function generate on second delay
 {   
   unsigned int i;
   for(i=1;i<(5*t);i++)
   {
    TMR0=0;
   while(TMR0-128);}}

 /*function to write data into serial port*/
 
 void put_data(unsigned char c)
 {    
    while(!TXIF)
      {if (OERR)																		
	{
	TXEN=0;									
	TXEN=1;
	CREN=0;
	CREN=1;
	}
if (FERR)																	
	{	
	TXEN=0;	
	TXEN=1;
         }}    
      
      
      RP0=1;
      TX9D=0;
      RP1=0; 
      TXREG=c;
      //PORTB=c;
      //del_t(2);
       }
/*function to read from serial port*/      
unsigned char get(void)
  {    
    while(!RCIF)
         {if (OERR)																		
	{
	TXEN=0;									
	TXEN=1;
	CREN=0;
	CREN=1;
	}
if (FERR)																	
	{	
	TXEN=0;	
	TXEN=1;
         }}    
      
                                 
    return RCREG;
   }
 
 /*delay time function*/      
  void del(int t)                     
{
unsigned int i,j;
for(i=0;i<t;i++)
{
for(j=0;j<2300;j++);
}}
  
 /*function to write address into serial port*/ 
 void put_address(unsigned char c)
 {
    
    while(!TXIF)
      {if (OERR)																		
	{
	TXEN=0;									
	TXEN=1;
	CREN=0;
	CREN=1;
	}
if (FERR)																	
	{	
	TXEN=0;	
	TXEN=1;
         }}    
      
      
      RP0=1;
      TX9D=1;
      RP1=0; 
      TXREG=c;
   //PORTB=c;
     //  del_t(2);
      }
      
 /*function to write string in serial port*/     
 void putst(const unsigned char *str)
{
	while((*str)!=0)
	{
		put_data(*str);
               // del_t(1);
		str++;
	}
}

 /*function to three byte into serial port*/ 
void put_frame(unsigned char a,unsigned char b,unsigned char c)
     {
       put_address(a);
       put_data(b);
       put_data(c);
      
         }  
  

 int read_adc0 ( char c ) 
{
    
    int digital;
    

	/* enable the converter and start a conversion   */
	ADCON0 =c ;    //Channel 0, analoge channel select
	/*  wait for convertion to be over   */
	while ( ADCON0 & 0x04 );
	/* read the  two MSBs  */
	digital = ADRESH;
	digital=digital<<8;
	/* merge MSBs with LSBs */
	return digital+ADRESL;
}
 


      