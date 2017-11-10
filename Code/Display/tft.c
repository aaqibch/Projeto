#include "tft.h"
#define HIGH Bit_SET
#define LOW Bit_RESET

#define WIDTH 320
#define HEIGHT 480


void setupPinsTFT(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LCD_PERIPH, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LCD_WR | LCD_RS | LCD_CS | LCD_RST;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LCD_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(DBI_PERIPH, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DBI_PINS;
	GPIO_Init(DBI_PORT, &GPIO_InitStructure);
}

void pinWrite(uint16_t pin, BitAction val){
	
	GPIO_WriteBit(LCD_PORT, pin, val);
}

void writeBus(uint8_t d){
	GPIO_Write(DBI_PORT,(uint16_t) d);
	
	pinWrite(LCD_WR, LOW);
  pinWrite(LCD_WR, HIGH);
}

void writeCommand(uint8_t com){
	pinWrite(LCD_RS, LOW);
  writeBus(com);
}

void writeData(uint8_t data){
	pinWrite(LCD_RS, HIGH);
  writeBus(data);
}

void writeData16(uint16_t data){
	writeData(data>>8);
	writeData(data);
}

void setAdress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	writeCommand(0x2a);
	writeData16(x1);
	writeData16(x2);
	writeCommand(0x2b);
	writeData16(y1);
	writeData16(y2);
	writeCommand(0x2c);
}



void initTFT(void){
	setupPinsTFT();
	pinWrite(LCD_WR, HIGH);
	pinWrite(LCD_RS, HIGH);
	pinWrite(LCD_CS, HIGH);
	pinWrite(LCD_RST, HIGH);
	
	
	pinWrite(LCD_RST,HIGH);
	//delay(5); 
	pinWrite(LCD_RST,LOW);
	//delay(15);
	pinWrite(LCD_RST,HIGH);
	//delay(15);

	pinWrite(LCD_CS,HIGH);
	pinWrite(LCD_WR,HIGH);
	pinWrite(LCD_CS,LOW);

	writeCommand(0x11);
	//delay(20);
	writeCommand(0xD0);
	writeData(0x07);
	writeData(0x42);
	writeData(0x18);
	
	writeCommand(0xD1);
	writeData(0x00);
	writeData(0x07);
	writeData(0x10);
	
	writeCommand(0xD2);
	writeData(0x01);
	writeData(0x02);
	
	writeCommand(0xC0);
	writeData(0x10);
	writeData(0x3B);
	writeData(0x00);
	writeData(0x02);
	writeData(0x11);
	
	writeCommand(0xC5);
	writeData(0x03);
	writeCommand(0x36);
	writeData(0x0A);
	
	writeCommand(0x3A);
	writeData(0x55);
	
	writeCommand(0x2A);
	writeData(0x00);
	writeData(0x00);
	writeData(0x01);
	writeData(0x3F);
	
	writeCommand(0x2B);
	writeData(0x00);
	writeData(0x00);
	writeData(0x01);
	writeData(0xE0);
	//delay(120);
	writeCommand(0x29);
	//writeCommand(0x002c);
}

void minimal(void){
	setupPinsTFT();
	pinWrite(LCD_WR, HIGH);
	pinWrite(LCD_RS, HIGH);
	pinWrite(LCD_CS, HIGH);
	pinWrite(LCD_RST, HIGH);
	
	//minimal dots
	pinWrite(LCD_CS,HIGH);
	pinWrite(LCD_WR,HIGH);
	pinWrite(LCD_CS,LOW);

	writeCommand(0x11);

	writeCommand(0xD0);
	writeData(0x07);
	writeData(0x42);
	writeData(0x18);
	
	writeCommand(0x29);
}

void clearTFT(uint16_t c){
	int i,j;
	setAdress(0, 0, WIDTH, HEIGHT);
  //writeCom(0x02c); //write_memory_start
  //pinWrite(LCD_RS,HIGH);
  pinWrite(LCD_CS, LOW);

  for(i=0;i<WIDTH;i++)
    for(j=0;j<HEIGHT;j++)
      writeData16(c);
	
  pinWrite(LCD_CS, HIGH); 
}

void rectFillTFT(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c){
	int i,j;
	setAdress(0, 0, x+w, y+h);
  writeCommand(0x02c); //write_memory_start
  pinWrite(LCD_RS, HIGH);
  pinWrite(LCD_CS, LOW);

  for(i=0;i<=w;i++)
    for(j=0;j<=h;j++)
      writeData16(c);
	
  pinWrite(LCD_CS, HIGH); 
}

void hLineTFT(uint16_t x, uint16_t y, uint16_t l, uint16_t c){
	rectFillTFT(x, y, l+x, y, c);
}

void vLineTFT(uint16_t x, uint16_t y, uint16_t l, uint16_t c){
	rectFillTFT(x, y, x, l+y, c);
}

uint16_t RGBto565(uint8_t r, uint8_t g, uint8_t b){
	return 0;
}
