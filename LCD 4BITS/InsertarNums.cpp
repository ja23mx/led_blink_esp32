
#include <VAR_LCD_EXT.h>
#include <LCD_4BITS.h>
#include <VAR_EXT.h>

#define INIT_CARACTERES 12

int cnt_eje_x, teclas_correctas, teclas_final, lim_ciclos, salir_f1;

int LCD4BITS::GetNumeros(void)
{
    int i, rsp;
    char val_parpadeo[2] = {'_', ' '};

    MsgGral(1, 12);                  // CLEAR=1, MSG-> 0,0 "HABITACION:"
    lim_ciclos = 4;                  // limite de dígitos
                                     //
    for (i = 0; i < 6; i++)          //
        TeclasIntroducidas[i] = 255; //

    TamTecIntroducidas = lim_ciclos;
    lim_ciclos--;

    cnt_eje_x = 0;
    teclas_correctas = 0;
    teclas_final = 0;
    salir_f1 = 0;

    __espera_tecla();

    while (1)
    {
        while (teclas_final == 0)
        {
            for (i = 0; i < 2; i++)
            {
                if (cnt_eje_x <= lim_ciclos)
                {
                    lcd.setCursor(INIT_CARACTERES + cnt_eje_x, 0);
                    lcd.print(val_parpadeo[i]);
                }
                __valor_clave_parpadeo(lim_ciclos);
                if (teclas_final == 1 || salir_f1 == 1)
                    break;
            }
            if (salir_f1 == 1)
                break;
        }
        if (tecla_presionada == TECLA_MENOS)
            __borrar_caracteres();
        else if (tecla_presionada == TECLA_MENU)
            __salir_menu();
        if (salir_f1 == 1 || tecla_presionada == TECLA_ESC)
            break;
        delay(1);
    }

    if (teclas_final == 1) // tecla OK y todas los digitos son introducidos
    {
        rsp = 1;
        NumeroIntroducido = "";
        for (i = 0; i < lim_ciclos + 1; i++)
        {
            if (TeclasIntroducidas[i] != 255)
                NumeroIntroducido += String(TeclasIntroducidas[i]);
        }

        //Serial.print("\r\n GetNumero: " + NumeroIntroducido);
    }
    else
    {
        NumeroIntroducido = "";
        rsp = 0;
    }

    lcd.clear();
    Serial.print("\r\n\r\nFIN CICLO: " + NumeroIntroducido);
    return rsp;
}

void LCD4BITS::__valor_clave_parpadeo(int lim_ciclos) // lim_ciclos es el total de digitos a introducir
{
    int i;

    for (i = 0; i < 500; i++)
    {
        if (tecla_presionada >= '1' && tecla_presionada <= '9')
        {
            if (teclas_final == 0)
            {
                TeclasIntroducidas[cnt_eje_x] = tecla_presionada;
                teclas_correctas++;
                lcd.setCursor(INIT_CARACTERES + cnt_eje_x, 0);
                lcd.print(tecla_presionada);
                if (cnt_eje_x < lim_ciclos)
                    cnt_eje_x++;
                else if (cnt_eje_x == lim_ciclos)
                    teclas_final = 1;
                __espera_tecla();
                break;
            }
            else                  // tecla no fue correcta
                __espera_tecla(); // misma que anterior, es tecla menos o
        }
        else if (tecla_presionada == TECLA_MENOS)
        {
            __borrar_caracteres();
            break;
        }
        else if (tecla_presionada == TECLA_MENU)
        {
            __salir_menu();
            break;
        }
        else if (tecla_presionada == TECLA_ESC)
        {
            salir_f1 = 1;
            break;
        }
        delay(1);
    }
}

int LCD4BITS::__borrar_caracteres(void)
{
    if (tecla_presionada != TECLA_MENOS)
        return false;

    if (teclas_correctas == 0)
        return false;

    if (teclas_final == 0)                                    // teclas no han finalizado
    {                                                         //
        lcd.setCursor(INIT_CARACTERES + teclas_correctas, 0); //
        lcd.print(" ");                                       //
        cnt_eje_x--;                                          // indice de posición
    }
    else                                                          // teclas totales introducidas
    {                                                             //
        lcd.setCursor(INIT_CARACTERES + teclas_correctas - 1, 0); //
        lcd.print(" ");
    }

    TeclasIntroducidas[cnt_eje_x] = 255;

    if (teclas_final == 1) // fue finalizado los numeros datos
        teclas_final = 0;  //
    teclas_correctas--;

    Serial.print("\r\n TeclasIntroducidas. ");
    for (int i = 0; i < 6; i++)
    {
        if (TeclasIntroducidas[i] != 255)
            Serial.print(String(TeclasIntroducidas[i]) + ",");
    }

    Serial.print("\r\n teclas_correctas: " + String(teclas_correctas) + ". cnt_eje_x: " + String(cnt_eje_x));

    __espera_tecla();
    return true;
}

void LCD4BITS::__salir_menu(void)
{
    int i, init_vacio, print_val_tot;
    unsigned long ti_x;

    MsgGral(1, 14); // CLEAR=1, MSG-> 0,0 "GUARDAR?"

    __espera_tecla();
    ti_x = millis();

    salir_f1 = 0;
    while (1)
    {
        if (tecla_presionada == TECLA_OK)
        {
            teclas_final = 1;
            salir_f1 = 1;
            break;
        }
        else if (tecla_presionada == TECLA_ESC)
            break;
        if ((unsigned long)millis() - ti_x > TT_ESPERA_SALIR)
            break;
        delay(5);
    }

    if (!salir_f1)
    {
        MsgGral(1, 12);            // CLEAR=1, MSG-> 0,0 "HABITACION:"
        if (teclas_correctas == 0) //
            init_vacio = 0;
        else
        {
            teclas_final == 0 ? print_val_tot = teclas_correctas + 1 : print_val_tot = teclas_correctas; //
            for (i = 0; i < print_val_tot; i++)                                                          // print de linea de caract vacios
            {
                lcd.setCursor(INIT_CARACTERES + i, 0);
                lcd.print(TeclasIntroducidas[i]);
            }
            init_vacio = teclas_correctas + 1;
        }
        if (teclas_final == 0)                            // imprime para espacios vacios si no has sido
        {                                                 // terminado imprime
            for (i = init_vacio; i < lim_ciclos + 1; i++) // print de linea de caract vacios
            {
                lcd.setCursor(INIT_CARACTERES + i, 0);
                lcd.print(" ");
            }
        }
    }
}