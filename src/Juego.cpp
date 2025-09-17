#include "Juego.h"
#include <cstdlib>
#include "Tablero.h"
#include <ctime>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>//PARA USAR SQRT Y ATAN2



Juego::Juego(Tablero* tableroExtern) : tablero(tableroExtern) {
    efectoPortal.absorcionTerminada = true;
    tiempoDesdeAbsorcion = 0.f;
    //MISIL
    const sf::Texture& textura = efectoPortal.getTexturaMisil();
    spriteMisil.setTexture(textura);
    spriteMisil.setScale(0.4f, 0.4f);

    std::srand(std::time(nullptr));
    iniciarNivel(1);
}
Juego::~Juego() {
    delete tablero;
}
void Juego::generarFichaAleatoria() {

        std::vector<std::pair<int, int>> vacios;
        for (int i = 0; i < tableroLogica.size(); ++i) {
            for (int j = 0; j < tableroLogica[i].size(); ++j) {
                if (tableroLogica[i][j].valor == 0) {
                    vacios.emplace_back(i, j);
                }
            }
        }
        if (!vacios.empty()) {
        auto [fila, col] = vacios[rand() % vacios.size()];

        Celda nueva;

        bool esBomba = (nivelActual >= 2 && rand() % 15 == 0);
        bool esHielo = (nivelActual >= 2 && rand() % 5 == 0);


        if (esBomba) {
            nueva.valor = -1;
        } else {
            nueva.valor = (rand() % 10 < 9) ? 2 : 4;

            if (esHielo) {
                nueva.congelada = true;
                fichaHieloPendiente = true;
                nueva.turnosCongelada = 10;
            }
        }

        sf::Vector2f posVisual = calcularPosicionVisual(fila, col);
        nueva.posicionVisual = posVisual;
        nueva.posicionObjetivo = posVisual;
        fichaPendiente = nueva;
        posFichaPendiente = {fila, col};
        hayFichaPendiente = true;

    }

}

void Juego::iniciarNivel(int nivel) {
    nivelYaCompletado = false;
    nivelActual = nivel;
    int tamanio = 4;
    if (nivel == 2) tamanio = 5;
    else if (nivel == 3) tamanio = 6;

    if (nivel == 1) objetivoNivel = 64;
    else if (nivel == 2) objetivoNivel = 1024;
    else if (nivel == 3) objetivoNivel = 2048;
    else objetivoNivel = 2048;



    tablero->configurarParaNivel(nivelActual);

    tableroLogica = std::vector<std::vector<Celda>>(tamanio, std::vector<Celda>(tamanio));
    limpiarTablero();

    efectoPortal.absorcionTerminada = true;

    generarFichaAleatoria();
    generarFichaAleatoria();

    tablero->actualizarDesdeLogica(tableroLogica, fichas.getMapaTexturas(),fichaEspeciales);

    if (fichaEspeciales) {
        fichaEspeciales->setMostrarBotonDeshacer(nivelActual >= 2);
        fichaEspeciales->setMostrarBotonCohete(nivelActual >= 1);
        fichaEspeciales->reiniciarContadorMovimientos();
    }
}
bool Juego::nivelCompletado() const {
    for (const auto& fila : tableroLogica) {
        for (const auto& celda : fila) {
            if (celda.valor >= objetivoNivel) {
                return true;
            }
        }
    }
    return false;
}
bool Juego::hayMovimientosDisponibles() const {
    int n = tableroLogica.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (tableroLogica[i][j].valor == 0) return true;
            if (i > 0 && tableroLogica[i][j].valor == tableroLogica[i - 1][j].valor) return true;
            if (i < n - 1 && tableroLogica[i][j].valor == tableroLogica[i + 1][j].valor) return true;
            if (j > 0 && tableroLogica[i][j].valor == tableroLogica[i][j - 1].valor) return true;
            if (j < n - 1 && tableroLogica[i][j].valor == tableroLogica[i][j + 1].valor) return true;
        }
    }
    return false;
}
void Juego::reiniciar() {
    nivelYaCompletado = false;
   nivelActual = 1;
    puntaje = 0;
    tiempoJugado = 0;

    int tam = 4;
    tableroLogica = std::vector<std::vector<Celda>>(tam, std::vector<Celda>(tam));

    for (int fila = 0; fila < tam; ++fila) {
        for (int col = 0; col < tam; ++col) {
            tableroLogica[fila][col].valor = 0;
            tableroLogica[fila][col].congelada = false;
            tableroLogica[fila][col].turnosCongelada = 0;

            tableroLogica[fila][col].posicionObjetivo = calcularPosicionVisual(fila, col);
            tableroLogica[fila][col].posicionVisual = tableroLogica[fila][col].posicionObjetivo;
        }
    }

    fichaEspeciales->reiniciarContadorMovimientos();
    fichaEspeciales->mostrarBotonCohete = false;
    fichaEspeciales->mostrarBotonDeshacer = false;
    fichaEspeciales->frameCarga = 0;
    fichaEspeciales->mostrarBotonMezcla = false;

    efectoPortal.movimientos = 0;
    efectoPortal.spriteBarra.setTextureRect(sf::IntRect(0, 0, efectoPortal.anchoFrameBarra, efectoPortal.altoFrameBarra));
    efectoPortal.absorcionTerminada = true;

    coheteActivo = false;
    mezclaDisponible = false;
    misilEnAnimacion = false;

    tablero->configurarParaNivel(nivelActual);
    tablero->actualizarDesdeLogica(tableroLogica, fichas.getMapaTexturas(), fichaEspeciales);


}
void Juego::marcarNivelCompletado() {
    nivelYaCompletado = true;
}

bool Juego::estaMarcadoComoCompletado() const {
    return nivelYaCompletado;
}
void Juego::irAlSiguienteNivel() {

    if (nivelActual < 3) {
        nivelActual++;
        iniciarNivel(nivelActual);
    }
}
void Juego::prepararNuevoNivel() {

    for (auto& fila : tableroLogica) {
        std::fill(fila.begin(), fila.end(), Celda{});
    }


    generarFichaAleatoria();
    generarFichaAleatoria();
}

const std::vector<std::vector<Celda>>& Juego::obtenerTablero() const {
    if (efectoPortal.absorcionTerminada) {
        return tableroLogica;
    }

    return tableroLogica;
}

int Juego::obtenerValorCelda(int fila, int columna) const {
    return tableroLogica[fila][columna].valor;
}

int Juego::obtenerPuntaje() const {
    return puntaje;
}

void Juego::guardarProgresoDeNivel() {
    puntajeAcumulado += puntaje;
}
const std::vector<std::vector<Celda>>& Juego::getTablero() const {
    return tableroLogica;
}

bool Juego::moverIzquierdaInterno(std::vector<Celda>& fila, int filaIndex, DireccionMovimiento dir,bool invertido ) {
    if (efectoPortal.estaAbsorbiendo() || !efectoPortal.absorcionTerminada)
        return false;

    bool movimiento = false;

    std::vector<Celda> nuevaFila(fila.size(), {0, false, 0});

    int segmentoInicio = 0;

    while (segmentoInicio < (int)fila.size()) {

        int segmentoFin = segmentoInicio;
        while (segmentoFin < (int)fila.size() && !fila[segmentoFin].congelada && fila[segmentoFin].turnosCongelada == 0) {
            segmentoFin++;
        }


        std::vector<Celda> segmentoFichas;
        std::vector<std::pair<Celda, int>> segmentoFichasConIndices;
        for (int i = segmentoInicio; i < segmentoFin; ++i) {
            if (fila[i].valor != 0)
                segmentoFichas.push_back(fila[i]);
        }

        std::vector<Celda> segmentoProcesado;
        Celda anterior = {0, false, 0};
        int indiceEnFila = 0;
        int anteriorIndiceReal = -1;
        int pos = 0;
        int indiceReal;
        int indiceFinal = pos;


        for (auto& actual : segmentoFichas) {
            if (anterior.valor == 0) {
                anterior = actual;
                anteriorIndiceReal = segmentoInicio + pos;

            }
            else if (anterior.valor == actual.valor) {
                if (anterior.valor == -1) {
                    if (invertido) {
                        indiceFinal = (int)fila.size() - 1 - anteriorIndiceReal;
                    } else {
                        indiceFinal = anteriorIndiceReal;
                    }
                    int posReal = invertido ? ((int)fila.size() - 1 - anteriorIndiceReal) : anteriorIndiceReal;

                    if (dir == HORIZONTAL) {
                        ultimaExplosionLogica = {filaIndex, posReal};
                    } else {
                        ultimaExplosionLogica = {posReal, filaIndex};
                    }
                        explosionActiva = true;
                        fichasEliminadas = false;

                        sf::Vector2f posVisual = calcularPosicionVisual(ultimaExplosionLogica.x, ultimaExplosionLogica.y);
                        fichaEspeciales->activarExplosionEn(posVisual);

                        segmentoProcesado.push_back({0, false, 0});
                        anterior = {0, false, 0};
                        ++pos;

                } else {
                    Celda fusionada = anterior;
                    fusionada.valor *= 2;
                    fusionada.congelada = false;
                    fusionada.turnosCongelada = 0;
                    segmentoProcesado.push_back(fusionada);
                    puntaje += fusionada.valor;
                }
                anterior = {0, false, 0};
                ++pos;
            }
            else {
                segmentoProcesado.push_back(anterior);
                anterior = actual;
                anteriorIndiceReal = indiceEnFila;
                ++pos;
            }
            ++indiceEnFila;

        }
        if (anterior.valor != 0) {
            segmentoProcesado.push_back(anterior);
        }

        while (segmentoProcesado.size() < (segmentoFin - segmentoInicio)) {
            segmentoProcesado.push_back({0, false, 0});
        }

        for (int i = segmentoInicio; i < segmentoFin; ++i) {
            if (!(fila[i] == segmentoProcesado[i - segmentoInicio]))
                movimiento = true;
            nuevaFila[i] = segmentoProcesado[i - segmentoInicio];
        }

        if (segmentoFin < (int)fila.size()) {
            nuevaFila[segmentoFin] = fila[segmentoFin];
        }

        segmentoInicio = segmentoFin + 1;
    }

    if (fila != nuevaFila) {
        fila = nuevaFila;
        movimiento = true;
    }

    return movimiento;
}
void Juego::moverIzquierda() {
    DireccionMovimiento dirMovimientoActual = HORIZONTAL;
    guardarEstadoAnterior();
    if (animacionesEnCurso) return ;
    if (explosionActiva) return;

    for (auto& fila : tableroLogica) {
        for (auto& celda : fila) {
            if (celda.congelada && celda.turnosCongelada > 0) {
                celda.turnosCongelada--;
                if (celda.turnosCongelada == 0)
                    celda.congelada = false;
            }
        }
    }
    if (explosionActiva) return;

    estadoAnterior = tableroLogica;
    puntajeAnterior = puntaje;

    bool movido = false;
    for (int i = 0; i < tableroLogica.size(); ++i)
        movido |= moverIzquierdaInterno(tableroLogica[i], i, HORIZONTAL);

    if (explosionActiva && !fichasEliminadas) {
            activarSacudida();
        }

    if (movido) {
        for (int i = 0; i < tableroLogica.size(); ++i) {
            for (int j = 0; j < tableroLogica[i].size(); ++j) {
                if (tableroLogica[i][j].valor != 0) {
                    tableroLogica[i][j].posicionObjetivo = calcularPosicionVisual(i, j);
                }
            }
        }
        generarFichaAleatoria();
        if(nivelActual>=1){
            fichaEspeciales->registrarMovimiento();
        }
        if (nivelActual >= 2) {

            fichaEspeciales->registrarMovimientoCarga();
            efectoPortal.registrarMovimiento();
        }
    }
    if (movido)
        generarFichaAleatoria();
    if (fichaEspeciales->explosionFinalizada() && !fichasEliminadas) {
        eliminarFichasAlrededorUltimaExplosion();
        fichasEliminadas = true;
        explosionActiva = false;
    }

}
void Juego::moverDerecha() {
     dirMovimientoActual = HORIZONTAL;
    guardarEstadoAnterior();
    if (animacionesEnCurso) return ;
    if (explosionActiva) return;


    for (auto& fila : tableroLogica) {
        for (auto& celda : fila) {
            if (celda.congelada && celda.turnosCongelada > 0) {
                celda.turnosCongelada--;
                if (celda.turnosCongelada == 0)
                    celda.congelada = false;
            }
        }
    }
    if (explosionActiva) return;

    estadoAnterior = tableroLogica;
    puntajeAnterior = puntaje;

    bool movido = false;

    for (int i = 0; i < tableroLogica.size(); ++i) {
        std::reverse(tableroLogica[i].begin(), tableroLogica[i].end());

        movido |= moverIzquierdaInterno(tableroLogica[i], i, HORIZONTAL, true);

        std::reverse(tableroLogica[i].begin(), tableroLogica[i].end());

    }

        if (explosionActiva && !fichasEliminadas) {
            int ancho = tableroLogica[0].size();
            sf::Vector2f posVisual = calcularPosicionVisual(ultimaExplosionLogica.x, ultimaExplosionLogica.y);
            activarSacudida();
        }
    if (movido) {
        for (int i = 0; i < tableroLogica.size(); ++i) {
            for (int j = 0; j < tableroLogica[i].size(); ++j) {
                if (tableroLogica[i][j].valor != 0) {
                    tableroLogica[i][j].posicionObjetivo = calcularPosicionVisual(i, j);
                }
            }
        }
        generarFichaAleatoria();
            if(nivelActual>=1){
            fichaEspeciales->registrarMovimiento();
        }
        if (nivelActual >= 2) {

            fichaEspeciales->registrarMovimientoCarga();
            efectoPortal.registrarMovimiento();
        }
    }


    if (movido)
        generarFichaAleatoria();
    if (fichaEspeciales->explosionFinalizada() && !fichasEliminadas) {
        eliminarFichasAlrededorUltimaExplosion();
        fichasEliminadas = true;
        explosionActiva = false;
    }
}

void Juego::moverArriba() {
    dirMovimientoActual = VERTICAL;
     guardarEstadoAnterior();
     if (animacionesEnCurso) return ;
     if (explosionActiva) return;


    for (auto& fila : tableroLogica) {
        for (auto& celda : fila) {
            if (celda.congelada && celda.turnosCongelada > 0) {
                celda.turnosCongelada--;
                if (celda.turnosCongelada == 0)
                    celda.congelada = false;
            }
        }
    }
    if (explosionActiva) return;

    estadoAnterior = tableroLogica;
    puntajeAnterior = puntaje;

    bool movido = false;
    int N = tableroLogica.size();

    for (int col = 0; col < N; ++col) {
        std::vector<Celda> columna;
        for (int fila = 0; fila < N; ++fila)
            columna.push_back(tableroLogica[fila][col]);


        movido |= moverIzquierdaInterno(columna, col, VERTICAL, false);


        for (int fila = 0; fila < N; ++fila)
            tableroLogica[fila][col] = columna[fila];


    }
    if (explosionActiva && !fichasEliminadas) {
            int filas = tableroLogica[0].size();
             ultimaExplosionLogica.x = filas - 1 - ultimaExplosionLogica.x;
            sf::Vector2f posVisual = calcularPosicionVisual(ultimaExplosionLogica.x, ultimaExplosionLogica.y);
            activarSacudida();
        }
    if (movido) {
   for (int i = 0; i < tableroLogica.size(); ++i) {
        for (int j = 0; j < tableroLogica[i].size(); ++j) {
            if (tableroLogica[i][j].valor != 0) {
                tableroLogica[i][j].posicionObjetivo = calcularPosicionVisual(i, j);
            }
        }
    }
    generarFichaAleatoria();
        if(nivelActual>=1){
            fichaEspeciales->registrarMovimiento();
        }
        if (nivelActual >= 2) {

            fichaEspeciales->registrarMovimientoCarga();
            efectoPortal.registrarMovimiento();
        }
    }

    if (movido)
        generarFichaAleatoria();
    if (fichaEspeciales->explosionFinalizada() && !fichasEliminadas) {
        eliminarFichasAlrededorUltimaExplosion();
        fichasEliminadas = true;
        explosionActiva = false;
    }
}
void Juego::moverAbajo() {
    dirMovimientoActual = VERTICAL;

    guardarEstadoAnterior();
    if (animacionesEnCurso) return ;
    if (explosionActiva) return;


        for (auto& fila : tableroLogica) {
            for (auto& celda : fila) {
                if (celda.congelada && celda.turnosCongelada > 0) {
                    celda.turnosCongelada--;
                    if (celda.turnosCongelada == 0)
                        celda.congelada = false;
                }
            }
        }
        if (explosionActiva) return;

        estadoAnterior = tableroLogica;
        puntajeAnterior = puntaje;

        bool movido = false;
        int N = tableroLogica.size();

        for (int col = 0; col < N; ++col) {
            std::vector<Celda> columna;
            for (int fila = 0; fila < N; ++fila)
                columna.push_back(tableroLogica[fila][col]);

            std::reverse(columna.begin(), columna.end());

            movido |= moverIzquierdaInterno(columna, col, VERTICAL,true);

            std::reverse(columna.begin(), columna.end());

            for (int fila = 0; fila < N; ++fila)
                tableroLogica[fila][col] = columna[fila];
        }

        if (explosionActiva && !fichasEliminadas) {
            int filas = tableroLogica[0].size();
            sf::Vector2f posVisual = calcularPosicionVisual(ultimaExplosionLogica.x, ultimaExplosionLogica.y);
            activarSacudida();
        }
        if (movido) {
        for (int i = 0; i < tableroLogica.size(); ++i) {
            for (int j = 0; j < tableroLogica[i].size(); ++j) {
                if (tableroLogica[i][j].valor != 0) {
                    tableroLogica[i][j].posicionObjetivo = calcularPosicionVisual(i, j);
                }
            }
        }
        generarFichaAleatoria();
            if(nivelActual>=1){
                fichaEspeciales->registrarMovimiento();
            }
            if (nivelActual >= 2) {

                fichaEspeciales->registrarMovimientoCarga();
                efectoPortal.registrarMovimiento();
            }
        }

        if (movido)
            generarFichaAleatoria();
        if (fichaEspeciales->explosionFinalizada() && !fichasEliminadas) {
            eliminarFichasAlrededorUltimaExplosion();
            fichasEliminadas = true;
            explosionActiva = false;
        }
}
int Juego::getPuntaje() const {
    return puntaje;
}
int Juego::getNivelActual() const {
    return nivelActual;
}
void Juego::reiniciarPuntaje() {
    puntaje = 0;
}
void Juego::iniciarTemporizador() {
        tiempoJugado = 0.f;
        relojTemporizador.restart();
        temporizadorActivo = true;
}
void Juego::pausarTemporizador() {
    if (temporizadorActivo) {
        tiempoJugado += relojTemporizador.getElapsedTime().asSeconds();
        temporizadorActivo = false;
    }
}
void Juego::reanudarTemporizador() {

    if (!temporizadorActivo) {
        relojTemporizador.restart();
        temporizadorActivo = true;

    }
}
void Juego::reiniciarTemporizador() {
        tiempoJugado = 0.f;
        relojTemporizador.restart();
}

float Juego::getTiempoJugado()  {
    if (temporizadorActivo) {
        return tiempoJugado + relojTemporizador.getElapsedTime().asSeconds();
    } else {
        return tiempoJugado;
    }
}

void Juego::guardarPartida() {
    std::ofstream archivo("partida_guardada.txt");
    if (archivo.is_open()) {
        tiempoJugado = getTiempoJugado();
        archivo << nivelActual << std::endl;
        archivo << puntaje << std::endl;
        archivo << tiempoJugado << std::endl;


        auto matriz = tableroLogica;
        int tamanio = matriz.size();
        archivo << tamanio << "\n";

        for (int fila = 0; fila < tamanio; ++fila) {
            for (int col = 0; col < tamanio; ++col) {
                const Celda& celda = matriz[fila][col];
                archivo << celda.valor << " " << celda.congelada << " " << celda.turnosCongelada << " ";
            }
            archivo << "\n";
        }


        archivo << coheteActivo << "\n";
        archivo << misilEnAnimacion << "\n";
        archivo << mezclaDisponible << "\n";
        archivo << efectoPortal.absorcionTerminada << "\n";
        archivo << fichaEspeciales->contadorMovimientos << "\n";
        archivo << fichaEspeciales->frameCarga << "\n";
        archivo << efectoPortal.movimientos << "\n";
    }
}
bool Juego::cargarPartida() {
    std::ifstream archivo("partida_guardada.txt");
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo de partida" << std::endl;
        return false;
    }

    int tamanio;
    archivo >> nivelActual >> puntaje >> tiempoJugado >> tamanio;
    iniciarNivel(nivelActual);
    if (nivelActual == 1) objetivoNivel = 64;
    else if (nivelActual == 2) objetivoNivel = 128;
    else if (nivelActual == 3) objetivoNivel = 256;
    else objetivoNivel = 2048;

    tablero->configurarParaNivel(nivelActual);

    std::vector<std::vector<Celda>> matriz(tamanio, std::vector<Celda>(tamanio));
    for (int fila = 0; fila < tamanio; ++fila) {
        for (int col = 0; col < tamanio; ++col) {
             archivo >> matriz[fila][col].valor
                     >> matriz[fila][col].congelada
                     >> matriz[fila][col].turnosCongelada;

             matriz[fila][col].posicionObjetivo = calcularPosicionVisual(fila, col);
                matriz[fila][col].posicionVisual = calcularPosicionVisual(fila, col);
             if (fila == 0 && col == 0) {
            std::cout << "Posicion objetivo (0,0): " << matriz[fila][col].posicionObjetivo.x << ", " << matriz[fila][col].posicionObjetivo.y << std::endl;
        }
        }
    }

    tableroLogica = matriz;
    for (int fila = 0; fila < tableroLogica.size(); ++fila) {
        for (int col = 0; col < tableroLogica[fila].size(); ++col) {
            tableroLogica[fila][col].posicionVisual = tableroLogica[fila][col].posicionObjetivo;
            std::cout << "Celda[" << fila << "][" << col << "] valor: " << tableroLogica[fila][col].valor << "\n";
        }
    }

    tablero->actualizarDesdeLogica(tableroLogica, fichas.getMapaTexturas(), fichaEspeciales);
    std::cout << "ajusteVertical = " << ajusteVertical << std::endl;
    std::cout << "origenTablero.y = " << tablero->origenTablero.y << std::endl;
    std::cout << "margen = " << tablero->margen << std::endl;
    std::cout << "tamanioCelda = " << tablero->tamanioCelda << std::endl;


        archivo >> coheteActivo;
        archivo >> misilEnAnimacion;
        archivo >> mezclaDisponible;
        archivo >> efectoPortal.absorcionTerminada;
        archivo >> fichaEspeciales->contadorMovimientos;
        fichaEspeciales->coheteListo = (fichaEspeciales->contadorMovimientos >= fichaEspeciales->maxMovimientos);
        fichaEspeciales->mostrarBotonCohete = fichaEspeciales->coheteListo;

        int indice = std::min(fichaEspeciales->contadorMovimientos, fichaEspeciales->maxMovimientos);
        fichaEspeciales->spriteBarraCohete.setTextureRect(sf::IntRect(0, fichaEspeciales->altoFragmento * indice, fichaEspeciales->anchoFragmento, fichaEspeciales->altoFragmento));

        archivo >> fichaEspeciales->frameCarga;
        fichaEspeciales->frameCarga = std::min(fichaEspeciales->frameCarga, fichaEspeciales->totalFramesCarga - 1);
        fichaEspeciales->spriteBarraMezcla.setTextureRect(sf::IntRect(fichaEspeciales->frameCarga * fichaEspeciales->anchoFrameCarga, 0, fichaEspeciales->anchoFrameCarga, fichaEspeciales->altoFrameCarga));
        fichaEspeciales->mostrarBotonMezcla = (fichaEspeciales->frameCarga == fichaEspeciales->totalFramesCarga - 1);
        archivo >> efectoPortal.movimientos;
        efectoPortal.spriteBarra.setTextureRect(

        sf::IntRect(efectoPortal.movimientos * efectoPortal.anchoFrameBarra, 0,
                    efectoPortal.anchoFrameBarra, efectoPortal.altoFrameBarra));


        fichaEspeciales->mostrarBotonDeshacer = (nivelActual >= 2);


    return true;
}
void Juego::guardarCheckpoint() {
  cpNivel    = nivelActual;
  cpTablero  = tableroLogica;
  cpPuntaje  = puntaje;
  cpTiempo   = getTiempoJugado();
}
void Juego::restaurarCheckpoint() {
  nivelActual    = cpNivel;
  tableroLogica  = cpTablero;
  puntaje        = cpPuntaje;

  tiempoJugado   = cpTiempo;
  reiniciarTemporizador();
  temporizadorActivo = true;

  tablero->configurarParaNivel(nivelActual);
}
void Juego::nuevaPartida() {

    puntaje = 0;
    puntajeAcumulado = 0;
    tiempoJugado = 0.f;
    nivelActual = 1;
    juegoTerminado = false;
    temporizadorActivo = false;
    tableroLogica.assign(4, std::vector<Celda>(4));


    reloj.restart();
    borrarPartidaGuardada();

    puntajeInicioNivel = 0;

    generarFichaAleatoria();
    generarFichaAleatoria();
    iniciarNivel(1);
}
void Juego::borrarPartidaGuardada() {
    std::remove("partida_guardada.txt");
}
Tablero* Juego::obtenerTableroGrafico() const {
    return tablero;
}
//BOMBA
sf::Vector2f Juego::calcularPosicionVisual(int fila, int col) const {

     return sf::Vector2f(
        tablero->origenTablero.x + tablero->margen + col * (tablero->tamanioCelda + tablero->margen),
        tablero->origenTablero.y + tablero->margen + fila * (tablero->tamanioCelda + tablero->margen)
    );
}

void Juego::dibujarBombas(sf::RenderWindow& ventana, float deltaTime) {
    sf::Sprite sprite;
    const int ANCHO_FRAME = 129;
    const int ALTO_FRAME  = 149;

    for (int fila = 0; fila < tableroLogica.size(); ++fila) {
        for (int col = 0; col < tableroLogica[fila].size(); ++col) {
            if (tableroLogica[fila][col].valor == -1) {
                sf::Sprite sprite;
                sprite.setTexture(fichaEspeciales->getBombaTexture());


                sprite.setTextureRect(sf::IntRect(0, 0, ANCHO_FRAME, ALTO_FRAME));


                float escalaX = static_cast<float>(tablero->tamanioCelda) / ANCHO_FRAME;
                float escalaY = static_cast<float>(tablero->tamanioCelda) / ALTO_FRAME;
                sprite.setScale(escalaX, escalaY);


                float x = tablero->origenTablero.x + tablero->margen + col * (tablero->tamanioCelda + tablero->margen);
                float y = tablero->origenTablero.y + tablero->margen + fila * (tablero->tamanioCelda + tablero->margen);
                sprite.setPosition(x, y);
            }
        }
    }

    if (explosionActiva && fichaEspeciales->explosionFinalizada()) {
        eliminarFichasAlrededorUltimaExplosion();
        fichasEliminadas = true;
        explosionActiva = false;
    }
}

void Juego::dibujarExplosiones(sf::RenderWindow& ventana, float deltaTime) {
    fichaEspeciales->dibujarExplosion(ventana, deltaTime);

}
void Juego::eliminarFichasAlrededorUltimaExplosion() {
    int n = tableroLogica.size();
    int fila = ultimaExplosionLogica.x;
    int col = ultimaExplosionLogica.y;

    for (int f = fila - 1; f <= fila + 1; ++f) {
        for (int c = col - 1; c <= col + 1; ++c) {
            if (f >= 0 && f < n && c >= 0 && c < n) {
                tableroLogica[f][c] = Celda{};
            }
        }
    }
}
void Juego::actualizarSacudidaPantalla(sf::RenderWindow& ventana, float deltaTime) {
    if (!sacudirPantalla) return;

    tiempoSacudidaRestante -= deltaTime;
    if (tiempoSacudidaRestante > 0.f) {
        float offsetX = static_cast<float>(std::rand() % 11 - 5);
        float offsetY = static_cast<float>(std::rand() % 11 - 5);
        sf::View view = ventana.getDefaultView();
        view.move(offsetX * intensidadSacudida, offsetY * intensidadSacudida);
        ventana.setView(view);
    } else {
        sacudirPantalla = false;
        ventana.setView(ventana.getDefaultView());
    }

}
void Juego::activarSacudida() {
    sacudirPantalla = true;
    tiempoSacudidaRestante = 0.3f;
    intensidadSacudida = 2.f;
}
//RETROCEDER
void Juego::deshacerMovimiento() {
    if (!estadoAnterior.empty()) {
        tableroLogica = estadoAnterior;
        puntaje = puntajeAnterior;
    }
}
void Juego::guardarEstadoAnterior() {
    estadoAnterior = tableroLogica;
    puntajeAnterior = puntaje;
}
void Juego::restarPuntos(int cantidad) {
    puntaje -= cantidad;
    if (puntaje < 0) puntaje = 0;
}
//HIELO
void Juego::dibujarHielos(sf::RenderWindow& ventana, Fichas& fichas) {
    for (int fila = 0; fila < tableroLogica.size(); ++fila) {
        for (int col = 0; col < tableroLogica[fila].size(); ++col) {
            const Celda& celda = tableroLogica[fila][col];
            if (!celda.congelada) continue;

            sf::Vector2f pos = calcularPosicionVisual(fila, col);

            sf::Sprite hielo;
            hielo.setTexture(fichas.getFichaHielo());
            hielo.setPosition(pos);

            auto size = hielo.getTexture()->getSize();
            if (size.x > 0 && size.y > 0) {
                float escalaX = tablero->tamanioCelda / static_cast<float>(size.x);
                float escalaY = tablero->tamanioCelda / static_cast<float>(size.y);
                hielo.setScale(escalaX, escalaY);
            }
            ventana.draw(hielo);
        }
    }
}

//COHETE
void Juego::activarCohete() {
    if (puedeUsarCohete()) {
        puntaje -= obtenerCostoCohete();
        coheteActivo = true;
    } else {
        std::cout << "No tienes puntos suficientes para usar el cohete.\n";
    }
}
void Juego::actualizarCohete(sf::RenderWindow& ventana,float dt) {

if (!misilEnAnimacion) return;

    static float tiempo = 0.f;
    tiempo += dt;

    if (tiempo > 0.4f) {
        tiempo = 0.f;
        misilEnAnimacion = false;

        guardarEstadoAnterior();
        tableroLogica[celdaObjetivoMisil.x][celdaObjetivoMisil.y] = Celda{};
        fichaEspeciales->usarCohete();
    }
}
void Juego::dibujarCohete(sf::RenderWindow& ventana) {
   if (!coheteActivo && !misilEnAnimacion) return;

    posicionCohete = static_cast<sf::Vector2f>(sf::Mouse::getPosition(ventana));
    sf::Sprite sprite = fichaEspeciales->getBotonCohete();
    sprite.setPosition(posicionCohete);
    ventana.draw(sprite);

    if (misilEnAnimacion) {
    ventana.draw(spriteMisil);
}

}
int Juego::getIndiceSpriteCohete() const {
    return std::min(fichaEspeciales->contadorMovimientos, 5);
}

bool Juego::estaCoheteActivo() const {
    return fichaEspeciales->coheteListo;
}
void Juego::procesarClickCohete(const sf::Vector2i& mousePos) {
    if (!coheteActivo) return;

    int fila = (mousePos.y - tablero->origenTablero.y - tablero->margen) / (tablero->tamanioCelda + tablero->margen);
    int col  = (mousePos.x - tablero->origenTablero.x - tablero->margen) / (tablero->tamanioCelda + tablero->margen);

    if (fila < 0 || fila >= (int)tableroLogica.size() ||
        col < 0 || col >= (int)tableroLogica[0].size() ||
        tableroLogica[fila][col].valor == 0)
        return;
    celdaObjetivoMisil = {fila, col};

    origenMisil = fichaEspeciales->posicionBotonCohete;
    destinoMisil = calcularPosicionCelda(fila, col);


    sf::Vector2f delta = destinoMisil - origenMisil;
    float distancia = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if (distancia < 1.f) distancia = 1.f;

    float angulo = std::atan2(delta.y, delta.x) * 180.f / 3.14159265f;

    sf::Vector2f origenAjustado = fichaEspeciales->posicionBotonCohete;
    origenAjustado.y += spriteMisil.getLocalBounds().height / 2.f;

    spriteMisil.setPosition(origenAjustado);
    spriteMisil.setOrigin(0.f, spriteMisil.getLocalBounds().height / 2.f);
    spriteMisil.setRotation(angulo);

    float escalaX = distancia / static_cast<float>(efectoPortal.texturaMisil.getSize().x);
    spriteMisil.setScale(escalaX, 1.f);

    misilEnAnimacion = true;
    coheteActivo = false;

}
sf::Vector2f Juego::calcularPosicionCelda(int fila, int columna) const {
   float x = tablero->origenTablero.x + tablero->margen + columna * (tablero->tamanioCelda + tablero->margen) + tablero->tamanioCelda / 2.f;
    float y = tablero->origenTablero.y + tablero->margen + fila * (tablero->tamanioCelda + tablero->margen) + tablero->tamanioCelda / 2.f - ajusteVertical;
    return sf::Vector2f(x, y);
}
int Juego::obtenerCostoCohete() const {
    if (nivelActual == 2) return 100;
    else if (nivelActual==3)return 300;
    else return 0;
}
bool Juego::puedeUsarCohete() const {
    int costo = obtenerCostoCohete();
    return (puntaje >= costo) && (puntaje > 0);
}
//MEZCLA

void Juego::activarMezcla() {
    if (!mezclaDisponible) return;
    if (!puedeMezclar()) return;

    puntaje -= obtenerCostoMezclar();

        struct CeldaTemp {
        int valor;
        bool congelada;
        int turnosCongelada;
    };

    std::vector<CeldaTemp> fichasGuardadas;

    for (int fila = 0; fila < tableroLogica.size(); ++fila) {
        for (int col = 0; col < tableroLogica[fila].size(); ++col) {
            int val = tableroLogica[fila][col].valor;
            bool cong = tableroLogica[fila][col].congelada;
            int tCong = tableroLogica[fila][col].turnosCongelada;

            if (val != 0) {
                fichasGuardadas.push_back({val, cong, tCong});

                tableroLogica[fila][col].valor = 0;
                tableroLogica[fila][col].congelada = false;
                tableroLogica[fila][col].turnosCongelada = 0;
            }
        }
    }


    std::random_shuffle(fichasGuardadas.begin(), fichasGuardadas.end());

    std::vector<std::pair<int,int>> posiciones;
    for (int fila = 0; fila < tableroLogica.size(); ++fila) {
        for (int col = 0; col < tableroLogica[fila].size(); ++col) {
            posiciones.emplace_back(fila, col);
        }
    }
    std::random_shuffle(posiciones.begin(), posiciones.end());


    int n = std::min((int)fichasGuardadas.size(), (int)posiciones.size());
    for (int i = 0; i < n; ++i) {
        int f = posiciones[i].first;
        int c = posiciones[i].second;

        tableroLogica[f][c].valor = fichasGuardadas[i].valor;
        tableroLogica[f][c].congelada = fichasGuardadas[i].congelada;
        tableroLogica[f][c].turnosCongelada = fichasGuardadas[i].turnosCongelada;


        tableroLogica[f][c].posicionVisual = calcularPosicionVisual(f, c);
        tableroLogica[f][c].posicionObjetivo = tableroLogica[f][c].posicionVisual;
    }

    mezclaDisponible = false;


    tablero->actualizarDesdeLogica(tableroLogica, fichas.getMapaTexturas(), fichaEspeciales);
}
void Juego::actualizarEstadoMezcla() {
    mezclaDisponible = fichaEspeciales->mostrarBotonMezcla;
}
int Juego::obtenerCostoMezclar() const {
    return 15;
}
bool Juego::puedeMezclar() const {
    return puntaje >= obtenerCostoMezclar();
}
//PORTAL
void Juego::actualizar(float dt) {
    static bool absorcionCompletadaAntes = false;
    bool absorcionCompletadaAhora = efectoPortal.estaAbsorbiendo();


    if (absorcionCompletadaAntes && !absorcionCompletadaAhora) {

        for (auto& fila : tableroLogica) {
            for (auto& celda : fila) {
                if (celda.valor != -1) {
                    celda.valor = 0;
                }
            }
        }
        for (auto& fila : tableroLogica) {
            for (auto& celda : fila) {

                if (celda.valor == -1) {
                    celda.valor = 0;
                }

                celda.congelada = false;
                celda.turnosCongelada = 0;
            }
        }

        if (fichaEspeciales != nullptr) {
            fichaEspeciales->explosionesActivas.clear();
            explosionActiva = false;
            fichasEliminadas = false;
            ultimaExplosionVisual = sf::Vector2f(0.f, 0.f);
            ultimaExplosionLogica = sf::Vector2i(0, 0);
        }

        tablero->limpiarFichas();
        tablero->actualizarDesdeLogica(tableroLogica, fichas.getMapaTexturas(),fichaEspeciales);
        generarFichaAleatoria();

        puntaje -= obtenerCostoPortal();
        if (puntaje < 0) puntaje = 0;

        efectoPortal.reiniciar();
        efectoPortal.absorcionTerminada = true;
    }

    absorcionCompletadaAntes = absorcionCompletadaAhora;
    efectoPortal.actualizar(dt);

}
void Juego::limpiarTablero() {
     for (auto& fila : tableroLogica) {
        for (auto& celda : fila) {
            celda.valor = 0;
            celda.congelada = false;
            celda.turnosCongelada = 0;
        }
    }

}
int Juego::obtenerCostoPortal() const {
    return 500;
}
bool Juego::puedeUsarPortal() const {
    return puntaje >= obtenerCostoPortal();
}
//MOVIMIENTO DE FICHAS
void Juego::actualizarAnimaciones(float deltaTime) {
    float velocidad = 260.f;
    animacionesEnCurso = false;
    for (int i = 0; i < tableroLogica.size(); ++i) {
        for (int j = 0; j < tableroLogica[i].size(); ++j) {
            Celda& celda = tableroLogica[i][j];


            if (celda.valor != 0 && celda.posicionVisual != celda.posicionObjetivo) {
                animacionesEnCurso = true;

            }

            sf::Vector2f direccion = celda.posicionObjetivo - celda.posicionVisual;
            if (std::abs(direccion.x) > 1.f) {
                direccion.y = 0.f;
            } else if (std::abs(direccion.y) > 1.f) {
                direccion.x = 0.f;
            }
            float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

            if (distancia < velocidad * deltaTime) {
                celda.posicionVisual = celda.posicionObjetivo;
            } else {
                direccion /= distancia;
                celda.posicionVisual += direccion * velocidad * deltaTime;
            }

            if (celda.valorFusionado > 0) {
                celda.tiempoFusion += deltaTime;
                if (celda.tiempoFusion >= 0.3f) {
                    celda.valor = celda.valorFusionado;
                    celda.valorFusionado = 0;
                    celda.tiempoFusion = 0.f;
                } else {
                    animacionesEnCurso = true;
                }
            }
            if (fichaHieloPendiente && celda.congelada && celda.posicionVisual == celda.posicionObjetivo) {
                    fichaEspeciales->activarPantallaHielo();
                    fichaHieloPendiente = false;
            }
            if (celda.congelada) {
                celda.posicionVisual = celda.posicionObjetivo;
                continue;
            }

        }
    }
    if (!animacionesEnCurso && hayFichaPendiente) {
        tableroLogica[posFichaPendiente.x][posFichaPendiente.y] = fichaPendiente;
        hayFichaPendiente = false;
        tablero->actualizarDesdeLogica(tableroLogica, fichas.getMapaTexturas(), fichaEspeciales);
    }


}
