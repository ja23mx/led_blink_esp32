#include <LCD_4BITS.h>
#include <VAR_LCD_EXT.h>

/*
    @param
    clear 0=no limpia lcd, 1=limpia lcd
    @param
    mensaje 1 hasta núm total de mensajes
*/
void LCD4BITS::MsgGral(int clear, int mensaje)
{
    int i, x_matriz, y_matriz, limite_x;

    int eje_y[3][5] = {{0, 1, 0, 1, 0},
                       {0, 1, 0, 1, 1},
                       {1, 0, 0, 0, 0}};
    //                          1                2                  3              4               5
    String msg[3][5] = {{"  CONECTANDO A", "   RED LOCAL", "    CONEXION", "    EXITOSA", "    IP LOCAL"},
                        //       6               7               8              9                10
                        {"    BUSCANDO", "   LETRERO...", "    LETRERO", "   ENCONTRADO", " NO ENCONTRADO"},
                        //      11          12                13             14
                        {"    CONEKTA", "HABITACION:", "     SALIR?", "    GUARDAR?", ""}};
    String cx;

    if (mensaje < 6)
    {
        x_matriz = 0;
        y_matriz = mensaje - 1;
    }
    else
    {
        limite_x = 11;
        for (i = 0; i < 5; i++)
        {
            if (mensaje < limite_x)
            {
                x_matriz = i + 1;
                y_matriz = (mensaje - (5 * (i + 1))) - 1;
                break;
            }
            limite_x += 5;
        }
    }

    // Serial.print("\r\n x_matriz: " + String(x_matriz) + ". y_matriz: " + String(y_matriz));

    cx = msg[x_matriz][y_matriz];

    if (clear)
        lcd.clear();

    lcd.setCursor(0, eje_y[x_matriz][y_matriz]);
    lcd.print(cx);
}

void LCD4BITS::MsgEscritura(int opcion, String msg)
{
    int eje_x[1] = {0};
    int eje_y[1] = {1};

    String cx;

    switch (opcion)
    {
    case 0:                                // escritura de usuario agregado
        lcd.setCursor(eje_x[0], eje_y[0]); //
        lcd.print(msg);
        break;
    }
}

/*
    @param cnf_servcio 1=habitacion, 2=villa
    @param direccion 1=derecha, 2=izquierda
*/
void LCD4BITS::MsgDatosEnviados(int cnf_servicio, int direccion)
{
    int num_x;

    num_x = NumeroIntroducido.toInt();

    Serial.print("\r\n\r\ncnf_servicio: " + String(cnf_servicio) + "\r\ndireccion:" + String(direccion) + "\r\nnum_x: " + String(num_x));

    lcd.clear();

    lcd.setCursor(0, 0);
    direccion == 1 ? lcd.print(">>") : lcd.print("<<");
    lcd.setCursor(14, 0);
    direccion == 1 ? lcd.print(">>") : lcd.print("<<");

    lcd.setCursor(4, 0);
    cnf_servicio == 1 ? lcd.print("HABIT") : lcd.print("VILLA");

    lcd.setCursor(10, 0);
    if (num_x < 10)
        lcd.print("00");
    else if (num_x < 100)
        lcd.print("0");
    lcd.print(NumeroIntroducido);

    lcd.setCursor(2, 1);
    lcd.print("ENVIANDO...");

    while (1)
    {
        delay(1);
    }
}