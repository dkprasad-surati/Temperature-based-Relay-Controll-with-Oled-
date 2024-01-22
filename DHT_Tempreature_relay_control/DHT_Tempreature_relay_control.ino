//Programa: Arduino e DHT11 controle de rele por temperatura
//Autor: Arduino e Cia

#include "U8glib.h"
#include <DHT.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

int posicao = 0;
int temp_max = 30;

#define pino_aumenta 5  // D5 Up   Button
#define pino_diminui 4  // D4 Down Button
#define pino_rele 7     // D7 Relay Pin
#define pino_DHT A0     // A0 DHT Signal Pin
#define DHTTYPE DHT11

DHT dht(pino_DHT, DHTTYPE);

int temperatura;
int aumenta, diminui;

unsigned long previousMillis = 0;
//Intervalo de medicao temperatura
const long interval = 10000;

void draw()
{
//Retangulo temperatura atual
u8g.drawRFrame(0, 17, 84, 46, 2);
u8g.setFont(u8g_font_profont29 );// u8g_font_fub20
//Atualiza a temperatura no display
u8g.setPrintPos(9, 50);
u8g.print(temperatura);
//Circulo grau
u8g.drawCircle(51, 33, 3);
u8g.drawStr( 56, 50, "C");

//Box superior amarelo
u8g.setFont(u8g_font_tpss);
u8g.drawRBox(0, 0, 128, 18, 0);
u8g.setColorIndex(0);
u8g.drawStr( 9, 13, "Meril CelQuant 3 Prime");
u8g.setColorIndex(1);

//box temperatura maxima
u8g.drawRFrame(86, 17, 42, 46, 2);
if (temp_max <= temperatura)
{
//Temperature greater than temp. SET
u8g.drawRBox(86, 17, 42, 22, 2);
u8g.setColorIndex(0);
u8g.drawStr(96, 33, "SET");
u8g.setColorIndex(1);
//Aciona saida do rele
digitalWrite(pino_rele, LOW);

}
else
{
//Temperature lower than temp. SET
u8g.drawRFrame(86, 17, 42, 22, 2);
u8g.drawStr(96, 33, "SET");
//Desliga saida do rele
digitalWrite(pino_rele, HIGH);
}
//Atualiza na tela o valor da temp. maxima
u8g.setPrintPos(100, 55);
u8g.print(temp_max);
u8g.drawCircle(120, 47, 2);
u8g.setColorIndex(1);
}

void setup(void)
{
Serial.begin(9600);
pinMode(pino_rele, OUTPUT);
pinMode(pino_aumenta, INPUT);
pinMode(pino_diminui, INPUT);
dht.begin();
}

void loop(void)
{
unsigned long currentMillis = millis();
//Timer para ler o valor da temperatura
if (currentMillis - previousMillis >= interval)
{
temperatura = dht.readTemperature();
previousMillis = currentMillis;
}

//Testa botao aumenta temperatura
aumenta = digitalRead(pino_aumenta);
if (aumenta == 1)
{temp_max++;}
while (digitalRead(pino_aumenta) == 1)
{delay(100);}

//testa botao diminui temperatura
diminui = digitalRead(pino_diminui);
if (diminui == 1)
{temp_max--;}
while (digitalRead(pino_diminui) == 1)
{delay(100);}

u8g.firstPage();
// Chama a rotina de desenho na tela
do
{
draw();
}
while ( u8g.nextPage() );
delay(50);
}
