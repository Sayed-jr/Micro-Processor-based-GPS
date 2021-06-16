#include "tm4c123gh6pm.h"
#include "UART.h"
#include "string.h"
#include "stdint.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#define pi 3.14;
#define CPAC (*((volatile uint32_t)0xE000ED88))
void EnableInterrupts(void){};
void WaitForInterrupt(void){} ; 
uint8_t counter = 0;
uint32_t RGB_color[8] = {0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E};
//                       0000  0010  0100  0110  1000  1010  1100  1110
//                       off   red   green yellw blue  pink  cyan  white
	/*******************************************************************LCD********************************************************************/
		void init_portF()
{
	SYSCTL_RCGCGPIO_R |= 0x00000020; //init clock reg
	delay(50);
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F; // FOR ALL PINS I USE
	delay(50);
	/* make the Switches as input and Leds are output*/
	GPIO_PORTF_AFSEL_R = 0x00 ; // neednt
	GPIO_PORTF_PCTL_R = 0x00000000 ; //neednt
	GPIO_PORTF_AMSEL_R = 0x00 ; //neednt
	
	//Now we set the Directions/
	GPIO_PORTF_DIR_R = 0x0E; // 
	GPIO_PORTF_DEN_R = 0x1F;//
	GPIO_PORTF_PUR_R = 0x11; // FOR THE SWITCHES AT PIN 0 AND 4
	// INTERRUPT INITIALIZATION
  // negative edge triggered (falling) configuration
  GPIO_PORTF_IS_R &= ~0x10;  //edge sensitive interrupt
  GPIO_PORTF_IBE_R &= ~0x10; //single edge
  GPIO_PORTF_IEV_R &= ~0x10; //falling edge
  GPIO_PORTF_ICR_R |= 0x10;  //clear interrupt signal of PF4 (switch)
  GPIO_PORTF_IM_R |= 0x10;   // arm interrupt on PF4 (switch)

  // setting Port F priority (setting priority to 2)
  NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00400000;
  NVIC_EN0_R |= (1 << 30); // enable IRQ from Port F (bit 30 of EN0 register)
  EnableInterrupts(); //enable interrupts
	
}
void GPIOF_Handler(double x,double y)
{
  GPIO_PORTF_ICR_R |= 0x10; //clear interrupt signal of PF4 (switch)
  //el 7aga elly hn3mlha
  GPIO_PORTF_DATA_R = RGB_color[counter];
  counter++;
  if (counter == 8)
    counter = 0;
}

void LCD_command(unsigned char com)
{	
	delay(50);
	GPIO_PORTA_DATA_R &=~0x20; //Rs and RW are 0
	delay(2);
	GPIO_PORTB_DATA_R = com;
	delay(2);
	GPIO_PORTA_DATA_R |= 0x80;
	delay(2);
	GPIO_PORTA_DATA_R &=~0x80; 
	delay(2);
}

void LCD_DATA(unsigned char data)
{
	SYSCTL_RCGCGPIO_R |= 0x00000020; 
	GPIO_PORTA_DATA_R |= 0x20; // Make RS = 1 and RW =0     /* 0010_0000 A5
	GPIO_PORTB_DATA_R = data; // take the data
	GPIO_PORTA_DATA_R |= 0x80; //enable E                   /*   10000000 A7 
	delay(2);
	GPIO_PORTA_DATA_R &=~0x80;    // Disable E
	delay(2);
	GPIO_PORTF_DATA_R = 0x02 ;
	delay(2);
}

void init(void)
{
	delay(50); 
	SYSCTL_RCGCGPIO_R |= 0x01; //init Port A&B clk
	SYSCTL_RCGCGPIO_R |= 0x02; //init Port A&B clk
//For Port B
	delay(50) ;
	GPIO_PORTB_LOCK_R=0x4C4F434B;
	GPIO_PORTB_CR_R = 0xFF; // FOR ALL PINS I USE
	GPIO_PORTB_DIR_R=0xFF; // to enable all the pins in port B
	GPIO_PORTB_DEN_R=0XFF;// to make them degital
	GPIO_PORTB_AFSEL_R =0; // neednt
	GPIO_PORTB_PCTL_R = 0; //neednt
	GPIO_PORTB_AMSEL_R = 0; //neednt
	GPIO_PORTB_PUR_R = 0 ;
//For Port A
	//GPIO_PORTA	_CR_R = 0x1F; // FOR ALL PINS I USE
	GPIO_PORTA_LOCK_R=0x4C4F434B;
	GPIO_PORTA_CR_R = 0xF0; // FOR ALL PINS I USE
	GPIO_PORTA_DIR_R=0xE0;
	GPIO_PORTA_DEN_R=0xE0;
	GPIO_PORTA_AFSEL_R =0; // neednt
	GPIO_PORTA_PCTL_R = 0; //neednt
	GPIO_PORTA_AMSEL_R = 0; //neednt
	GPIO_PORTA_PUR_R = 0 ;
	delay(2);
	//LCD_command(0x03);	
	delay(2);
	LCD_command(0x06);	/* Increment cursor (shift cursor to right) */
	delay(2);
	LCD_command(0x0E);
	delay(2);
	LCD_command(0x38);	/* 2 line, 5*7 matrix in 4-bit mode */
	delay(2);
	LCD_command(0x01);	/* Clear display screen */
	delay(2);
}

/**LCD_displayCharacter**/
void LCD_displayCharacter(uint8_t data)
{
	delay(1); /* delay for processing Tpw - Tdws = 190ns */
	LCD_DATA(data); /* out the required data char to the data bus D0 --> D7 */
	delay(1); /* delay for processing Tdsw = 100ns */
}

/***LCD_displayString**/
void LCD_displayString(const char *Str)
{
	uint8_t i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}

/**countDigit**/

int countDigit(long long n)
{
    int count = 0;
    while (n != 0)
    {
        n = n / 10;
        ++count;
    }
    return count;
}



// Reverses a string 'str' of length 'len'
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
  
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
  
    reverse(str, i);
    str[i] = '\0';
    return i;
}



/**Exceed_distance***/
void Exceed_distance(long distance){
	  
	if(distance >= 100 ){
		int digit ;
		int i ;
		char *b ;
		b = (char*) malloc(digit * sizeof(char)) ;
		intToStr(distance , b , 3) ;
	  LCD_displayString(b) ;
		
		GPIO_PORTF_DATA_R = 0x08; // 1000 blink green led
		delay(50);
		
		}
	else {
		int digit ;
		int i ;
		char *b ;
		b = (char*) malloc(digit * sizeof(char)) ;
		intToStr(distance , b , 3) ;
	  LCD_displayString(b) ;
		}
		
}


 double toRadians(double degree)
{
    // cmath library in C++
    // defines the constant
    // M_PI as the value of
    // pi accurate to 1e-30
	  
     double one_deg = ( 3.141592653589793 / 180);
    return (one_deg * degree);
}

double distance(double lat1, double long1, double lat2, double long2)
{
    // Convert the latitudes
    // and longitudes
    // from degree to radians.
		double x,y,z,k,dlong,dlat, ans,R;
    x = toRadians(lat1);
    y = toRadians(long1);
    z= toRadians(lat2);
    k = toRadians(long2);

    // Haversine Formula
   // double  dlong =5.25 ;
	   dlong= z - y;
     dlat = z - x;
 
    ans = pow(sin(dlat / 2), 2) + cos(x) * cos(y) * pow(sin(dlong / 2), 2);
 
 //   ans = 2 * asin(sqrt(ans));
 
    // Radius of Earth in
    // Kilometers, R = 6371
    // Use R = 3956 for miles
    R =4;// = 6371;
     
    // Calculate the result
   ans = ans * R;
 
  return ans;
}



	/**********************************************************************************************************************************/
//#include <cmath>

/*gps*/
void delay2(void)
{
	int i,j;
	 for ( i = 0 ; i < 100 ; i++ ){
  for ( j = 0 ; j < 16000 ; j++ ){
//empty
}}
}

/*double atof2(char *s){
    int i;
	  int power2;
	  int sign;
		double value;
	  double power;
	  int powersign; //The sign following the E/
    for(i = 0; isspace(s[i]); ++i);
        //skip white space/

    
    sign = (s[i] == '-')? -1 : 1; //The sign of the number/

    if(s[i] == '-' || s[i] == '+'){
        ++i;
    }

    
    for(value = 0.0; isdigit(s[i]); ++i){
        value = value * 10.0 + (s[i] - '0');
    }

    if(s[i] == '.'){
        ++i;
    }

    
    for(power = 1.0; isdigit(s[i]); ++i){
        value = value * 10.0 + (s[i] - '0');
        power *= 10.0;
    }

    if(s[i] == 'e' || s[i] == 'E'){
        ++i;
    }
    else{
        return sign * value/power;
    }

    
    powersign = (s[i] == '-')? -1 : 1;

    if(s[i] == '-' || s[i] == '+'){
        ++i;
    }

     //The number following the E/
    for(power2 = 0; isdigit(s[i]); ++i){
        power2 = power2 * 10 + (s[i] - '0');
    }

    if(powersign == -1){
        while(power2 != 0){
            power *= 10;
            --power2;
        }
    }
    else{
        while(power2 != 0){
            power /= 10;
            --power2;
        }
    }

    return sign * value/power;
}
*/

float ad_lat(float lat ) {

	float r;
	float d;
	float a;
	float temp;
	int h;
	int whole;

	h = (int)(lat); // the whole float lat to int --> 3006.136546 --> 3006
	a =  ((float)(h)) / 100; // 3006 --> 30.06
    temp = lat- h ; // 0.136546
	whole = (int)(a); // 30.06 --> 30
	d = (a - whole)*100+temp; // 30.06-30 --> 0.06

	return (whole + d/60);

}

/***********************/

float get_lon(int index[], char arr[]) {

   // printf("Hello From") ;

		float longitude;
    int q = 0 , in ;
    char lon[11];
    for ( in = index[3] + 1; in < index[4]; in++) {

        lon[q] = arr[in];
        //printf("%c",lon[q]);
        q++;


    }
		longitude = atof(lon) ;
   // cout << longitude;
    return longitude;
}
/**********************/
double ad_lon(double lon) {

    double r;
    double d;
    double a;
    double temp;
    int h;
    int whole;

    h = (int)(lon); // the whole float lat to int --> 3006.136546 --> 3006
    temp = lon-h;   // 0.136546
    a = ((double)(h)) / 100; // 3006 --> 30.06
    whole = (int)(a); // 30.06 --> 30
    d = (a - whole)*100+temp; // 30.06-30 --> 0.06

    return (whole + (d/60));

}
/**********************/
/**********************/


/*float get_lat(int *index, char* arr) {
   // printf("Hello From LAT") ;
 char lat[10];
float lat2;
	int i;
	int q = 0;
    int in;
    
	for ( in = index[1] + 1; in < index[2] ; in++) {

		lat[q] = arr[in];
		//printf("%c",lat[q]);
		q++;

	}
    lat2 = atof(lat);

	return lat2;
}


/**/

/*****************************************************/









/********/
int main(void){

	
	/*
	int i;
	char input;
	char lat_c[100],log_c[100];
	float logg[100],lat[100],ss;
	char arr[400];
	int counter;
	UART0_Init();
	UART2_init() ;
	delay2();
	
	
	*/
//	char ak[5]={'3','1','.','2','4'};
//	double temp;
	
	
		char lon[11];
	   double r;
    double d;
    double a;
    double temp;
    int h;
    int whole;
	int power2;
	  int sign;
		double value;
	  double power;
	  int powersign;
	volatile unsigned in;
	char arr[400];
  int index_comma[20],i,j;
	int k=0,start;
	//volatile int ;
	volatile int flag2=0;
	int q = 0  ;

	// var of get lat
	char lat[10];
	volatile double lat2,longitude;
	volatile double out2,out;
	char* pEnd;
	UART0_Init();
	UART2_init() ;
		init();
	init_portF() ;
	delay2();
	
	

		while(1){
		//temp= strtod(ak,NULL);
		/************************Delay ***************/
				//$GPGGA,231810.00,3006.17106,N,03118.27989,E
		/*********************************************/
				for(i = 0; i < 400; i++){
					arr[i]=UART2_Read(); // scanf
					UART0_Write(arr[i]); //printf	
					}
		/**************************************/
					k=0;
					for (i = 0; i < 400; i++) {

								if (arr[i] == '$' && arr[i + 1] == 'G' && arr[i + 2] == 'P' && arr[i + 3] == 'G' && arr[i + 4] == 'G' && arr[i + 5] == 'A') {
								//	flag=1;
								// printf("found sequance") ;
								//printf("\n") ;
								start = i + 6;
								for (j = start ; j<start+75; j++) {
								if(arr[j]=='E') break;
								if (arr[j] == ',') {
								index_comma[k] = j;
								//printf("%d",index_comma[k]) ;
								//printf("\n") ;
								k++;
								// printf("done with the commas") ;
								//printf("\n") ;
								}
								}
								}
								}
									
			
				
					//in = index_comma[3] + 4;
				//	lat[0] = arr[in];			
			//out2 = get_lat( &index_comma[20],&arr[400]);			
				/********************/
				in = index_comma[3] + 1;
				for (q=0;q<10;q++) {
			//		in = index_comma[3] + 1; in < index_comma[4]; in++)
						flag2 = 5 ;
							lat[q] = arr[in];
							//printf("%c",lon[q]);
							in++;			
				}		
								//376    0x31 '1'
				lat2 = strtod(lat,NULL);	
					//flag2 = 5 ;
			//	out2 = lat2;
				/*********************/						
			//	out2=  ad_lat(lat2);
				h = (int)(lat2); // the whole float lat to int --> 3006.136546 --> 3006
				a =  ((float)(h)) / 100; // 3006 --> 30.06
				temp = lat2- h ; // 0.136546
				whole = (int)(a); // 30.06 --> 30
				d = (a - whole)*100+temp; // 30.06-30 --> 0.06
				out2=(whole + d/60);
				
				
				//	out = get_lon(index_comma, arr);
				in =index_comma[1] + 1;
						
    for ( q=0;q<10;q++) {

        lon[q] = arr[in];
        //printf("%c",lon[q]);
        in++;
    }
		longitude = strtod(lon,NULL) ;
   // cout << longitude;
   // return longitude;
	//	out = ad_lon(out);
			h = (int)(longitude); // the whole float lat to int --> 3006.136546 --> 3006
			a =  ((float)(h)) / 100; // 3006 --> 30.06
			temp = longitude- h ; // 0.136546
			whole = (int)(a); // 30.06 --> 30
			d = (a - whole)*100+temp; // 30.06-30 --> 0.06
			out=(whole + d/60);
		GPIOF_Handler(out,out2);
			UART0_Write(out);
		UART0_Write(out2);
/**************************LCD*****************************/
		
	LCD_command(0x80);
	LCD_DATA('D');
	LCD_DATA('I');
	LCD_DATA('S');
	LCD_DATA('T');
	LCD_DATA('A');
	LCD_DATA('N');
	LCD_DATA('C');
	LCD_DATA('E');
	LCD_DATA('=');
	Exceed_distance(150);
	
	}
}






