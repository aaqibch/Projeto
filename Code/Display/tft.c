#include "tft.h"
#include "font.h"
#include "img.h"
#include "FreeSans12pt7b.h"

#define HIGH Bit_SET
#define LOW Bit_RESET

#define SET_WR(); LCD_PORT->BSRRL = LCD_WR;
#define SET_RS(); LCD_PORT->BSRRL = LCD_RS;
#define RESET_WR(); LCD_PORT->BSRRH = LCD_WR;
#define RESET_RS(); LCD_PORT->BSRRH = LCD_RS;

#define WIDTH 320
#define HEIGHT 480

uint16_t pins[] = {GPIO_Pin_7,GPIO_Pin_8,GPIO_Pin_9,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14};

void setupPinsTFT(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LCD_PERIPH, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LCD_WR | LCD_RS | LCD_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(LCD_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(DBI_PERIPH, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(DBI_PORT, &GPIO_InitStructure);
}

void pinWrite(uint16_t pin, BitAction val){
	GPIO_WriteBit(LCD_PORT, pin, val);
}

void writeBus(uint8_t d){
	RESET_WR();
	//GPIO_Write(DBI_PORT,(uint16_t) d<<7);
	DBI_PORT->ODR = (uint16_t) d<<7;
	//for(int i=0; i<8; i++)		GPIO_WriteBit(DBI_PORT, pins[i], (BitAction) ((d >> i)  & 0x01));
  SET_WR();
}

void writeCommand(uint8_t com){
	RESET_RS();
  writeBus(com);
}

void writeData(uint8_t data){
	SET_RS();
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
	SET_WR();
	SET_RS();
	pinWrite(LCD_CS, LOW);
	
	writeCommand(0x11);
	
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
	
	writeCommand(0x29);
}

void rectFillTFT(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c){
	int i,j;
	setAdress(x, y, x+w-1, y+h-1);

  for(i=0;i<w;i++)
    for(j=0;j<h;j++)
      writeData16(c);
}
/*
void rectFillTFT(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c){
	int i,j;
	setAdress(y, x, y+h-1, x+w-1);

  for(i=0;i<w;i++)
    for(j=0;j<h;j++)
      writeData16(c);
}*/

void clearTFT(uint16_t c){
	rectFillTFT(0, 0, WIDTH, HEIGHT, c);
}

void hLineTFT(uint16_t x, uint16_t y, uint16_t l, uint16_t c){
	rectFillTFT(x, y, l, 5, c);
}

void vLineTFT(uint16_t x, uint16_t y, uint16_t l, uint16_t c){
	rectFillTFT(x, y, 5, l, c);
}

void letterTFT(uint16_t x, uint16_t y, uint16_t s, uint16_t c, char letter){
	int i, j;
	int l = letter*5;
	
	for(i=0; i<5; i++)
		for(j=0; j<7; j++)
			if(((font[l+i] >> j)  & 0x01))
				rectFillTFT(x+(i*s),y+(j*s),s,s,c);
}

void stringTFT(uint16_t x, uint16_t y, uint16_t s, uint16_t c, char str[]){
	int i;
	while(str[i]){
		letterTFT(x+(i*s*6),y,s,c,str[i]);
		i++;
	}
}

void stringTFT0(uint16_t x, uint16_t y, uint16_t s, uint16_t c, char str[], uint8_t len){
	int i;
	for(i=0; i<len; i++)
		letterTFT(x+(i*s*6),y,s,c,str[i]);
}


void letter2TFT(uint16_t x, uint16_t y, uint16_t s, uint16_t c, char letter){
	
	
	letter -= FreeSans12pt7b.first;
	//GFXglyph *glyph  = FreeSans12pt7bGlyphs[letter];
	//uint8_t  *bitmap = FreeSans12pt7b.bitmap;

	uint16_t bo = FreeSans12pt7bGlyphs[letter].bitmapOffset;
	uint8_t  w  = FreeSans12pt7bGlyphs[letter].width,
					 h  = FreeSans12pt7bGlyphs[letter].height;
	int8_t   xo = FreeSans12pt7bGlyphs[letter].xOffset,
					 yo = FreeSans12pt7bGlyphs[letter].yOffset;
	uint8_t  xx, yy, bits = 0, bit = 0;

	for(yy=0; yy<h; yy++) {
			for(xx=0; xx<w; xx++) {
					if(!(bit++ & 7)) {
							bits = FreeSans12pt7bBitmaps[bo++];
					}
					if(bits & 0x80) {
									rectFillTFT(x+(xo+xx)*s, y+(yo+yy)*s, s, s, c);
					}
					bits <<= 1;
			}
	}
	
	
}

void buttonTFT(uint16_t x, uint16_t y, char str[]){
	uint16_t w=240, h=60;
	rectFillTFT(x,y,w,h,0xFFFF);
	stringTFT0(x+20,y+15,5,0,str,sizeof(&str)+1);
	
	hLineTFT(x,y,w,0);
	hLineTFT(x,y+h,w,0);
	vLineTFT(x,y,h,0);
	vLineTFT(x+w,y,h+5,0);
}

uint16_t RGBto565(uint8_t r, uint8_t g, uint8_t b){
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void loadImg(uint16_t x, uint16_t y){
	int i;
	
	dalt2.arr = dalt2_img;
	dalt2.size = 202248;
	dalt2.w = 320;
	dalt2.h = 320;
	
	setAdress(x, y, x+dalt2.w-1, y+dalt2.h-1);

  for(i=0;i<dalt2.size;i+=2){
    writeData16(dalt2.arr[i+1]<<8 | dalt2.arr[i]);
	}
}

void resizeImg(uint16_t x, uint16_t y, uint16_t s){
	char arr [9];
	uint16_t w = 3, h = 3;
	int i, j;
	int a;
	
	setAdress(x, y, x+(w*s)-1, y+(h*s)-1);
	x = 0;
	y = 0;
	
	for(j=0; j<h; j++){
		if(j%s == 0)
			y++;	
		for(i=0; i<w; i++){
			if(i%s == 0)
				x++;
			a = x + y*w*s;
			a *= 2;
			writeData16(arr[a+1]<<8 | arr[a]);
		}
		x = 0;
	}
	
}
