#include<io16f870.h>
#include"LCDdrive.h"
//this is a test for githubs
void del_t(unsigned int t);
void put_data(unsigned char c);
void put_address(unsigned char c);
 void putst(const unsigned char *str);
//void putst(register const char *str);
void del(int t);   
unsigned char get(void);
#pragma vector=0x04   // set inerrupt vector
__interrupt void my_ISR(void);
const unsigned char message[]={"Hello Brouce5"};
const unsigned char message1[]={"Hello Ali5"};
unsigned char recived_message[30];
unsigned char b,x,i;
int a;
void main (void)
 {unsigned int i,c;

 /*select bank1*/
 
        RP0=1;
        OPTION=0x07;
        TRISA=0xcf;  //make port a 0utput
        TRISC=0xc0;
        TRISB=0x00;  //make port b output
        ADCON1=0x06;
        SPBRG=20;                                     // BAUD RATE
        TXSTA=0x64; // 9-bit mode
        RCSTA=0xd8; // 9-bit mode with address detect					
         INTCON=0xc0; //enable globle interrupt 
         PIE1=0x20;  // enable peripheral inerrupt   
     /* select bank0*/
 LCD_initialise();  
     RP0=0;
 // LCD_initialise(); 
a=0;
while(1)
{//PORTB=0;
RC4=0;
           
if(RA0==1){RC4=1;del_t(1);put_address(0xf1);putst(message);}
if(RA1==1){RC4=1;del_t(1);put_address(0xf1);putst(message1);}                                           
if(RA2==1){RC4=1;del_t(1);put_address(0xf2);putst(message);}
if(RA3==1){RC4=1;del_t(1);put_address(0xf2);putst(message1);}
del_t(1);
RC4=0;
LCD_clear();
LCD_display_value(a);
del(1);  
   }}
 __interrupt void my_ISR(void)
  
 {unsigned char address;
    GIE=0;//RCIF=0;
   //TXIF=0;RCIF=0;
   address=get();//del(1);
  if (address==0xff){ ADDEN=0;
a=get();
//b=get();
 x=1;}
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
     // PORTB=c;
     //del_t(1);
       }
/*function to read from serial port*/      
unsigned char get(void)
  {    
    while(!RCIF)
    {
    
if (OERR)																		
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
        {  if (OERR)
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
   //del_t(1);
      }
      
 /*function to write string in serial port*/     
 void putst(const unsigned char *str)
{
	while((*str)!=0)
	{
		put_data(*str);
                //del_t(1);
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