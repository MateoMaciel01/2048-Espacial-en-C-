#include "Efectos.h"
#include <iostream>

Efectos::Efectos() : movimientos(0), centro(390, 390), velocidad(600.f), animacionActiva(false) {


    if (!texturaBoton.loadFromFile("resourses/Efectos/nave_portal.png"))
        std::cerr << "❌ No se pudo cargar la textura del botón del portal\n";
    spriteBoton.setTexture(texturaBoton);
    spriteBoton.setPosition(80,200);
    spriteBoton.setScale(0.3f, 0.3f);

    if (!texturaPortal.loadFromFile("resourses/Efectos/espiral.png"))
        std::cerr << "❌ No se pudo cargar la textura del portal\n";

    spritePortal.setTexture(texturaPortal);
    sf::Vector2u texSize = texturaPortal.getSize();
    spritePortal.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
    spritePortal.setRotation(0.f);
    spritePortal.setPosition(centro);


    if (!texturaBarra.loadFromFile("resourses/Efectos/spriteBarra.png")) {
    std::cerr << "❌ No se pudo cargar la textura de la barra del portal\n";
    }

    spriteBarra.setTexture(texturaBarra);
    spriteBarra.setTextureRect(sf::IntRect(0, 0, anchoFrameBarra, altoFrameBarra));
    spriteBarra.setPosition(100.f, 220.f);
    spriteBarra.setScale(0.25f, 0.25f);

    //MISIL
    if (!texturaMisil.loadFromFile("resourses/efectos/misil.png")) {
        std::cerr << "❌ No se pudo cargar la textura del misil\n";
    }
    spriteMisil.setTexture(texturaMisil);
    spriteMisil.setOrigin(0.f, 0.f);
    spriteMisil.setScale(0.5f, 0.5f);
    spriteMisil.setOrigin(0, texturaMisil.getSize().y / 2.f);
}



void Efectos::registrarMovimiento() {
     if (movimientos < maxMovimientos) {
        movimientos++;
        frameBarra = movimientos - 1;

        int left = frameBarra * anchoFrameBarra;

        if (left + anchoFrameBarra > texturaBarra.getSize().x) {
            left = texturaBarra.getSize().x - anchoFrameBarra;
            std::cout<<"CAMBIO FRAME"<<std::endl;

        }

        spriteBarra.setTextureRect(sf::IntRect(left, 0, anchoFrameBarra, altoFrameBarra));
    }

    if (movimientos == maxMovimientos) {
        botonVisible = true;

    }
}
bool Efectos::estaListo() const {
    return botonVisible;
}

void Efectos::activarEfecto(const std::vector<std::vector<Celda>>& tablero, const Fichas& fichas, sf::Vector2u tamVentana) {

    fichasAbsorbiendose.clear();
    spritePortal.setRotation(0.f);
    spritePortal.setPosition(centro);

    centro = sf::Vector2f(tamVentana.x / 2.f, tamVentana.y / 2.f);
    spritePortal.setPosition(centro);


    for (int i = 0; i < tablero.size(); ++i) {
        for (int j = 0; j < tablero[i].size(); ++j) {
            int val = tablero[i][j].valor;
            if (val > 0) {

                sf::Sprite sprite = fichas.obtenerSprite(val);
                sprite.setPosition(j * 130.f + 50.f, i * 130.f + 50.f);
                fichasAbsorbiendose.push_back({sprite, sprite.getPosition(), 0.f});
            }
        }
    }


    portalActivo = true;
    absorbiendo = true;
    absorcionTerminada = false;
    tiempoPortal = 0.f;

}
void Efectos::actualizar(float dt) {

    if (!absorbiendo) return;
        if (portalActivo) {
                tiempoPortal += dt;
                rotacionPortal += 60.f * dt;
                spritePortal.setRotation(rotacionPortal);
            }
            if (absorbiendo) {
                bool todoListo = true;
                for (auto& f : fichasAbsorbiendose) {
                    f.tiempo += dt;
                    float t = std::min(f.tiempo / 1.0f, 1.f);
                    sf::Vector2f destino = centro;
                    sf::Vector2f nuevaPos = f.posicionInicial + (destino - f.posicionInicial) * t;
                    f.sprite.setPosition(nuevaPos);
                    f.sprite.setScale(1 - t, 1 - t);



                    if (t < 1.f)
                        todoListo = false;
                }

                if (todoListo || tiempoPortal > 2.f) {

                    absorbiendo = false;
                    portalActivo = false;
                    fichasAbsorbiendose.clear();
                }

        }
}

void Efectos::dibujar(sf::RenderWindow& ventana,int nivelActual) {
    if (nivelActual < 2) return;
    if (movimientos < maxMovimientos || !botonVisible) {
            ventana.draw(spriteBarra);

    }

    if (botonVisible) {
        ventana.draw(spriteBoton);
    }

    if (absorbiendo || portalActivo) {
        ventana.draw(spritePortal);
    }

    if (absorbiendo) {
        for (auto& f : fichasAbsorbiendose)
            ventana.draw(f.sprite);
    }

}
void Efectos::procesarClick(const sf::Vector2i& mousePos, sf::RenderWindow& ventana) {
    if (botonVisible) {
        sf::Vector2f mouse = ventana.mapPixelToCoords(mousePos);
        if (spriteBoton.getGlobalBounds().contains(mouse)) {
            botonVisible = false;
        }
    }

}

bool Efectos::estaAbsorbiendo() const {
    return absorbiendo;
}

bool Efectos::fichasAbsorbidasCompletamente() const {
    return !absorbiendo;
}

void Efectos::reiniciar() {
    absorbiendo = false;
    absorcionTerminada = false;
    movimientos = 0;
    frameBarra = 0;
    botonVisible = false;
    spriteBarra.setTextureRect(sf::IntRect(0, 0, anchoFrameBarra, altoFrameBarra));
    fichasAbsorbiendose.clear();
}


bool Efectos::fueClickeado(const sf::Vector2i& mousePos) const {
    if (!botonVisible) return false;
    return spriteBoton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}
bool Efectos::manejarClick(const sf::Vector2i& mousePos, sf::RenderWindow& ventana,
                  const std::vector<std::vector<Celda>>& tablero, const Fichas& fichas) {
     if (!botonVisible) {
        return false;
    }

    sf::FloatRect bounds = spriteBoton.getGlobalBounds();

    if (bounds.contains(static_cast<sf::Vector2f>(mousePos))) {
        reiniciar();
        absorcionTerminada = false;
        activarEfecto(tablero, fichas, ventana.getSize());

        return true;
    }

    return false;
}

//MISILES COHETE
void Efectos::iniciarAnimacionMisil(sf::Vector2f origen, sf::Vector2f destino_) {
    posicionActual = origen;
    destino = destino_;



    direccion.x = (destino.x > origen.x) ? 1.f : ((destino.x < origen.x) ? -1.f : 0.f);
    direccion.y = (destino.y > origen.y) ? 1.f : ((destino.y < origen.y) ? -1.f : 0.f);

    spriteMisil.setPosition(posicionActual);
    animacionActiva = true;
}

void Efectos::actualizarAnimacionMisil(float dt) {
    if (!animacionActiva) return;


    if (!animacionActiva) return;

    posicionActual.x += direccion.x * velocidad * dt;
    posicionActual.y += direccion.y * velocidad * dt;

    spriteMisil.setPosition(posicionActual);
}

void Efectos::dibujarAnimacionMisil(sf::RenderWindow& window) {
    if (animacionActiva)
        window.draw(spriteMisil);
}

bool Efectos::animacionFinalizada() const {
    return !animacionActiva;
}
const sf::Texture& Efectos::getTexturaMisil() const {
    return texturaMisil;
}
