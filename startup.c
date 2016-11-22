/*
 * 	startup.c
 *
 */
 #include <string.h>
#define GPIO_D 0x40020C00
#define GPIO_MODER  ((volatile unsigned int *) (GPIO_D))  
#define GPIO_OTYPER  ((volatile unsigned short *) (GPIO_D+0x4))  
#define GPIO_PUPDR ((volatile unsigned int *) (GPIO_D+0xC))  
#define GPIO_IDR_LOW ((volatile unsigned char *) (GPIO_D+0x10))  
#define GPIO_IDR_HIGH  ((volatile unsigned char *) (GPIO_D+0x11))  
#define GPIO_ODR_LOW ((volatile unsigned char *) (GPIO_D+0x14))  
#define GPIO_ODR_HIGH ((volatile unsigned char *) (GPIO_D+0x15))  
void startup(void) __attribute__((naked)) __attribute__((section (".start_section")) );


void startup ( void )
{
asm volatile(
	" LDR R0,=0x2001C000\n"		/* set stack */
	" MOV SP,R0\n"
	" BL main\n"				/* call main */
	".L1: B .L1\n"				/* never return */
	) ;
}
unsigned char keyb(void);


void out7seg(unsigned char c){
	unsigned char key[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
	
	if(c <16){
		*GPIO_ODR_LOW = key[c];
		
		}
	else{
		
		*GPIO_ODR_LOW = 0x00;
		}
	}
void app_init(void){
	
	*GPIO_MODER = 0x55005555;
	*GPIO_OTYPER &= 0x00FF;
	*GPIO_OTYPER |= 0x0500;
	*GPIO_PUPDR &= 0x0000FFFF;
	*GPIO_PUPDR |= 0x00550000;

	}

void activateRow(unsigned int row){
	switch(row){
		case 1: *GPIO_ODR_HIGH = 0x10;break;
		case 2: *GPIO_ODR_HIGH = 0x20;break;
		case 3: *GPIO_ODR_HIGH = 0x40;break;
		case 4: *GPIO_ODR_HIGH = 0x80;break;
		case 0: *GPIO_ODR_HIGH = 0x00;break;
		default: return;
		}
	
	}
	
int getCol(void){
	
	
	unsigned char c;
	c = *GPIO_IDR_HIGH;
	if(c & 0x8) return 4;
	if(c & 0x4) return 3;
	if(c & 0x2) return 2;
	if(c & 0x1) return 1;
	return 0;
	}
	
	
unsigned char keyb(void){
	unsigned char key[]={1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};
	int row,col;
		for(row=1; row<=4; row++) {
		activateRow(row);
		if((col= getCol() ) )
			{
			activateRow(0 );
			return key[4*(row-1)+(col-1)];
		}
		}
	activateRow(0 );
	unsigned char c = 0xFF;
	return c;
}

int main(void)
{
	
	app_init();
	while(1){
		unsigned char c = keyb();
		out7seg(c);
		}
	return 0;
}