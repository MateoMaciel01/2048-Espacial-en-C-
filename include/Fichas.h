#pragma once
#include <SFML/Graphics.hpp>
#include <map>


class Fichas {
private:
    std::map<int, sf::Texture> texturas;
    float anchoDeseado = 90.f;
    float altoDeseado = 90.f;


public:
    Fichas();
    void cargarFichas();
    sf::Sprite obtenerSprite(int valor);
    const std::map<int, sf::Texture>& getMapaTexturas() const;
    sf::Sprite obtenerSprite(int valor) const;
    void setTamanioDeseado(float ancho, float alto);

    //HIELO
    sf::Texture fichaHielo;
    void cargarFichaHielo();
    const sf::Texture& getFichaHielo() const;



};
