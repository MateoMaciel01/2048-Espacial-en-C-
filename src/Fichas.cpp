#include "Fichas.h"
#include <iostream>
#include <fstream>


Fichas::Fichas() {
    cargarFichas();
}

void Fichas::cargarFichas() {
    sf::Texture textura;
    sf::Texture fichaHielo;

    if (!texturas[2].loadFromFile("resourses/2.png"))
        std::cerr << "Error cargando ficha 2\n";

    if (!texturas[4].loadFromFile("resourses/4.png"))
        std::cerr << "Error cargando ficha 4\n";

    if (!texturas[8].loadFromFile("resourses/8.png"))
        std::cerr << "Error cargando ficha 8\n";

    if (!texturas[16].loadFromFile("resourses/16.png"))
        std::cerr << "Error cargando ficha 16\n";

    if (!texturas[32].loadFromFile("resourses/32.png"))
        std::cerr << "Error cargando ficha 32\n";

    if (!texturas[64].loadFromFile("resourses/64.png"))
        std::cerr << "Error cargando ficha 64\n";

    if (!texturas[128].loadFromFile("resourses/128.png"))
        std::cerr << "Error cargando ficha 128\n";

    if (!texturas[256].loadFromFile("resourses/256.png"))
        std::cerr << "Error cargando ficha 256\n";

    if (!texturas[512].loadFromFile("resourses/512.png"))
        std::cerr << "Error cargando ficha 512\n";

    if (!texturas[1024].loadFromFile("resourses/1024.png"))
        std::cerr << "Error cargando ficha 1024\n";

    if (!texturas[2048].loadFromFile("resourses/2048.png"))
        std::cerr << "Error cargando ficha 2048\n";




}
const std::map<int, sf::Texture>& Fichas::getMapaTexturas() const {
    return texturas;
}
void Fichas::setTamanioDeseado(float ancho, float alto) {
    anchoDeseado = ancho;
    altoDeseado = alto;
}
sf::Sprite Fichas::obtenerSprite(int valor) const {
    sf::Sprite sprite;
    auto it = texturas.find(valor);
    if (it != texturas.end()) {
        sprite.setTexture(it->second);

        sf::Vector2u sizeOriginal = it->second.getSize();
        float escalaX = anchoDeseado / sizeOriginal.x;
        float escalaY = altoDeseado / sizeOriginal.y;

        sprite.setScale(escalaX, escalaY);
    }
    return sprite;


}
//HIELO
void Fichas::cargarFichaHielo() {
    if (!fichaHielo.loadFromFile("resourses/hieloFicha.png")) {
    } else {

    }

}
const sf::Texture& Fichas::getFichaHielo() const {
    return fichaHielo;
}
