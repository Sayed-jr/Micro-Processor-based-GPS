#ifndef __LCD_H__
#define	__LCD_H__

#include "stdint.h"
#define pi 3.14;
#define CPAC (*((volatile uint32_t)0xE000ED88))

void SystemInit(){}

/**delay**/
void delay( uint32_t d );

void init_portF();

void LCD_command(unsigned char com);


void LCD_DATA(unsigned char data);


void init(void);

/**LCD_displayCharacter**/
void LCD_displayCharacter(uint8_t data);


/***LCD_displayString**/
void LCD_displayString(const char *Str);

/**countDigit**/

int countDigit(long long n);




// Reverses a string 'str' of length 'len'
void reverse(char* str, int len);


// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d);



/**Exceed_distance***/
void Exceed_distance(long distance);

 double toRadians(double degree);


double distance(double lat1, double long1, double lat2, double long2);


#endif // __LCD_H__