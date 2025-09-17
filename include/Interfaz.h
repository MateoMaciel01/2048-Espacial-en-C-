#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Estados.h"
#include "Tablero.h"
#include "Juego.h"
#include "Opciones.h"


class Interfaz {
private:


    sf::RenderWindow& ventana;
    Menu& menu;
    Juego& juego;
    Tablero& tablero;
    Fichas& fichas;
    Opciones& opciones;
    FichaEspeciales& fichaEspeciales;
    Efectos& efectoPortal;
    bool& saltarChequeoNivel;
    float& deltaTime;

public:
    Interfaz(
        sf::RenderWindow& ventana_,
        Menu& menu_,
        Juego& juego_,
        Tablero& tablero_,
        Fichas& fichas_,
        Opciones& opciones_,
        FichaEspeciales& fichaEspeciales_,
        Efectos& efectoPortal_,
        bool& saltarChequeoNivel_,
        float& deltaTime_);

    EstadoMenu procesarEventos(EstadoMenu estado);
    EstadoMenu manejarEntradaJuego(sf::Event evento);

};

#endif
