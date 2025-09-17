#include "Interfaz.h"
#include "Estados.h"
#include "Juego.h"
#include "FichaEspeciales.h"
#include "Fichas.h"
#include "Efectos.h"
#include <iostream>



Interfaz::Interfaz(
    sf::RenderWindow& ventana_,
    Menu& menu_,
    Juego& juego_,
    Tablero& tablero_,
    Fichas& fichas_,
    Opciones& opciones_,
    FichaEspeciales& fichaEspeciales_,
    Efectos& efectoPortal_,
    bool& saltarChequeoNivel_,
    float& deltaTime_

) :
    ventana(ventana_),
    menu(menu_),
    juego(juego_),
    tablero(tablero_),
    fichas(fichas_),
    opciones(opciones_),
    fichaEspeciales(fichaEspeciales_),
    efectoPortal(efectoPortal_),
    saltarChequeoNivel(saltarChequeoNivel_),
    deltaTime(deltaTime_)

{fichaEspeciales.cargarSpriteCohete(); }
EstadoMenu Interfaz::procesarEventos(EstadoMenu estado) {
    sf::Event evento;

    sf::Clock relojDelta;

    while (ventana.pollEvent(evento)) {

                if (evento.type == sf::Event::Closed) {
                    ventana.close();
                }

                menu.setEstado(estado);


                if (estado == NUEVA_PARTIDA) {
                    std::cout << "Entrando a NUEVA_PARTIDA" << std::endl;
                    juego.borrarPartidaGuardada();
                    juego.nuevaPartida();
                    juego.iniciarNivel(1);

                    saltarChequeoNivel = false;
                    juego.iniciarTemporizador();
                    tablero.actualizarDesdeLogica(juego.obtenerTablero(), fichas.getMapaTexturas(),&fichaEspeciales);
                    opciones.actualizarPuntaje(juego.getPuntaje());
                    opciones.actualizarTiempo(juego.getTiempoJugado());
                    estado = EN_JUEGO;
                    menu.setEstado(estado);
                    return estado;
                }

                if (estado == CONTINUAR_PARTIDA) {

                    if (juego.cargarPartida()) {
                        std::cout << " Partida cargada con Exito." << std::endl;
                        juego.reanudarTemporizador();
                        auto tab = juego.obtenerTablero();
                        std::cout << "Valor en (0,0) desde obtenerTablero(): " << tab[0][0].valor << std::endl;

                        tablero.actualizarDesdeLogica(juego.obtenerTablero(), fichas.getMapaTexturas(),&fichaEspeciales);
                        opciones.actualizarPuntaje(juego.getPuntaje());
                        opciones.actualizarTiempo(juego.getTiempoJugado());
                        estado = EN_JUEGO;
                        menu.setEstado(estado);
                        saltarChequeoNivel = true;
                    } else {
                        std::cout << "Error al cargar partida." << std::endl;
                        estado = EN_MENU;
                        menu.setEstado(estado);
                    }
                    return estado;
                }
                if (estado == EN_JUEGO) {
                    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = {evento.mouseButton.x, evento.mouseButton.y};

                        if (fichaEspeciales.fueClickeadoBotonDeshacer(mousePos)) {
                            if (juego.obtenerPuntaje() >= 10) {

                                juego.deshacerMovimiento();
                                juego.restarPuntos(10);
                                tablero.actualizarDesdeLogica(juego.obtenerTablero(), fichas.getMapaTexturas(), &fichaEspeciales);
                            }
                            continue;
                        }

                        if (fichaEspeciales.fueClickeadoBotonCohete(mousePos) && fichaEspeciales.coheteDisponible()) {
                                if (!juego.puedeUsarCohete()) {
                                    std::cout << "No se puede usar el cohete. Puntaje insuficiente.\n";
                                    continue;
                                }
                            juego.activarCohete();
                        } else if (juego.estaCoheteActivo()) {

                            juego.procesarClickCohete(mousePos);
                        }

                        if (fichaEspeciales.fueClickeadoBotonMezcla(mousePos)) {

                            juego.activarMezcla();
                            fichaEspeciales.usarMezcla();
                        }
                        if (juego.efectoPortal.manejarClick(mousePos, ventana, juego.obtenerTablero(), fichas)) {



                        }

                    }


                    EstadoMenu nuevoEstado = manejarEntradaJuego(evento);
                    if (!saltarChequeoNivel && juego.nivelCompletado()) {
                        std::cout << "Nivel completado detectado" << std::endl;
                        opciones.actualizarPuntaje(juego.getPuntaje());
                        opciones.actualizarTiempo(juego.getTiempoJugado());
                        juego.marcarNivelCompletado();
                        juego.guardarProgresoDeNivel();
                        juego.guardarCheckpoint();
                        estado = (juego.getNivelActual() == 3 ? VICTORIA : NIVEL_COMPLETO);
                        menu.setEstado(estado);
                        saltarChequeoNivel = false;
                        juego.pausarTemporizador();
                        return estado; }

                    ventana.clear();
                    tablero.dibujar(ventana);
                    juego.dibujarBombas(ventana, deltaTime);
                    juego.dibujarHielos(ventana, fichas);
                    juego.actualizarCohete(ventana,deltaTime);
                    juego.dibujarCohete(ventana);
                    juego.efectoPortal.dibujar(ventana,juego.getNivelActual());
                    opciones.actualizarPuntaje(juego.getPuntaje());
                    opciones.actualizarTiempo(juego.getTiempoJugado());
                    ventana.display();

                    if (nuevoEstado == PAUSA) {
                        juego.pausarTemporizador();
                        estado = PAUSA;
                        menu.setEstado(PAUSA);
                    }else if (estado == NUEVA_PARTIDA) {
                        std::cout << "Entrando a NUEVA_PARTIDA" << std::endl;
                        juego.borrarPartidaGuardada();
                        juego.iniciarNivel(1);
                        saltarChequeoNivel = false;
                        juego.iniciarTemporizador();
                        tablero.actualizarDesdeLogica(juego.obtenerTablero(), fichas.getMapaTexturas(),&fichaEspeciales);
                        opciones.actualizarPuntaje(juego.getPuntaje());
                        opciones.actualizarTiempo(juego.getTiempoJugado());
                        estado = EN_JUEGO;
                        menu.setEstado(estado);
                        continue;
                    }

                    else if (nuevoEstado == PERDIDO) {

                        tablero.actualizarDesdeLogica(juego.obtenerTablero(), fichas.getMapaTexturas(),&fichaEspeciales);
                        tablero.dibujar(ventana);
                        estado = PERDIDO;
                        menu.setEstado(PERDIDO);
                        return PERDIDO;
                    }
                    continue;

                } else if (menu.estaIngresandoNombre()) {
                    if (evento.type == sf::Event::TextEntered) {
                        menu.manejarEntradaNombre(evento);
                    }
                    else if (evento.type == sf::Event::KeyPressed) {
                        if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                            return estado;
                        }
                    }

                    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                        EstadoMenu nuevoEstado = menu.manejarClick({evento.mouseButton.x, evento.mouseButton.y});
                        if (nuevoEstado != NINGUNO) {
                            estado = nuevoEstado;
                            menu.setEstado(estado);
                            return estado;
                        }
                    }
                }
                else if (estado == EN_MENU && evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                    EstadoMenu nuevoEstado = menu.manejarClick({evento.mouseButton.x, evento.mouseButton.y});

                    if (nuevoEstado == SALIR) {
                            ventana.close();
                        }
                        else if (nuevoEstado != NINGUNO) {

                            if (nuevoEstado == EN_JUEGO && !menu.nombreFueConfirmado()) continue;

                            estado = nuevoEstado;
                            menu.setEstado(estado);

                            if (estado == NUEVA_PARTIDA) {
                                menu.iniciarIngresoNombre();
                                estado = EN_SUBMENU_NOMBRE;
                                menu.setEstado(estado);
                            }
                        }
                        else {

                        }
                }
                if (evento.type == sf::Event::Closed) {
                    return SALIR;
                }

                if (estado == REINTENTAR) {
                    juego.restaurarCheckpoint();
                    opciones.actualizarPuntaje(juego.getPuntaje());
                    opciones.actualizarTiempo(juego.getTiempoJugado());
                    juego.reanudarTemporizador();
                    estado = EN_JUEGO;
                    menu.setEstado(EN_JUEGO);
                    continue;
                }

                else if (estado == VER_REGLAS && evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                    EstadoMenu nuevoEstado = menu.manejarClick({evento.mouseButton.x, evento.mouseButton.y});
                    if (nuevoEstado != NINGUNO) {
                        estado = nuevoEstado;
                        menu.setEstado(nuevoEstado);
                    }
                }
                else if (estado == SUBMENU_PUNTOS && evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                    EstadoMenu nuevoEstado = menu.manejarClick({evento.mouseButton.x, evento.mouseButton.y});
                    if (nuevoEstado != NINGUNO) {
                        estado = nuevoEstado;
                        menu.setEstado(nuevoEstado);
                    }
                }
                else if (estado == PERDIDO && evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                    EstadoMenu nuevoEstado = menu.manejarClick({evento.mouseButton.x, evento.mouseButton.y});

                    if (nuevoEstado != NINGUNO) {
                        estado = nuevoEstado;
                        menu.setEstado(estado);

                        if (estado == REINTENTAR) {
                            juego.restaurarCheckpoint();
                            ;
                            opciones.actualizarPuntaje(juego.getPuntaje());
                            opciones.actualizarTiempo(juego.getTiempoJugado());
                            estado = EN_JUEGO;
                            if (juego.cargarPartida()) {
                                juego.reanudarTemporizador();
                                estado = EN_JUEGO;
                                menu.setEstado(EN_JUEGO);
                            }
                            continue;
                        }

                    }
                }
                else if (estado == PAUSA && evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                EstadoMenu nuevoEstado = menu.manejarClick({evento.mouseButton.x, evento.mouseButton.y});
                    if (nuevoEstado != NINGUNO) {
                        if (nuevoEstado == GUARDAR_PARTIDA) {
                            juego.pausarTemporizador();
                            juego.guardarPartida();
                            Puntaje nuevo = {menu.obtenerNombreJugador(), juego.getPuntaje(), juego.getTiempoJugado()};
                            opciones.insertarNuevoPuntaje(nuevo);


                        }
                        else if (nuevoEstado == EN_MENU) {
                            estado = EN_MENU;
                        } else if (nuevoEstado == EN_JUEGO) {
                             juego.reanudarTemporizador();
                            estado = EN_JUEGO;
                        }
                        menu.setEstado(estado);
                    }
                }
                else if (estado == NIVEL_COMPLETO && evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                EstadoMenu nuevoEstado = menu.manejarClick({evento.mouseButton.x, evento.mouseButton.y});

                    if (nuevoEstado != NINGUNO) {
                        estado = nuevoEstado;
                        menu.setEstado(estado);
                        if(nuevoEstado == GUARDAR_PARTIDA){
                            juego.pausarTemporizador();
                            juego.guardarPartida();
                            Puntaje nuevo = {menu.obtenerNombreJugador(), juego.getPuntaje(), juego.getTiempoJugado()};
                            opciones.insertarNuevoPuntaje(nuevo);


                            estado = NIVEL_COMPLETO;

                        }

                        if (estado == SIGUIENTE_NIVEL) {

                            juego.reanudarTemporizador();
                            juego.irAlSiguienteNivel();
                            tablero.configurarParaNivel(juego.nivelActual);
                            opciones.actualizarPuntaje(juego.getPuntaje());
                            opciones.actualizarTiempo(juego.getTiempoJugado());
                            estado = EN_JUEGO;
                            menu.setEstado(estado);
                        } else if (estado == EN_MENU) {

                        }
                    }
                }
                else if (estado == VICTORIA && evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                    EstadoMenu nuevoEstado = menu.manejarClick({evento.mouseButton.x, evento.mouseButton.y});
                    if (nuevoEstado != NINGUNO) {
                        estado = nuevoEstado;
                        menu.setEstado(estado);
                        if(nuevoEstado == GUARDAR_PARTIDA){
                            juego.pausarTemporizador();
                            juego.guardarPartida();
                            Puntaje nuevo = {menu.obtenerNombreJugador(), juego.getPuntaje(), juego.getTiempoJugado()};
                            opciones.insertarNuevoPuntaje(nuevo);


                            estado= VICTORIA;

                        }

                        if (estado == EN_MENU) {
                        }
                    }
                }
                if (estado == PERDIDO || estado == NIVEL_COMPLETO || estado == VICTORIA) {
                    juego.pausarTemporizador();

                }




        return estado;

    }

}

EstadoMenu Interfaz::manejarEntradaJuego(sf::Event evento) {
    if (evento.type == sf::Event::KeyPressed) {
        switch (evento.key.code) {
            case sf::Keyboard::Up:
                juego.moverArriba();
                break;
            case sf::Keyboard::Down:
                juego.moverAbajo();
                break;
            case sf::Keyboard::Left:
                juego.moverIzquierda();
                break;
            case sf::Keyboard::Right:
                juego.moverDerecha();
                break;
            case sf::Keyboard::Escape:
                return PAUSA;
            default:
                break;
        }
        if (!juego.hayMovimientosDisponibles()) {
            menu.setEstado(PERDIDO);
            return PERDIDO;
        }
        if (juego.coheteActivo) {
            return NINGUNO;
        }

    }

    return NINGUNO;
}
