#ifndef CLASS_LCD
#define CLASS_LCD

#include <Arduino.h>

#include <Pines&Const.h>

#define NIV_SUBMENU_01 3

class LCD4BITS
{
public:
    //////////////  NAVEG_SUBMENU ///////////////////////////////////
    int tamano_submenu[1] = {NIV_SUBMENU_01};
    String __mensajes_submenu_01[NIV_SUBMENU_01] = {"HABITACION", "VILLA", "CONEX LETRERO"};
    int opcion_ciclos_submenu_x;
    int f1_recorrido_fila, indice_recorrido_fila;
    String recorrido_fila_encabezado, recorrido_fila_contenido;
    /////////////////////////////////////////////////////////////////

    //////////////  InsertarNums ///////////////////////////////////
    int TamTecIntroducidas;
    char TeclasIntroducidas[6];
    String NumeroIntroducido;
    /////////////////////////////////////////////////////////////////

    //////////////  InsertarNums ////////////////////////////////////

    int GetNumeros(void);
    /////////////////////////////////////////////////////////////////

    void Init(void);

    //////////////  MENSAJEPRED   ///////////////////////////////////
    void MsgGral(int clear, int mensaje);
    void MsgEscritura(int opcion, String msg);
    void MsgDatosEnviados(int cnf_servicio, int direccion);
    /////////////////////////////////////////////////////////////////

    //////////////  NAVEG_SUBMENU ///////////////////////////////////
    int NavegSubmenu(String datos);
    /////////////////////////////////////////////////////////////////

private:
    //////////////  NAVEG_SUBMENU ///////////////////////////////////
    int __datos_totales;
    String __datoIndividual[10];

    void __print_linea_submenu_x(int clear_lcd, int flecha, int eje_y, int indice);
    void __print_flecha_submenu(int eje_y);
    String __get_msg_fila_submenu(int indice);
    void __espera_tecla(void);
    int __salir_menu_naveg(int time);
    int __get_datos_str(String datos);
    /////////////////////////////////////////////////////////////////

    //////////////  InsertarNums ///////////////////////////////////
    void __valor_clave_parpadeo(int lim_ciclos);
    int __borrar_caracteres(void);
    void __salir_menu(void);
    /////////////////////////////////////////////////////////////////
};

#endif