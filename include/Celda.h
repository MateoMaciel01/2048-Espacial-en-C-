#pragma once

struct Celda {
    int valor = 0;
    bool congelada = false;
    int turnosCongelada = 0;
    int valorFusionado = 0;
    float tiempoFusion = 0.f;

    // POSICIÓN VISUAL
    sf::Vector2f posicionVisual;
    sf::Vector2f posicionObjetivo;

    bool operator==(const Celda& otra) const {
        return valor == otra.valor &&
               congelada == otra.congelada &&
               turnosCongelada == otra.turnosCongelada;
    }

    bool operator!=(const Celda& otra) const {
        return !(*this == otra);
    }
};
