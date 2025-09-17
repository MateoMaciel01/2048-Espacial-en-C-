#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#pragma once
#include "Estados.h"
#include "Fichas.h"
#include "Juego.h"

class Tablero {
private:
    sf::Texture fondoTexture;
    sf::Sprite fondoSprite;

    sf::Texture tableroTexture;
    sf::Sprite tableroSprite;

    sf::Texture tableroTextureNivel2;


    sf::Texture tableroTextureNivel3;


    std::vector<sf::Sprite> fichasGraficas;




public:
    Tablero();
    Tablero(const sf::Vector2u& tamVentana);
    void dibujar(sf::RenderWindow& ventana);
    void actualizarDesdeLogica(const std::vector<std::vector<Celda>>& matriz, const std::map<int, sf::Texture>& fichas,
    FichaEspeciales* fichaEspeciales);
    void configurarParaNivel(int nivel);
    void moverArriba();
    void moverAbajo();
    void moverIzquierda();
    void moverDerecha();

    sf::Vector2f origenTablero;
    float tamanioCelda;
    float margen;
    int filas;
    int columnas;

    void limpiarFichas();

    std::vector<sf::Sprite> bombasGraficas;



};
