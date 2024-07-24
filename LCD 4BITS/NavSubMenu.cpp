
#include <LCD_4BITS.h>

#include <Pines&Const.h>
#include <VAR_EXT.h>
#include <VAR_LCD_EXT.h>

int LCD4BITS::NavegSubmenu(String datos) // int opcion
{
    int i, contador_eventos, eje_y, eje_x_init_recorrido, print_flecha, print_x, f1, f2, eventos, opcion_escogida, indice, msg_tamano, limite;
    int limite_tamano_recorrido;
    String msg, linea1, cx;

    // opcion--;
    // opcion_ciclos_submenu_x = opcion;opcion: " + String(opcion_ciclos_submenu_x) + ".
    // eventos = tamano_submenu[opcion];

    if (!__get_datos_str(datos))
        return false;

    eventos = __datos_totales;

    Serial.print("\r\n\r\n eventos: " + String(eventos));

    opcion_escogida = 0;                                        //
    contador_eventos = 0;                                       //
    eje_y = 0;                                                  //
    print_flecha = 0;                                           //
    print_x = 0;                                                // autoriza actualizar la lcd
    __print_linea_submenu_x(1, 1, 0, contador_eventos);         // clear=1, flecha=1, eje_y=0, indice info=0
                                                                //
    if (eventos > 1)                                            //
        __print_linea_submenu_x(2, 0, 1, contador_eventos + 1); // clear=2, flecha=0, eje_y=1, indice info=1

    __espera_tecla();

    // Serial.print("\r\n\r\n opcion_escogida: " + String(opcion_escogida) + ". eje_y: " + String(eje_y));

    f1 = 0;
    while (!f1)
    {

        msg = __get_msg_fila_submenu(opcion_escogida);
        msg_tamano = msg.length();

        if (msg_tamano >= 16)
        {
            if (f1_recorrido_fila)
            {
                cx = recorrido_fila_encabezado + recorrido_fila_contenido;
                msg = recorrido_fila_contenido;
                msg_tamano = recorrido_fila_contenido.length();      //
                eje_x_init_recorrido = indice_recorrido_fila + 2;    // init de contenido en eje x
                limite_tamano_recorrido = 16 - eje_x_init_recorrido; //
                limite = msg_tamano - limite_tamano_recorrido + 1;   //
                lcd.setCursor(1, eje_y);
                lcd.print(recorrido_fila_encabezado);
            }
            else
            {
                cx = msg;
                limite_tamano_recorrido = 15;
                eje_x_init_recorrido = 1;
                limite = (msg_tamano - 15) + 1;
            }

            indice = 0;

            f2 = 0;
            while (!f2)
            {
                linea1 = "";
                for (i = 0; i < limite_tamano_recorrido; i++)
                {
                    if (msg.charAt(i + indice) >= ' ' && msg.charAt(i + indice) <= '~') // solo imprime caractes validos
                        linea1 += msg.charAt(i + indice);                               //
                } //
                lcd.setCursor(eje_x_init_recorrido, eje_y); //
                lcd.print(linea1);                          // info recorrida
                f2 = __salir_menu_naveg(800);               //
                indice++;                                   // contador de recorrido
                if (indice == limite)                       // rst de contador de recorrido
                    indice = 0;

                if (f2 == 0)
                {
                    lcd.setCursor(eje_x_init_recorrido, eje_y);
                    for (i = 0; i < 16; i++)
                        lcd.print(" ");
                    f2 = __salir_menu_naveg(500);
                }
            }

            linea1 = cx.substring(0, 15);
            lcd.setCursor(1, eje_y);
            lcd.print(linea1);
        }

        if (tecla_presionada > 0)
        {
            Serial.print("\r\n\r\nTECLA 23: " + String(tecla_presionada));
            if (tecla_presionada == TECLA_OK || tecla_presionada == TECLA_ESC)
                f1 = 1;
            switch (tecla_presionada)
            {
            case TECLA_MAS:                                                                // tecla ARRIBA
                if (eventos > 1)                                                           //
                {                                                                          //
                    print_flecha = 1;                                                      //
                    if (contador_eventos == 0 && eje_y == 0)                               // está hasta arriba, reinicia desde abajo
                    {                                                                      //
                        eventos > 2 ? opcion_escogida = eventos - 1 : opcion_escogida = 1; //
                        contador_eventos = eventos - 2;                                    //
                        eje_y = 1;                                                         //
                        print_x = 1;                                                       // autoriza actualizar la lcd
                    } //
                    else if (eje_y == 0)                                       // decrementa índice
                    {                                                          //
                        eventos > 2 ? opcion_escogida-- : opcion_escogida = 1; //
                        contador_eventos--;                                    //
                        print_x = 1;                                           // autoriza actualizar la lcd
                    } //
                    else if (eje_y == 1)                                       // sólo mueve flecha hacia arriba
                    {                                                          //
                        eventos > 2 ? opcion_escogida-- : opcion_escogida = 0; //
                        eje_y = 0;
                    }
                    // Serial.print("\r\n\r\n opcion_escogida: " + String(opcion_escogida) + ". eje_y: " + String(eje_y));
                }
                break;
            case TECLA_MENOS:                                                  // tecla ABAJO
                if (eventos > 1)                                               //
                {                                                              //
                    print_flecha = 1;                                          //
                    if (eje_y == 0)                                            // sólo mueve flecha hacia abajo
                    {                                                          //
                        eventos > 2 ? opcion_escogida++ : opcion_escogida = 1; //
                        eje_y = 1;                                             //
                    } //
                    else if (eje_y == 1)                     // incrementa índice
                    {                                        //
                        opcion_escogida++;                   //
                        contador_eventos++;                  //
                        if (contador_eventos == eventos - 1) // llegó hasta abajo reinicia en índice 0
                        {                                    //
                            opcion_escogida = 0;             //
                            contador_eventos = 0;            //
                            eje_y = 0;                       //
                        } //
                        print_x = 1; // autoriza actualizar la lcd
                    } //
                    // Serial.print("\r\n\r\n opcion_escogida: " + String(opcion_escogida) + ". eje_y: " + String(eje_y));
                }
                break;             //
            case TECLA_OK:         // tecla OK
                opcion_escogida++; // Serial.print("\r\n\r\n opcion_escogida: " + String(opcion_escogida));
                break;             //
            case TECLA_ESC:        // tecla ESCAPE
                opcion_escogida = 0;
                break;
            }
            __espera_tecla();
        }
        if (!f1)                                                        // sigue en ciclo
        {                                                               //
            if (print_x)                                                // actualización de info en pantalla
            {                                                           //
                print_x = 0;                                            // rst flag
                __print_linea_submenu_x(1, 0, 0, contador_eventos);     // clear=1, flecha=0, eje_y=0, indice info=contador_eventos
                __print_linea_submenu_x(2, 0, 1, contador_eventos + 1); // clear=2, flecha=0, eje_y=1, indice info=contador_eventos+1
            } //
            if (print_flecha == 1)             // actualiza posición de la flecha
            {                                  //
                print_flecha = 0;              // rst flag
                __print_flecha_submenu(eje_y); // print la flecha indicada y borra la anterior
            }
        }
        delay(1);
    }

    Serial.print("\r\n\r\n opcion_escogida: " + String(opcion_escogida));
    Serial.print("\r\n FIN SUBMENU: ");
    return opcion_escogida;
}

void LCD4BITS::__print_linea_submenu_x(int clear_lcd, int flecha, int eje_y, int indice)
{
    String cx;

    if (clear_lcd == 1) // limpieza de LCD
    {                   //
        delay(200);     //
        lcd.clear();    //
    } //
    else if (clear_lcd == 2)           // limpieza sólo de línea de info de evento
    {                                  //
        lcd.setCursor(0, eje_y);       //
        lcd.print("                "); //
    }

    cx = __get_msg_fila_submenu(indice); // texto de acuerdo a indice
    if (cx.length() > 15)
        cx = cx.substring(0, 15);

    if (flecha) // print flecha
    {
        lcd.setCursor(0, eje_y);
        lcd.print((char)0x3E);
    }

    lcd.setCursor(1, eje_y); // print info evento
    lcd.print(cx);
}

void LCD4BITS::__print_flecha_submenu(int eje_y) //
{                                                //
    lcd.setCursor(0, !eje_y);                    // borra la flecha anterior
    lcd.print(" ");                              //
    delay(200);                                  //
    lcd.setCursor(0, eje_y);                     //
    lcd.print((char)0x3E);                       // print la flecha >
}

String LCD4BITS::__get_msg_fila_submenu(int indice)
{
    int i, tamano;
    String msg;

    /* switch (opcion_ciclos_submenu_x)
    {
    case 0:                             // submenú principal */
    msg = __datoIndividual[indice]; // __mensajes_submenu_01[indice];
    /* break;                          //
} */

    f1_recorrido_fila = 0;

    tamano = msg.length();

    for (i = 0; i < tamano; i++)
    {
        if (msg.charAt(i) == 0x27)     // comilla simple '
        {                              //
            indice_recorrido_fila = i; //
            f1_recorrido_fila = 1;     // hubo una comilla
            break;
        }
    }

    if (f1_recorrido_fila)
    {
        recorrido_fila_encabezado = "";
        for (i = 0; i < indice_recorrido_fila; i++)
            recorrido_fila_encabezado += msg.charAt(i);
        recorrido_fila_encabezado += " ";
        recorrido_fila_contenido = "";
        for (i = i + 1; i < tamano; i++)
            recorrido_fila_contenido += msg.charAt(i);

        Serial.print("\r\n\r\n\r\n encabezado: \r\n" + recorrido_fila_encabezado);
        Serial.print("\r\n contenido: \r\n" + recorrido_fila_contenido);
        msg = recorrido_fila_encabezado + recorrido_fila_contenido;
    }

    return msg;
}

void LCD4BITS::__espera_tecla(void)
{
    // tecla_presionada = 0;
    while (tecla_presionada > 0)
        delay(50);
    delay(100);
}

int LCD4BITS::__salir_menu_naveg(int time)
{
    int i, rsp;

    rsp = 0;

    for (i = 0; i < time; i++)
    {
        if (tecla_presionada > 0)
        {
            if (tecla_presionada == TECLA_OK || tecla_presionada == TECLA_ESC || tecla_presionada == TECLA_MAS || tecla_presionada == TECLA_MENOS)
            {
                rsp = 1;
                break;
            }
        }
        delay(1);
    }

    return rsp;
}

int LCD4BITS::__get_datos_str(String datos)
{
    int i, j, indice, num_datos, rsp_fn;
    String cx;

    indice = 0;
    __datos_totales = 0;
    datos += String('>');

    Serial.print("\r\n\r\n");

    for (i = 0; i < 10; i++)
        __datoIndividual[i] = "";

    for (i = 0; i < 10; i++)
    {
        cx = "";
        for (j = 0; j < 30; j++)
        {
            if (datos.charAt(indice) != '>')
            {
                if (datos.charAt(indice) != ',' && indice <= datos.length()) //
                    cx += datos.charAt(indice);
                else
                    break;
            }
            else
                break;
            indice++;
        }
        indice++;
        if (cx != "")
        {
            Serial.print("\r\ncx: " + cx);
            __datoIndividual[i] = cx;
            __datos_totales++;
        }
    }

    Serial.print("\r\ndatos_totales: " + String(__datos_totales));

    if (__datos_totales == 0)
        rsp_fn = 0;
    else if (__datos_totales >= 0)
        rsp_fn = 1;

    return rsp_fn;
}