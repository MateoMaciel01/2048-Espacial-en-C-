#ifndef ESTADOS_HPP
#define ESTADOS_HPP
#include <iostream>

enum EstadoMenu {
    NINGUNO,
    EN_MENU,
    NUEVA_PARTIDA,
    CONTINUAR_PARTIDA,
    EN_JUEGO,
    DESHACER,
    PERDIDO,
    NIVEL_COMPLETO,
    SIGUIENTE_NIVEL,
    VICTORIA,
    PAUSA,
    REINTENTAR,
    GUARDAR_PARTIDA,
    SUBMENU_PUNTOS,
    EN_SUBMENU_NOMBRE,
    VER_REGLAS,
    VER_PUNTAJES,
    SALIR
};

#endif
