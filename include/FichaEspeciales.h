#ifndef FICHAESPECIALES_H
#define FICHAESPECIALES_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#pragma once
#include "Efectos.h"
struct Explosion {
    sf::Vector2f posicion;
    float tiempoTranscurrido = 0.f;
    bool activa = true;
};
class FichaEspeciales
{
    public:
        FichaEspeciales();
        //BOMBA
        void dibujarBomba(sf::RenderWindow& window, float deltaTime, sf::Vector2f posicion);

        void dibujarExplosion(sf::RenderWindow& ventana, float deltaTime);
        void actualizarAnimacionExplosion(float deltaTime);
        void activarExplosion(const sf::Vector2f& posicion);
        sf::Sprite& getSpriteBomba();
        static const int ANCHO_FRAME = 125.f;
        static const int ALTO_FRAME = 149.f;

        const sf::Texture& getBombaTexture() const;

        void activarExplosionEn(sf::Vector2f posicion);

        bool hayExplosionActiva() const;

        std::vector<Explosion> explosionesActivas;
        bool explosionFinalizada() const;

        void limpiarExplosiones();

        //RETROCEDER
        sf::Sprite& getBotonDeshacer();
        void setMostrarBotonDeshacer(bool mostrar);
        void dibujarBotonDeshacer(sf::RenderWindow& ventana);
        sf::Sprite spriteRetroceder;
        bool fueClickeadoBotonDeshacer(sf::Vector2i mousePos);
        bool mostrarBotonDeshacer = false;

        //COHETE

        sf::Sprite spriteBotonCohete;
        bool arrastrandoFichaCohete = false;
        sf::Vector2f offsetArrastre;
        bool fichaDisponible = true;
        sf::Vector2f posicionBotonCohete;
        int const maxMovimientos = 5;

        sf::Sprite& getBotonCohete();

        void setMostrarBotonCohete(bool mostrar);
        void dibujarBotonCohete(sf::RenderWindow& ventana,int nivelActual);
        bool fueClickeadoBotonCohete(sf::Vector2i mousePos);
        void cargarSpriteCohete();
        void dibujarBarraCohete(sf::RenderWindow& ventana, int nivelActual);
        void registrarMovimiento();
        bool coheteDisponible() const;
        void usarCohete();
        bool coheteListo = false;
        sf::Sprite spriteBarraCohete;
        sf::Sprite getSpriteCoheteEnUso(int frame) const;
        int getContadorMovimientos() const;
        int contadorMovimientos = 0;
        void reiniciarContadorMovimientos();
        bool mostrarBotonCohete = false;
        int anchoFragmento = 1036;
        int altoFragmento = 153;

        //MISIL

        void activarCohete();
        void seleccionarFicha(sf::Vector2i celda, sf::Vector2f posPixel);
        void actualizarCohete(float dt);
        void dibujarCohete(sf::RenderWindow& window);
        bool coheteListoParaEliminar() const;
        sf::Vector2i obtenerObjetivo() const;

        //HIELO PANTALLA
        bool mostrarPantallaHielo = false;
        sf::Clock relojHielo;
        sf::Sprite spritePantallaHielo;
        sf::Texture texturaPantallaHielo;
        void activarPantallaHielo();
        void dibujarPantallaHielo(sf::RenderWindow& ventana);

        //MEZCLADOR
        sf::Texture texturaBarraMezcla;
        sf::Sprite spriteBarraMezcla;

        int frameCarga = 0;
        int totalFramesCarga = 31;

        int anchoFrameCarga = 241;
        int altoFrameCarga = 344;

        sf::Texture texturaBotonMezcla;
        sf::Sprite spriteBotonMezcla;
        bool mostrarBotonMezcla = false;

        void registrarMovimientoCarga();
        void dibujarCarga(sf::RenderWindow& ventana, int nivelActual);
        bool fueClickeadoBotonMezcla(sf::Vector2i mousePos);
        void usarMezcla();


    private:
        sf::Texture texturaBomba;
        sf::Sprite spriteBomba;

        sf::Vector2f posicionExplosion;

        bool animacionActiva;
        bool animacionTerminada;


        float tiempoPorFrame = 0.15f;
        float deltaTime;

        bool animando = false;
        float tiempoAcumulado = 0.f;
        int frameActual = 0;
        float duracionFrame = 0.09f;
        int totalFrames = 10;
        float tamanioCelda = 64.f;

        //RETROCEDER
        sf::Texture texturaRetroceder;

        //COHETE
        sf::Texture texturaBotonCohete;
        sf::Texture texturaBarraCohete;

        int anchoFragmentoCohete;
        int altoFragmentoCohete;

        //MISIL
        Efectos efectos;
        bool coheteActivo = false;
        sf::Vector2i fichaObjetivo;



};

#endif // FICHAESPECIALES_H
