#include <avr/io.h>
#include <stdlib.h>
#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#define BAUD 4800
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)


#include <util/delay.h>
#include <stdlib.h>

#define LCD_Dir DDRB
#define LCD_Port PORTB
#define RS PB0
#define EN PB1

void LCD_Command( unsigned char cmnd ){
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);
	LCD_Port &= ~ (1<<RS);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);
	LCD_Port |= (1<<RS);
	LCD_Port|= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (data << 4);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void intToString(int number, char* buffer, int bufferSize) {
	itoa(number, buffer, 10);
}

void LCD_Init (void){
	LCD_Dir = 0xFF;
	_delay_ms(20);
	LCD_Command(0x02); //return home mengembalikan ketampilan bagian awal
	LCD_Command(0x28); //2 baris, 5x8 Matriks, mode 4bit
	LCD_Command(0x0c); //Menyalakan display, Cursor mati
	LCD_Command(0x06); //Geser kursor ke kanan
	LCD_Command(0x01); //clrscreen (bersihin layar)
	_delay_ms(2);
}

void LCD_String (char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)
	{
		LCD_Char (str[i]);
	}
}



void LCD_String_xy (char row, char pos, char *str)
{
	if (row == 0 && pos<8)
	LCD_Command((pos & 0x0F)|0x80);
	else if (row == 1 && pos<8)
	LCD_Command((pos & 0x0F)|0xC0);
	LCD_String(str);
}

void LCD_Clear() {
	LCD_Command (0x01);
	_delay_ms(2);
	LCD_Command (0x80); //Meletakkan kursor di baris pertama

}


void ADC_init(void)
{
	ADMUX |= (0<<REFS1) | (1<<REFS0);
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS1);
	ADCSRA |= (1<<ADSC);
	while ((ADCSRA & (1<<ADSC)) != 0);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
	long a = (x - in_min) * (out_max - out_min);
	long b = a / (in_max - in_min);
	return b + out_min;
}

int main(void)
{
	DDRA = 0x00;
	PORTA = 0x00;
	DDRC = 0xFF;
	PORTC = 0xFF;

	long potensiometer;
	ADC_init();

	LCD_Init();

	while (1)
	{
		ADCSRA |= (1<<ADSC);
		while ((ADCSRA & (1<<ADSC)) != 0);


		potensiometer = map(ADC, 0, 1023, 0, 100);




		char strBuffer[10]; // Adjust the size based on the maximum expected digits




		if (potensiometer <= 10) {
			PORTC = 0xFF;
		}
		else if (potensiometer > 10 && potensiometer <= 25) {
			PORTC = 0b11111100;
		}
		else if (potensiometer > 25 && potensiometer <= 50) {
			PORTC = 0b11110000;
		}
		else if (potensiometer > 50 && potensiometer <= 75) {
			PORTC = 0b11000000;
		}
		else if (potensiometer > 75 && potensiometer <= 100) {
			PORTC = 0x00;
		}

		intToString(potensiometer, strBuffer, sizeof(strBuffer));

		LCD_String("DebitAir");
		LCD_Command(0xC0);
		LCD_String(strBuffer);
		LCD_String_xy(1,4,"%");

		_delay_ms(1000);

		LCD_Clear();
	}
}

//TERIMAKASIH
