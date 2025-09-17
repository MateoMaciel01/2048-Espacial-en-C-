#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Tablero.h"
#include "Interfaz.h"
#include "Estados.h"
#include "Juego.h"
#include "Fichas.h"
#include "Opciones.h"
#include "FichaEspeciales.h"
#include "Efectos.h"

int main() {
    sf::RenderWindow ventana(sf::VideoMode(1142, 700), "2048!");
    Menu menu(ventana.getSize());
    Tablero tablero(ventana.getSize());
    Juego juego(&tablero);
    Fichas fichas;
    fichas.setTamanioDeseado(100.f, 100.f);
    fichas.cargarFichas();
    fichas.cargarFichaHielo();
    float deltaTime = 0.f;
    FichaEspeciales fichaEspeciales;
    Opciones opciones({600.f, 20.f}, {150.f, 50.f}, "0");
    juego.setFichaEspeciales(&fichaEspeciales);
    bool saltarChequeoNivel = false;
    Efectos efectoPortal;



Interfaz interfaz(
    ventana,
    menu,
    juego,
    tablero,
    fichas,
    opciones,
    fichaEspeciales,
    efectoPortal,
    saltarChequeoNivel,
    deltaTime
);
   EstadoMenu estado = EN_MENU;
    while (ventana.isOpen()) {
        ventana.clear(sf::Color::Black);
        float deltaTime = juego.relojDeltaTime.restart().asSeconds();

        float dt = juego.relojDeltaTime.restart().asSeconds();
        juego.actualizar(dt);
        juego.actualizarAnimaciones(deltaTime);

        EstadoMenu nuevoEstado = interfaz.procesarEventos(estado);

        if (nuevoEstado != NINGUNO) {
            estado = nuevoEstado;
            menu.setEstado(estado);
        } else if (nuevoEstado != NINGUNO) {
            estado = nuevoEstado;
            menu.setEstado(estado);
        }



        switch (estado) {
                case EN_MENU:
                case VER_REGLAS:
                    menu.dibujar(ventana);
                    break;
                case SUBMENU_PUNTOS:
                    opciones.cargarTop5();
                    menu.dibujar(ventana);
                    opciones.dibujarSubmenuPuntajes(ventana);
                    break;
                case NINGUNO:
                    estado = EN_MENU;
                    menu.setEstado(estado);
                    menu.dibujar(ventana);
                    break;

                case EN_SUBMENU_NOMBRE:
                    menu.dibujar(ventana);
                    break;

                case EN_JUEGO:
                    if (!juego.efectoPortal.estaAbsorbiendo()) {
                        const auto& t = juego.obtenerTablero();
                        juego.obtenerTableroGrafico()->actualizarDesdeLogica(t, fichas.getMapaTexturas(),&fichaEspeciales);
                    }
                    juego.obtenerTableroGrafico()->dibujar(ventana);
                    juego.dibujarBombas(ventana, deltaTime);
                    juego.dibujarExplosiones(ventana, deltaTime);
                    juego.actualizarSacudidaPantalla(ventana, deltaTime);
                    fichaEspeciales.dibujarBotonDeshacer(ventana);
                    juego.actualizarEstadoMezcla();
                    fichaEspeciales.dibujarBotonCohete(ventana, juego.getNivelActual());
                    fichaEspeciales.dibujarBarraCohete(ventana, juego.getNivelActual());
                    fichaEspeciales.dibujarCarga(ventana, juego.getNivelActual());
                    juego.dibujarHielos(ventana, fichas);
                    juego.actualizarCohete(ventana,deltaTime);

                    juego.efectoPortal.actualizar(deltaTime);
                    juego.efectoPortal.dibujar(ventana,juego.getNivelActual());

                    if (juego.coheteActivo) {
                        juego.dibujarCohete(ventana);
                    }
                    if (juego.misilEnAnimacion) {
                        ventana.draw(juego.spriteMisil);
                    }
                    opciones.actualizarTiempo(juego.getTiempoJugado());
                    opciones.actualizarPuntaje(juego.getPuntaje());
                    opciones.dibujar(ventana);
                    fichaEspeciales.dibujarPantallaHielo(ventana);

                    break;
                case NIVEL_COMPLETO:
                    juego.obtenerTableroGrafico()->actualizarDesdeLogica(juego.obtenerTablero(), fichas.getMapaTexturas(),&fichaEspeciales);
                    juego.obtenerTableroGrafico()->dibujar(ventana);
                    menu.dibujar(ventana);
                    opciones.dibujarSubmenu(ventana);

                    break;
                case SIGUIENTE_NIVEL:
                    juego.obtenerTableroGrafico()->dibujar(ventana);
                    break;
                case PERDIDO:
                    juego.obtenerTableroGrafico()->actualizarDesdeLogica(juego.obtenerTablero(), fichas.getMapaTexturas(),&fichaEspeciales);
                    juego.obtenerTableroGrafico()->dibujar(ventana);
                    menu.dibujar(ventana);
                    opciones.dibujarSubmenu(ventana);

                    break;

                case REINTENTAR:

                    break;
                case VICTORIA:

                    menu.dibujar(ventana);
                    break;
                case PAUSA:
                    menu.dibujar(ventana);
                    break;
                case NUEVA_PARTIDA:
                    juego.reiniciar();
                    juego.iniciarNivel(1);
                    juego.iniciarTemporizador();
                    estado = EN_JUEGO;
                    break;

                default:
                    if (menu.estaIngresandoNombre()) {
                        menu.dibujar(ventana);
                    }
                    break;
            }

            ventana.display();

    }

    return 0;
}
