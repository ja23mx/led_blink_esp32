#include <Arduino.h>

#include <Pines&Const.h>
#include <VAR_LCD_CONF.h>
#include <LCD_4BITS.h>

void LCD4BITS::Init(void)
{

    lcd.begin(16, 2); 
    lcd.setCursor(0, 0);
    lcd.print(FIRMWARE);
    lcd.setCursor(0, 1);
    lcd.print(VERSION);
    delay(TT_ESP_MSG_X_LCD);
    lcd.clear();
}