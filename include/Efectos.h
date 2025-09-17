#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Fichas.h"
#include "Celda.h"

//#include "Juego.h"
class Juego;
class Tablero;
class Efectos {
    public:
        Efectos();


        void reiniciar();
        void actualizar(float dt);
        void dibujar(sf::RenderWindow& ventana,int nivelActual);

        void registrarMovimiento();
        bool estaListo() const;
        void activarEfecto(const std::vector<std::vector<Celda>>& tablero, const Fichas& fichas, sf::Vector2u tamVentana);

        void procesarClick(const sf::Vector2i& mousePos, sf::RenderWindow& ventana);
        bool estaAbsorbiendo() const;
        //BARRA
        int movimientos;
        sf::Sprite spriteBarra;
        const int anchoFrameBarra = 178;
        const int altoFrameBarra = 1738;



        bool fichasAbsorbidasCompletamente() const;

        bool fueClickeado(const sf::Vector2i& mousePos) const;
        bool manejarClick(const sf::Vector2i& mousePos, sf::RenderWindow& ventana, const std::vector<std::vector<Celda>>& tablero, const Fichas& fichas);
        void vaciarTablero(const std::vector<std::vector<Celda>>& tableroConst, Juego& juego);

        bool absorbiendo = false;
         bool absorcionTerminada = false;

        //MISILES COHETE
        const sf::Texture& getTexturaMisil() const;
        sf::Texture texturaMisil;

        sf::Sprite spriteMisil;
        void iniciarAnimacionMisil(sf::Vector2f origen, sf::Vector2f destino);
        void actualizarAnimacionMisil(float dt);
        void dibujarAnimacionMisil(sf::RenderWindow& window);
        bool animacionFinalizada() const;



    private:
        struct FichaAbsorbida {
            sf::Sprite sprite;
            sf::Vector2f posicionInicial;
            float tiempo;
        };

        // Barra

        const int maxMovimientos = 50;

        std::vector<std::unique_ptr<sf::Texture>> texturasBarra;
        sf::Texture texturaBarra;


        const int totalFramesBarra = 51;
        int frameBarra = 0;

        // Boton
        sf::Texture texturaBoton;
        sf::Sprite spriteBoton;
        bool botonVisible = false;

        // Portal
        sf::Texture texturaPortal;
        sf::Sprite spritePortal;
        float tiempoPortal = 0.f;
        float duracionFrame = 0.4f;
        int framePortal;
        float rotacionPortal = 0.f;
        bool portalActivo = false;

        // Absorción
        std::vector<FichaAbsorbida> fichasAbsorbiendose;
        sf::Vector2f centro;

        //MISILES COHETE

        sf::Vector2f posicionActual;
        sf::Vector2f destino;
        sf::Vector2f direccion;
        float velocidad;
        bool animacionActiva;

};
