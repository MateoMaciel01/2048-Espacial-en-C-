#pragma once
#include <vector>
#include "FichaEspeciales.h"
#include "Fichas.h"
#include "Efectos.h"
#include "Celda.h"

enum class Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };
enum DireccionMovimiento { HORIZONTAL, VERTICAL };
class Tablero;
class Efectos;

class Juego {
private:
    Tablero* tablero;
    Fichas fichas;
    Celda celda;


    std::vector<std::vector<Celda>> tableroLogica;
    bool juegoTerminado;
    int puntaje;
    int puntajeAcumulado=0;
    int puntajeInicioNivel = 0;

    int objetivoNivel = 0;
    bool nivelYaCompletado = false;

    int cpNivel;
    std::vector<std::vector<Celda>> cpTablero;
    int cpPuntaje;
    float cpTiempo;

    //BOMBA
    bool bombaActiva = false;
    int puntajeAnterior = 0;
    //PORTAL

    //COHETE
    float largoMisil = 0.f;
    float ajusteVertical = 10.f;

    int obtenerCostoCohete() const;

    DireccionMovimiento dirMovimientoActual = HORIZONTAL;



public:

     Juego(Tablero* tableroExtern);
    ~Juego();
    Efectos efectoPortal;
    Tablero* obtenerTableroGrafico() const;

    void iniciarNivel(int nivel);
    void irAlSiguienteNivel();
    bool nivelCompletado() const;
    void guardarProgresoDeNivel();
    void cargarProgresoDeNivel();
    void prepararNuevoNivel();
    bool hayMovimientosDisponibles() const;
    void reiniciar();
    bool estaMarcadoComoCompletado() const;
    void marcarNivelCompletado() ;


    void generarFichaAleatoria();


    const std::vector<std::vector<Celda>>& obtenerTablero() const;
    const std::vector<std::vector<Celda>>& getTablero() const;

    int obtenerValorCelda(int fila, int columna) const;
    int obtenerPuntaje() const;
    int nivelActual;
    int getPuntaje() const;
    void reiniciarPuntaje();

    int getNivelActual() const;

    void moverArriba();
    void moverAbajo();
    void moverIzquierda();
    void moverDerecha();

    bool moverIzquierdaInterno(std::vector<Celda>& fila, int filaIndex, DireccionMovimiento dir = HORIZONTAL,bool invertido = false);

    void iniciarTemporizador();

    void reiniciarTemporizador() ;

    float getTiempoJugado() ;
    void guardarPartida();
    bool cargarPartida();
    float tiempoJugado = 0.f;

    void pausarTemporizador();
    void reanudarTemporizador();
    sf::Clock reloj;
    sf::Clock relojTemporizador;
    sf::Clock relojDeltaTime;
    bool temporizadorActivo = false;

    void guardarCheckpoint();
    void restaurarCheckpoint();

    void nuevaPartida();
    void borrarPartidaGuardada();


    //BOMBA
    FichaEspeciales* fichaEspeciales = nullptr;
    sf::Vector2f calcularPosicionVisual(int fila, int col) const ;
    void dibujarBombas(sf::RenderWindow& ventana, float deltaTime);
    void dibujarExplosiones(sf::RenderWindow& ventana, float deltaTime);
    bool explosionActiva = false;
    void eliminarFichasAlrededorUltimaExplosion();
    sf::Vector2f ultimaExplosionVisual;
    sf::Vector2i ultimaExplosionLogica;
    bool fichasEliminadas = false;
     bool movimientoTerminado = false;
    bool explosionPendiente = false;
    sf::Vector2f posicionExplosionPendiente;

    //SACUDIR
    bool sacudirPantalla = false;
    float duracionSacudida = 0.3f;
    float tiempoSacudidaRestante = 0.f;
    float intensidadSacudida = 3.f;
    void actualizarSacudidaPantalla(sf::RenderWindow& ventana, float deltaTime);
    void activarSacudida();

    //RETROCEDER
    std::vector<std::vector<Celda>> estadoAnterior;

    void deshacerMovimiento();
    void setFichaEspeciales(FichaEspeciales* f) { fichaEspeciales = f; };
    void guardarEstadoAnterior();
    void restarPuntos(int cantidad);

    //HIELO
    void dibujarHielos(sf::RenderWindow& ventana, Fichas& fichas);
    bool fichaHieloPendiente = false;

    //COHETE
    bool coheteActivo = false;
    sf::Vector2f posicionCohete;
    void activarCohete();
    void actualizarCohete(sf::RenderWindow& ventana, float dt);
    void dibujarCohete(sf::RenderWindow& ventana);
    int getIndiceSpriteCohete() const;
    bool estaCoheteActivo() const;
    void procesarClickCohete(const sf::Vector2i& mousePos);
    bool puedeUsarCohete() const;

    //MISIL
    bool misilEnAnimacion = false;
    sf::Vector2f origenMisil;
    sf::Vector2f destinoMisil;
    sf::Vector2f direccionMisil;
    sf::Vector2i celdaObjetivoMisil;
    sf::Sprite spriteMisil;
    float velocidadMisil = 600.f;
    sf::Vector2f posicionActualMisil;
    sf::Vector2f calcularPosicionCelda(int fila, int columna) const;

    //MEZCLA
    void activarMezcla();
    bool puedeMezclar() const;
    int obtenerCostoMezclar() const;
    //PORTAL
    void actualizar(float dt);
    void limpiarTablero();
    float tiempoDesdeAbsorcion = 0.f;
    bool mezclaDisponible;
    bool mezclaActiva = false;
    void actualizarEstadoMezcla();
    bool puedeUsarPortal() const;
    int obtenerCostoPortal() const;

    //MOVIMIENTO DE FICHAS
    void actualizarAnimaciones(float deltaTime);
    bool animacionesEnCurso = false;
    Celda fichaPendiente;
    bool hayFichaPendiente = false;
    sf::Vector2i posFichaPendiente;

};
