#include "FichaEspeciales.h"
#include <vector>
#include <map>
#pragma once
#include <iostream>

FichaEspeciales::FichaEspeciales()
{
    mostrarPantallaHielo = false;


    texturaBomba.loadFromFile("resourses/especiales/spriteBomb.png");
    spriteBomba.setTexture(texturaBomba);

    spriteBomba.setScale(tamanioCelda / ANCHO_FRAME, tamanioCelda / ALTO_FRAME);
    frameActual = 0;
    tiempoPorFrame = 0.15f;
    tiempoAcumulado = 0.f;
    animacionActiva = false;
    animacionTerminada = false;
    spriteBomba.setTextureRect(sf::IntRect(0, 0, ANCHO_FRAME, ALTO_FRAME));

    //RETROCEDER

    texturaRetroceder.loadFromFile("resourses/especiales/retroceder.png");
    spriteRetroceder.setTexture(texturaRetroceder);
    spriteRetroceder.setPosition(850.f, 280.f);
    spriteRetroceder.setScale(0.2f, 0.2f);

    //COHETE

    if (!texturaBotonCohete.loadFromFile("resourses/especiales/cohete.png")) {
        std::cerr << "Error cargando textura boton cohete\n";
    }

    if (!texturaBarraCohete.loadFromFile("resourses/especiales/sprite-Cohete.png")) {
    std::cerr << "Error cargando textura barra cohete\n";
    }
    if (!texturaPantallaHielo.loadFromFile("resourses/hielo_fondo.png")) {
        std::cerr << "No se pudo cargar la imagen de congelamiento\n";
    }
    spritePantallaHielo.setTexture(texturaPantallaHielo);

    //MEZCLA

    if (!texturaBarraMezcla.loadFromFile("resourses/especiales/cargar_mezcla.png")) {
        std::cerr << "Error cargando sprite de carga\n";
    }

    anchoFrameCarga = texturaBarraMezcla.getSize().x / totalFramesCarga;
    altoFrameCarga = texturaBarraMezcla.getSize().y;

    spriteBarraMezcla.setTexture(texturaBarraMezcla);
    spriteBarraMezcla.setTextureRect(sf::IntRect(0, 0, anchoFrameCarga, altoFrameCarga));
    spriteBarraMezcla.setPosition(850.f, 550.f);
    spriteBarraMezcla.setScale(0.3f, 0.3f);

    if (!texturaBotonMezcla.loadFromFile("resourses/especiales/mezcla.png")) {
        std::cerr << "Error cargando botón mezcla\n";
    }
    spriteBotonMezcla.setTexture(texturaBotonMezcla);
    spriteBotonMezcla.setScale(0.3f, 0.3f);
    spriteBotonMezcla.setPosition(850.f, 550.f);




}

//BOMBA
void FichaEspeciales::dibujarBomba(sf::RenderWindow& ventana, float deltaTime, sf::Vector2f posicion) {
    if (animacionActiva && !animacionTerminada) {

        tiempoAcumulado += deltaTime;
        if (tiempoAcumulado >= tiempoPorFrame) {
            tiempoAcumulado = 0.0f;
            frameActual++;
            if (frameActual >= 5) {
                animacionTerminada = true;
                frameActual = 4;
            }
            spriteBomba.setTextureRect(sf::IntRect(frameActual * 64, 0, 64, 64));
        }
        spriteBomba.setPosition(0,0);
        ventana.draw(spriteBomba);
    }


}

void FichaEspeciales::activarExplosion(const sf::Vector2f& posicion) {
    animando = true;
    frameActual = 0;
    tiempoAcumulado = 0.f;
    posicionExplosion = posicion;

}


sf::Sprite& FichaEspeciales::getSpriteBomba() {
    return spriteBomba;
}
const sf::Texture& FichaEspeciales::getBombaTexture() const {
    return texturaBomba;
}

void FichaEspeciales::actualizarAnimacionExplosion(float deltaTime) {
     if (!animacionActiva) return;

    tiempoAcumulado += deltaTime;
    if (tiempoAcumulado >= tiempoPorFrame) {
        tiempoAcumulado = 0.f;
        frameActual++;
        if (frameActual >= totalFrames) {
            frameActual = 0;
            animacionActiva = false;
        }
        spriteBomba.setTextureRect(sf::IntRect(frameActual * ANCHO_FRAME, 0, ANCHO_FRAME, ALTO_FRAME));
    }
}

void FichaEspeciales::dibujarExplosion(sf::RenderWindow& ventana, float deltaTime) {
std::vector<Explosion> explosionesRestantes;
for (auto& exp : explosionesActivas) {
    exp.tiempoTranscurrido += deltaTime;

    int frameActual = static_cast<int>(exp.tiempoTranscurrido / tiempoPorFrame);

    if (frameActual < totalFrames) {


        sf::Sprite spriteLocal;
        spriteLocal.setTexture(texturaBomba);
        spriteLocal.setTextureRect(sf::IntRect(frameActual * ANCHO_FRAME, 0, ANCHO_FRAME, ALTO_FRAME));
        spriteLocal.setOrigin(ANCHO_FRAME / 2.f, ALTO_FRAME / 2.f);
        spriteLocal.setPosition(
            exp.posicion.x + tamanioCelda / 2.f,
            exp.posicion.y + tamanioCelda / 2.f
        );

        ventana.draw(spriteLocal);
        explosionesRestantes.push_back(exp);
    }
}
explosionesActivas = explosionesRestantes;
}

void FichaEspeciales::activarExplosionEn(sf::Vector2f posicion) {
    if (posicion == sf::Vector2f(0.f, 0.f)) {
       ;
        return;
    }

    Explosion nueva;
    nueva.posicion = posicion;
    explosionesActivas.push_back(nueva);
}

bool FichaEspeciales::explosionFinalizada() const {
     return explosionesActivas.empty();

}
void FichaEspeciales::limpiarExplosiones() {
    explosionesActivas.clear();


    animando = false;
    animacionActiva = false;
    animacionTerminada = false;
    frameActual = 0;
    tiempoAcumulado = 0.f;


    spriteBomba.setPosition(0, 0);
    spriteBomba.setTextureRect(sf::IntRect(0, 0, ANCHO_FRAME, ALTO_FRAME));
}
//RETROCEDER

sf::Sprite& FichaEspeciales::getBotonDeshacer() {
    return spriteRetroceder;
}
void FichaEspeciales::setMostrarBotonDeshacer(bool mostrar) {
    mostrarBotonDeshacer = mostrar;
}
void FichaEspeciales::dibujarBotonDeshacer(sf::RenderWindow& ventana) {
    if (mostrarBotonDeshacer) {
        ventana.draw(spriteRetroceder);
    }
}
bool FichaEspeciales::fueClickeadoBotonDeshacer(sf::Vector2i mousePos) {
    return spriteRetroceder.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}
//COHETE
sf::Sprite& FichaEspeciales::getBotonCohete() {
    return spriteBotonCohete;
}
void FichaEspeciales::setMostrarBotonCohete(bool mostrar) {
    mostrarBotonCohete = mostrar;
}
void FichaEspeciales::dibujarBotonCohete(sf::RenderWindow& ventana, int nivelActual) {
    if (nivelActual < 1) return;
    ventana.draw(spriteBarraCohete);


    if (coheteDisponible() && mostrarBotonCohete) {
        ventana.draw(spriteBotonCohete);

    }
}
bool FichaEspeciales::fueClickeadoBotonCohete(sf::Vector2i mousePos) {
    if (!coheteDisponible()) return false;

    return spriteBotonCohete.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}
void FichaEspeciales::cargarSpriteCohete() {
    spriteBarraCohete.setTexture(texturaBarraCohete);
    altoFragmento = 170;
    anchoFragmento = 278;
    altoFragmento = texturaBarraCohete.getSize().y / maxMovimientos;
    anchoFragmento = texturaBarraCohete.getSize().x;
    spriteBarraCohete.setPosition(850.f, 450.f);
    spriteBarraCohete.setScale(0.50f, 0.40f);
    sf::Vector2u size = texturaBarraCohete.getSize();


    anchoFragmento = texturaBarraCohete.getSize().x;
    altoFragmento = texturaBarraCohete.getSize().y / maxMovimientos;


    spriteBotonCohete.setTexture(texturaBotonCohete);
    spriteBotonCohete.setPosition(850.f, 430.f);
    posicionBotonCohete = sf::Vector2f(790.f, 430.f);
    spriteBotonCohete.setScale(0.3f,0.3f);


    spriteBarraCohete.setTextureRect(sf::IntRect(0, 0, anchoFragmento, altoFragmento));
}

void FichaEspeciales::dibujarBarraCohete(sf::RenderWindow& ventana, int nivelActual) {
    if (nivelActual < 1) return;
    ventana.draw(spriteBarraCohete);
}

void FichaEspeciales::registrarMovimiento() {
      if (contadorMovimientos < maxMovimientos)
        contadorMovimientos++;

    int indice = std::min(contadorMovimientos, maxMovimientos);

    spriteBarraCohete.setTextureRect(sf::IntRect(0, altoFragmento * indice, anchoFragmento, altoFragmento));
    coheteListo = (contadorMovimientos >= maxMovimientos);
    mostrarBotonCohete = coheteListo;
}
bool FichaEspeciales::coheteDisponible() const {
    return coheteListo;
}

void FichaEspeciales::usarCohete() {
    contadorMovimientos = 0;
    coheteListo = false;
    mostrarBotonCohete = false;
    spriteBarraCohete.setTextureRect(sf::IntRect(0, 0, anchoFragmento, altoFragmento));
}
int FichaEspeciales::getContadorMovimientos() const {
    return contadorMovimientos;
}
void FichaEspeciales::reiniciarContadorMovimientos() {
    contadorMovimientos = 0;
    coheteListo = false;
}
sf::Sprite FichaEspeciales::getSpriteCoheteEnUso(int frame) const {
    sf::Sprite sprite;
    sprite.setTexture(texturaBotonCohete);
    sprite.setTextureRect(sf::IntRect(0, 0, anchoFragmento, altoFragmento));
    sprite.setScale(0.3f, 0.3f);
    return sprite;
}

//MISIL
void FichaEspeciales::activarCohete() {
    coheteActivo = true;
}

void FichaEspeciales::seleccionarFicha(sf::Vector2i celda, sf::Vector2f posPixel) {
    if (coheteActivo) {
        fichaObjetivo = celda;
        efectos.iniciarAnimacionMisil( posicionBotonCohete, posPixel);
    }
}

void FichaEspeciales::actualizarCohete(float dt) {
    efectos.actualizarAnimacionMisil(dt);
}

void FichaEspeciales::dibujarCohete(sf::RenderWindow& window) {
    efectos.dibujarAnimacionMisil(window);
}

bool FichaEspeciales::coheteListoParaEliminar() const {
    return coheteActivo && efectos.animacionFinalizada();
}

sf::Vector2i FichaEspeciales::obtenerObjetivo() const {
    return fichaObjetivo;
}

//HIELO PANTALLA
void FichaEspeciales::activarPantallaHielo() {
     if (!mostrarPantallaHielo) {
        mostrarPantallaHielo = true;
        relojHielo.restart();
    }

}
void FichaEspeciales::dibujarPantallaHielo(sf::RenderWindow& ventana) {

     if (!mostrarPantallaHielo) return;

    spritePantallaHielo.setScale(
        ventana.getSize().x / static_cast<float>(texturaPantallaHielo.getSize().x),
        ventana.getSize().y / static_cast<float>(texturaPantallaHielo.getSize().y)
    );

    ventana.draw(spritePantallaHielo);

    if (relojHielo.getElapsedTime().asSeconds() >= 1.f) {
        mostrarPantallaHielo = false;
    }


}
//MEZCLADOR
void FichaEspeciales::registrarMovimientoCarga() {
    if (frameCarga < totalFramesCarga - 1) {
        frameCarga++;
    }

    int left = frameCarga * anchoFrameCarga;


    if (left + anchoFrameCarga > texturaBarraMezcla.getSize().x) {

        left = texturaBarraMezcla.getSize().x - anchoFrameCarga;

    }

    spriteBarraMezcla.setTextureRect(sf::IntRect(left, 0, anchoFrameCarga, altoFrameCarga));

    if (frameCarga == totalFramesCarga - 1) {
        mostrarBotonMezcla = true;
    }
}
void FichaEspeciales::dibujarCarga(sf::RenderWindow& ventana, int nivelActual) {

    if (nivelActual < 2) return;


    if (frameCarga < totalFramesCarga - 1 || !mostrarBotonMezcla) {
        ventana.draw(spriteBarraMezcla);

    }


    if (mostrarBotonMezcla) {
        ventana.draw(spriteBotonMezcla);
    }

}
bool FichaEspeciales::fueClickeadoBotonMezcla(sf::Vector2i mousePos) {
    if (!mostrarBotonMezcla) return false;
    return spriteBotonMezcla.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}
void FichaEspeciales::usarMezcla() {

     frameCarga = 0;
    mostrarBotonMezcla = false;

    spriteBarraMezcla.setTextureRect(sf::IntRect(
        0,
        0,
        anchoFrameCarga,
        altoFrameCarga
    ));
}
