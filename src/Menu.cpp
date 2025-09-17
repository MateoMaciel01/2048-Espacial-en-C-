#include "Menu.h"
#include "Estados.h"
#include "FichaEspeciales.h"
#include "Juego.h"
#include <iostream>
#pragma once

Menu::Menu(const sf::Vector2u& tamVentana):botonNueva({250, 300}, {550, 80}, "NUEVA PARTIDA", "resourses/nueva_partida.png"),
    botonContinuar ({250, 370}, {550, 80}, "CONTINUAR", "resourses/continuar.png"),
    botonPuntajes({250, 440}, {550, 80},"PUNTAJES", "resourses/puntajes.png"),
    botonReglas({250, 510}, {550, 80}, "REGLAS", "resourses/reglas.png"),
    botonSalir({250, 580}, {550, 80}, "SALIR", "resourses/salir.png"),
    botonIniciarJuego({670, 600}, {350, 70}, "INICIAR JUEGO", "resourses/iniciar.png"),
    botonVolver({0, 600}, {350, 70}, "VOLVER", "resourses/atras.png"),
    botonContinuarPausa({400, 400}, {350, 80}, "CONTINUAR", "resourses/continuar.png"),
    botonGuardar({400, 490}, {350, 80}, "GUARDAR", "resourses/guardar.png"),
    botonVolverMenu({400, 570}, {350, 80}, "MENU", "resourses/boton_menu.png"),
    botonVolverReglas({800, 590}, {400, 80}, "VOLVER", "resourses/atras.png"),
    botonReintentar({580, 490}, {200, 100}, "REINTENTAR", "resourses/reintentar.png"),
    botonMenuPerdiste({350, 490}, {200, 100}, "BMENU", "resourses/menu_boton.png"),
    botonMenuVictoria({800, 590}, {200, 100}, "BMENU", "resourses/menu_boton.png"),
    botonGuardarPartida({0, 590}, {200, 150}, "GUARDAR_PARTIDA", "resourses/guardar_boton.png"),
    botonSiguienteNivel({580, 490}, {200, 100}, "SIGUIENTE_NIVEL", "resourses/siguiente_nivel_boton.png"){

    if (!fondoTexture.loadFromFile("resourses/Menu.jpg")) {

        std::cerr << "Error al cargar la imagen del menú." << std::endl;
    }

    fondoSprite.setTexture(fondoTexture);


    fondoSprite.setScale(
        static_cast<float>(tamVentana.x) / fondoTexture.getSize().x,
        static_cast<float>(tamVentana.y) / fondoTexture.getSize().y);

    if (!subfondoTexture.loadFromFile("resourses/nombre.jpg")) {
    std::cerr << "No se pudo cargar el fondo para ingresar nombre." << std::endl;
    }
        subfondoSprite.setTexture(subfondoTexture);
        subfondoSprite.setScale(
        static_cast<float>(tamVentana.x) / subfondoTexture.getSize().x,
        static_cast<float>(tamVentana.y) / subfondoTexture.getSize().y
    );

     if (!fuente.loadFromFile("Resourses/Font/PressStart2P-Regular.ttf")) {
        std::cerr << "No se pudo cargar la fuente." << std::endl;
    }


    textoNombre.setFont(fuente);
    textoNombre.setCharacterSize(24);
    textoNombre.setFillColor(sf::Color::White);
    textoNombre.setPosition(250,460);


    if (!reglasTexture.loadFromFile("resourses/reglasMenu.png")) {
    std::cerr << "No se pudo cargar la imagen de las reglas." << std::endl;
    }
    reglasSprite.setTexture(reglasTexture);
    reglasSprite.setScale(
        static_cast<float>(tamVentana.x) / reglasTexture.getSize().x,
        static_cast<float>(tamVentana.y) / reglasTexture.getSize().y
    );

    if (!puntosTexture.loadFromFile("resourses/puntos.jpg")) {
    std::cerr << "No se pudo cargar la imagen de los puntos." << std::endl;
    }
    puntosSprite.setTexture(puntosTexture);
    puntosSprite.setScale(
        static_cast<float>(tamVentana.x) / subfondoTexture.getSize().x,
        static_cast<float>(tamVentana.y) / subfondoTexture.getSize().y
    );

    if (!fondoPausaTexture.loadFromFile("resourses/pausa.png")) {
    std::cerr << "No se pudo cargar la imagen de pausa." << std::endl;
    }
    fondoPausaSprite.setTexture(fondoPausaTexture);
    fondoPausaSprite.setScale(
        static_cast<float>(tamVentana.x) / fondoPausaTexture.getSize().x,
        static_cast<float>(tamVentana.y) / fondoPausaTexture.getSize().y
    );
    if (!victoriaTexture.loadFromFile("resourses/victoria.png")) {
    std::cerr << "No se pudo cargar la imagen de pausa." << std::endl;
    }
    victoriaSprite.setTexture(victoriaTexture);
    victoriaSprite.setScale(
        static_cast<float>(tamVentana.x) / victoriaTexture.getSize().x,
        static_cast<float>(tamVentana.y) / victoriaTexture.getSize().y
    );
    if (!fondoPerdisteTexture.loadFromFile("resourses/perdiste.png")) {
        std::cerr << "Error al cargar la imagen de pantalla de derrota." << std::endl;
    }
    float escala = 0.45f;
    fondoPerdisteSprite.setScale(escala, escala);
    fondoPerdisteSprite.setTexture(fondoPerdisteTexture);

    sf::Vector2u tamOriginal = fondoPerdisteTexture.getSize();
    sf::Vector2f tamEscalado(tamOriginal.x * escala, tamOriginal.y * escala);

    fondoPerdisteSprite.setPosition(
        (tamVentana.x - tamEscalado.x) / 2.f,
        (tamVentana.y - tamEscalado.y) / 2.f);

    if (!fondoGanasteTexture.loadFromFile("resourses/ganaste.png")) {
        std::cerr << "Error al cargar la imagen de pantalla de derrota." << std::endl;
    }
    float escalaGanaste = 0.45f;
    fondoGanasteSprite.setScale(escalaGanaste, escalaGanaste);
    fondoGanasteSprite.setTexture(fondoGanasteTexture);

    sf::Vector2u tamOriginalGanaste = fondoGanasteTexture.getSize();
    sf::Vector2f tamEscaladoGanaste(tamOriginalGanaste.x * escalaGanaste, tamOriginalGanaste.y * escalaGanaste);

    fondoGanasteSprite.setPosition(
        (tamVentana.x - tamEscaladoGanaste.x) / 2.f,
        (tamVentana.y - tamEscaladoGanaste.y) / 2.f);


}

void Menu::dibujar(sf::RenderWindow& ventana) {
     switch (estadoActual) {
            case EN_MENU:
                ventana.draw(fondoSprite);
                botonNueva.dibujar(ventana);
                botonContinuar.dibujar(ventana);
                botonPuntajes.dibujar(ventana);
                botonReglas.dibujar(ventana);
                botonSalir.dibujar(ventana);
                break;

            case EN_SUBMENU_NOMBRE:
                ventana.draw(subfondoSprite);
                ventana.draw(textoNombre);
                botonIniciarJuego.dibujar(ventana);
                botonVolver.dibujar(ventana);
                break;

            case VER_REGLAS:
                ventana.draw(reglasSprite);
                botonVolverReglas.dibujar(ventana);
                break;
            case SUBMENU_PUNTOS:
                ventana.draw(puntosSprite);
                botonVolverReglas.dibujar(ventana);
                break;

            case PAUSA:
                ventana.draw(fondoPausaSprite);
                botonContinuarPausa.dibujar(ventana);
                botonGuardar.dibujar(ventana);
                botonVolverMenu.dibujar(ventana);
                break;
            case NIVEL_COMPLETO:
                ventana.draw(fondoGanasteSprite);
                botonMenuPerdiste.dibujar(ventana);

                botonSiguienteNivel.dibujar(ventana);
                break;

            case PERDIDO:
                ventana.draw(fondoPerdisteSprite);

                botonMenuPerdiste.dibujar(ventana);
                botonReintentar.dibujar(ventana);
                break;
            case VICTORIA:
                ventana.draw(victoriaSprite);
                botonGuardarPartida.dibujar(ventana);
                botonMenuVictoria.dibujar(ventana);
                break;

            default:
                break;
        }

}


EstadoMenu Menu::manejarClick(sf::Vector2f mousePos) {

    if (fichaEspeciales) {
        sf::FloatRect bounds = fichaEspeciales->getBotonDeshacer().getGlobalBounds();


        if (bounds.contains(mousePos)) {

            return DESHACER;
        } else {

        }
    } else {

    }
    if (estadoActual == VER_REGLAS) {
        if (botonVolverReglas.fueClickeado(mousePos)) {
            estadoActual = EN_MENU;
            return EN_MENU;
        }
        return NINGUNO;
    }
    if (estadoActual == SUBMENU_PUNTOS) {
        if (botonVolverReglas.fueClickeado(mousePos)) {
            estadoActual = EN_MENU;
            return EN_MENU;
        }
        return NINGUNO;
    }

    if(estadoActual == VICTORIA){
        if(botonMenuVictoria.fueClickeado(mousePos))return EN_MENU;
        if(botonGuardarPartida.fueClickeado(mousePos))return GUARDAR_PARTIDA;
        return NINGUNO;
    }
    if (estadoActual == PERDIDO) {
        if (botonReintentar.fueClickeado(mousePos)) return NUEVA_PARTIDA;
        if (botonMenuPerdiste.fueClickeado(mousePos)) return EN_MENU;
        if (botonGuardarPartida.fueClickeado(mousePos)) return GUARDAR_PARTIDA;
    return NINGUNO;
    }
    if(estadoActual == NIVEL_COMPLETO){
        if(botonSiguienteNivel.fueClickeado(mousePos))return SIGUIENTE_NIVEL;
        if (botonMenuPerdiste.fueClickeado(mousePos)) return EN_MENU;
        if (botonGuardarPartida.fueClickeado(mousePos)) return GUARDAR_PARTIDA;
    return NINGUNO;
    }

    if (estadoActual == PAUSA) {
        if (botonContinuarPausa.fueClickeado(mousePos)) return EN_JUEGO;
        if (botonGuardar.fueClickeado(mousePos)) return GUARDAR_PARTIDA;
        if (botonVolverMenu.fueClickeado(mousePos)) return EN_MENU;
    }

    if (ingresandoNombre) {
        if (botonIniciarJuego.fueClickeado(mousePos)) {
            if (!nombreIngresado.isEmpty()) {
                nombreConfirmado = true;
                ingresandoNombre = false;
                return NUEVA_PARTIDA;
            }
            return NINGUNO;

        } else if (botonVolver.fueClickeado(mousePos)) {
            ingresandoNombre = false;
            return EN_MENU;
        }
        return NINGUNO;
    }
    if (botonNueva.fueClickeado(mousePos)) return NUEVA_PARTIDA;
    if (botonIniciarJuego.fueClickeado(mousePos)) return NUEVA_PARTIDA;
    if (botonContinuar.fueClickeado(mousePos)) return CONTINUAR_PARTIDA;
    if (botonPuntajes.fueClickeado(mousePos)) return SUBMENU_PUNTOS;
    if (botonReglas.fueClickeado(mousePos)) {
        estadoActual = VER_REGLAS;
        return VER_REGLAS;
    }
    if (botonSalir.fueClickeado(mousePos)) {

    return SALIR;
}



    if (fichaEspeciales && fichaEspeciales->getBotonDeshacer().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {

        return DESHACER;
    } else {

    }


    return NINGUNO;
}
void Menu::manejarEntradaNombre(sf::Event evento) {
    if (evento.type == sf::Event::TextEntered) {
        if (evento.text.unicode == '\b') {
            if (!nombreIngresado.isEmpty())
                nombreIngresado.erase(nombreIngresado.getSize() - 1, 1);
        }
         else if (evento.text.unicode < 128) {
            nombreIngresado += static_cast<char>(evento.text.unicode);
        }

        textoNombre.setString(nombreIngresado);
    }
}
bool Menu::estaIngresandoNombre() const {
    return ingresandoNombre && !nombreConfirmado;
}

bool Menu::nombreFueConfirmado() const {
    return nombreConfirmado;
}
void Menu::iniciarIngresoNombre() {
    ingresandoNombre = true;
    nombreIngresado = "";
    textoNombre.setString("");
    nombreConfirmado = false;
    estadoActual = EN_SUBMENU_NOMBRE;
}
void Menu::setEstado(EstadoMenu nuevoEstado) {
    estadoActual = nuevoEstado;
}
EstadoMenu Menu::getEstadoActual() const {
    return estadoActual;
}
void Menu::setFichaEspeciales(FichaEspeciales* fichasEsp) {
    fichaEspeciales = fichasEsp;
}
