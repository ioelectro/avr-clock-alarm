/*
 By Liyanboy74
 https://github.com/liyanboy74
*/

#include <io.h>
#include <alcd.h>
#include <delay.h>
#include <stdio.h>

//Time Variable decelear in EEPROM
eeprom int s=0,m=0,h=0; 

//Alarm Variable decelear in EEPROM
eeprom int sm=0,sh=7; 

//Alarm Enable=0 Disable=1
int A=0;

//LCD State
char ch='L';

//LCD Sprintf Buffer
unsigned char lcd_b[32]; 

// Timer2 overflow interrupt service routine - Every Second
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
//plus second
s++;

//Calculate Time
if(s>=60){m++;A=0;}
if(m>=60)h++;
if(h>=24);//you can continue ..

//Check Ranges
if(s<0||s>=60)s=0;  
if(m<0||m>=60)m=0; 
if(h<0||h>=24)h=0; 
if(sm<0||sm>=60)sm=0; 
if(sh<0||sh>=24)sh=0; 

//if alarm happened and it's Enable, toggel PortD.6 as Alarm! one minute.
if(sm==m&&sh==h&&A==0)PORTD.6=~PORTD.6;
else PORTD.6=0;
}

void main(void)
{
//GPTO Initial
DDRB=0X00;
PORTB=0XFF;
DDRD.6=1;
PORTD.6=0;

// Timer/Counter 2 initialization - for Overflow in one Second!
// Clock source: TOSC1 pin
// Clock value: PCK2/128
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0x08;
TCCR2=0x05;
TCNT2=0x00;
OCR2=0x00;

// Timer/Counter2 Overflow Interrupt Enable
TIMSK=(1<<TOIE2);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
lcd_init(16);

//Enable Global Interupt
#asm("sei")

while (1)
    {
     //Display Time and Alarm
     lcd_gotoxy(0,0);
     sprintf(lcd_b,"%02d:%02d:%02d       %c",h,m,s,ch);
     lcd_puts(lcd_b);
     lcd_gotoxy(0,1);
     sprintf(lcd_b,"alarm=%02d:%02d",sh,sm,);
     lcd_puts(lcd_b);
     lcd_gotoxy(12,1);
     
     //Display Mute
     if(A==1)lcd_puts("mute");
     else lcd_puts("    ");
     
     //Keys Proccess
     delay_ms(10);
     
     //Mute Alarm  
     if(A==0&&(PINB.1==0||PINB.2==0))A=1;
     
     //Config Key Pressed - Pushed
     if(PINB.0==0)
     {
      //Disable Global Interupt 
      #asm("cli")
      
      lcd_clear(); 
      
      //Wait for the release key  
      while(PINB.0==0);
      delay_ms(50);  
      
      //Selected H
      ch='H'; 
      
      lcd_gotoxy(0,0);
      lcd_puts("set time:"); 
      
      //Wait for Press Config Key , For Nex Config 
      while(PINB.0==1)
      {
      lcd_gotoxy(0,1);
      sprintf(lcd_b,"%02d:%02d:%02d       %c",h,m,s,ch);
      lcd_puts(lcd_b);
      delay_ms(20);
      if(PINB.1==0){if(h<23)h++;while(PINB.1==0);delay_ms(50);}
      if(PINB.2==0){if(h>0)h--;while(PINB.2==0);delay_ms(50);} 
      }
       
      //Wait for the release key  
      while(PINB.0==0);
      
      delay_ms(50);
       
      //Selected M
      ch='M';   
      
      //Wait for Press Config Key , For Nex Config
      while(PINB.0==1)
      {
      lcd_gotoxy(0,1);
      sprintf(lcd_b,"%02d:%02d:%02d       %c",h,m,s,ch);
      lcd_puts(lcd_b);
      delay_ms(20);
      if(PINB.1==0){if(m<58)m++;while(PINB.1==0);delay_ms(50);}
      if(PINB.2==0){if(m>0)m--;while(PINB.2==0);delay_ms(50);} 
      }
      
      //Wait for the release key  
      while(PINB.0==0);    
      
      delay_ms(50);
      
      //Selected S 
      ch='S';    
      
      //Wait for Press Config Key , For Nex Config 
      while(PINB.0==1)
      {
      lcd_gotoxy(0,1);
      sprintf(lcd_b,"%02d:%02d:%02d       %c",h,m,s,ch);
      lcd_puts(lcd_b);
      delay_ms(20);
      if(PINB.1==0){if(s<58)s++;while(PINB.1==0);delay_ms(50);}
      if(PINB.2==0){if(s>0)s--;while(PINB.2==0);delay_ms(50);} 
      }  
      
      //Wait for the release key  
      while(PINB.0==0); 
      
      delay_ms(50);
      
      //ReEnable Global Interupt For Calculating Time  
      #asm("sei") 
      
      //Selected H
      ch='H';    
      
      lcd_gotoxy(0,0);
      lcd_puts("set alarm:");
      
      //Wait for Press Config Key , For Nex Config  
      while(PINB.0==1)
      {
      lcd_gotoxy(0,1);
      sprintf(lcd_b,"%02d:%02d          %c",sh,sm,ch);
      lcd_puts(lcd_b);
      delay_ms(20);
      if(PINB.1==0){if(sh<23)sh++;while(PINB.1==0);delay_ms(50);}
      if(PINB.2==0){if(sh>0)sh--;while(PINB.2==0);delay_ms(50);} 
      } 
      
      //Wait for the release key  
      while(PINB.0==0);
      
      delay_ms(50);
      
      //Selected M
      ch='M';      
      
      //Wait for Press Config Key , For Nex Config 
      while(PINB.0==1)
      {
      lcd_gotoxy(0,1);
      sprintf(lcd_b,"%02d:%02d          %c",sh,sm,ch);
      lcd_puts(lcd_b);
      delay_ms(20);
      if(PINB.1==0){if(sm<58)sm++;while(PINB.1==0);delay_ms(50);}
      if(PINB.2==0){if(sm>0)sm--;while(PINB.2==0);delay_ms(50);} 
      } 
      
      //Wait for the release key  
      while(PINB.0==0);       
      
      delay_ms(50); 
      
      //Setup Completed and Screen is Lock! 
      ch='L';   
      
      //Alarm Enabel  
      A=0;       
      
      lcd_clear(); 
     } 
    }   
}
