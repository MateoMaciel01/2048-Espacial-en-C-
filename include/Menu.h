#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Opciones.h"
#include "Estados.h"
#include "FichaEspeciales.h"

class Menu {
private:
    sf::Texture fondoTexture;
    sf::Sprite fondoSprite;

    sf::Texture subfondoTexture;
    sf::Sprite subfondoSprite;

    sf::Font fuente;
    sf::Text textoNombre;

    sf::String nombreIngresado;
    bool ingresandoNombre = false;
    bool nombreConfirmado = false;

    sf::Texture reglasTexture;
    sf::Sprite reglasSprite;

    sf::Texture puntosTexture;
    sf::Sprite puntosSprite;

    Opciones botonVolver;
    bool mostrandoReglas = false;

    sf::Texture fondoPausaTexture;
    sf::Sprite fondoPausaSprite;

    sf::Texture victoriaTexture;
    sf::Sprite victoriaSprite;

    sf::Texture fondoPerdisteTexture;
    sf::Sprite fondoPerdisteSprite;

    sf::Texture fondoGanasteTexture;
    sf::Sprite fondoGanasteSprite;

    Opciones botonNueva;
    Opciones botonContinuar;
    Opciones botonIniciarJuego;
    Opciones botonPuntajes;
    Opciones botonReglas;
    Opciones botonVolverReglas;
    Opciones botonSalir;
    Opciones botonContinuarPausa;
    Opciones botonGuardar;
    Opciones botonVolverMenu;
    Opciones botonMenuVictoria;
    Opciones botonMenuPerdiste;
    Opciones botonReintentar;
    Opciones botonGuardarPartida;
    Opciones botonSiguienteNivel;


    EstadoMenu estadoActual = EN_MENU;
    FichaEspeciales* fichaEspeciales = nullptr;



public:
    Menu();
    Menu(const sf::Vector2u& tamVentana);
    void dibujar(sf::RenderWindow& ventana);
    void dibujarPausa(sf::RenderWindow& ventana);
    EstadoMenu manejarClick(sf::Vector2f mousePos);
    EstadoMenu getEstadoActual() const;

    void manejarEntradaNombre(sf::Event evento);

    std::string obtenerNombreJugador() const { return nombreIngresado; }
    void iniciarIngresoNombre();
    bool estaIngresandoNombre() const;
    bool nombreFueConfirmado() const;
    void setEstado(EstadoMenu nuevoEstado);

    void setFichaEspeciales(FichaEspeciales* fichasEspECIALES);



};

#endif
