#ifndef OPCIONES_H
#define OPCIONES_H
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>


struct Puntaje {
        std::string nombre;
        int puntos;
        float tiempo;

        bool operator<(const Puntaje& otro) const {
        return puntos < otro.puntos;
}
};
class Opciones
{
    private:
        sf::Texture textura;
        sf::Sprite sprite;
        sf::Text texto;
        sf::Font fuente;
        std::vector<Opciones> botones;

        sf::Text textoPuntaje;
        sf::Text textoPuntajeSubmenu;

        sf::Text textoTiempo;
        sf::Text textoTiempoSubmenu;
        sf::Font fuenteT;

        //PUNTAJES
        std::vector<Puntaje> top5Puntajes;
        std::string archivoPuntajes = "top5puntajes.txt";

        std::vector<sf::Text> textosTop5;



    public:
        Opciones(sf::Vector2f posicion, sf::Vector2f tamano, const std::string& texto, const std::string& rutaImagen = "");
        void dibujar(sf::RenderWindow& ventana);
        void dibujarSubmenu(sf::RenderWindow& ventana);
        bool fueClickeado(sf::Vector2f mousePos);

        void actualizarPuntaje(int nuevoPuntaje);

        void configurarTiempoTexto();
        void actualizarTiempo(float tiempoSegundos);

        //PUNTAJES

        void cargarTop5();
        void guardarTop5();
        void insertarNuevoPuntaje(const Puntaje& nuevo);
        void dibujarTop5(sf::RenderWindow& ventana);
        void cargarPuntajes();
        void dibujarSubmenuPuntajes(sf::RenderWindow& ventana);
         sf::Sprite* puntosSpritePtr = nullptr;
         void setPuntosSprite(sf::Sprite* sprite) {
        puntosSpritePtr = sprite;
    }

    void actualizarTextosTop5();




};

#endif
