#include "Estados.h"
#include "Tablero.h"
#include "Fichas.h"

Tablero::Tablero(const sf::Vector2u& tamVentana) {
    if (!fondoTexture.loadFromFile("resourses/fondojuegoo.png")) {
        std::cerr << "Error al cargar la imagen de fondo del juego.\n";
    }
    fondoSprite.setTexture(fondoTexture);
    fondoSprite.setScale(
        static_cast<float>(tamVentana.x) / fondoTexture.getSize().x,
        static_cast<float>(tamVentana.y) / fondoTexture.getSize().y
    );

    if (!tableroTexture.loadFromFile("resourses/tablero.png")) {
        std::cerr << "Error al cargar imagen del tablero\n";
    }
    tableroSprite.setTexture(tableroTexture);

    const int columnas = 4;
    const int filas = 4;
    margen = 16.f;
    const float anchoTablero = tableroTexture.getSize().x;
    const float altoTablero = tableroTexture.getSize().y;

    tamanioCelda = (anchoTablero - (margen * (columnas + 1))) / columnas;

    float anchoDeseado = 400.f;
    float altoDeseado = 400.f;
    sf::Vector2u sizeOriginal = tableroTexture.getSize();
    float escalaX = anchoDeseado / sizeOriginal.x;
    float escalaY = altoDeseado / sizeOriginal.y;
    tableroSprite.setScale(escalaX, escalaY);


    origenTablero = sf::Vector2f(370, 255);
    tableroSprite.setPosition(origenTablero);

    margen = 16.f;

    tamanioCelda *= escalaX;
    margen *= escalaX;

    if (!tableroTextureNivel2.loadFromFile("resourses/tablero5x5.png")) {
    std::cerr << "Error al cargar tablero nivel 2\n";
    }
    if (!tableroTextureNivel3.loadFromFile("resourses/tablero6x6.png")) {
        std::cerr << "Error al cargar tablero nivel 3\n";
    }
}

void Tablero::actualizarDesdeLogica(const std::vector<std::vector<Celda>>& matriz, const std::map<int, sf::Texture>& fichas,
    FichaEspeciales* fichaEspeciales) {

    fichasGraficas.clear();
    bombasGraficas.clear();

    for (int fila = 0; fila < matriz.size(); ++fila) {
        for (int col = 0; col < matriz[fila].size(); ++col) {
            int valor = matriz[fila][col].valor;


            if (valor > 0 && fichas.count(valor)) {
                sf::Sprite ficha;
                ficha.setTexture(fichas.at(valor));

                sf::Vector2u sizeOriginal = ficha.getTexture()->getSize();
                float escalaX = tamanioCelda / sizeOriginal.x;
                float escalaY = tamanioCelda / sizeOriginal.y;
                ficha.setScale(escalaX, escalaY);
                ficha.setPosition(matriz[fila][col].posicionVisual);
                fichasGraficas.push_back(ficha);

            }else if (valor == -1 && fichaEspeciales) {

            sf::Sprite bomba;
            const int ANCHO_FRAME = 129;
            const int ALTO_FRAME  = 149;

            bomba.setTexture(fichaEspeciales->getBombaTexture());
            bomba.setTextureRect(sf::IntRect(0, 0, ANCHO_FRAME, ALTO_FRAME));

            float escalaX = static_cast<float>(tamanioCelda) / ANCHO_FRAME;
            float escalaY = static_cast<float>(tamanioCelda) / ALTO_FRAME;

            bomba.setScale(escalaX, escalaY);

            bomba.setPosition(matriz[fila][col].posicionVisual);

            fichasGraficas.push_back(bomba);
        }

        }
    }


}


void Tablero::configurarParaNivel(int nivel) {
    if (nivel == 1) {
        filas = columnas = 4;
        tableroSprite.setTexture(tableroTexture);
    } else if (nivel == 2) {
        filas = columnas = 5;
        tableroSprite.setTexture(tableroTextureNivel2);
    } else if (nivel == 3) {
        filas = columnas = 6;
        tableroSprite.setTexture(tableroTextureNivel3);
    }

    float anchoDeseado = 400.f;
    float altoDeseado = 400.f;
    sf::Vector2u sizeOriginal = tableroSprite.getTexture()->getSize();
    float escalaX = anchoDeseado / sizeOriginal.x;
    float escalaY = altoDeseado / sizeOriginal.y;

    tableroSprite.setScale(escalaX, escalaY);

    float anchoTablero = sizeOriginal.x * escalaX;

    margen = 16.f * escalaX;
    tamanioCelda = (anchoTablero - (margen * (columnas + 1))) / columnas;

    tableroSprite.setPosition(origenTablero);

}
void Tablero::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(fondoSprite);
    ventana.draw(tableroSprite);

    for (const auto& ficha : fichasGraficas){
        ventana.draw(ficha);}
    for (const auto& bomba : bombasGraficas) {
        ventana.draw(bomba);
    }
}



void Tablero::moverArriba() {

}

void Tablero::moverAbajo() {

}

void Tablero::moverIzquierda() {

}

void Tablero::moverDerecha() {

}
void Tablero::limpiarFichas() {
    std::cout << "Limpiando fichas visuales...\n";
    fichasGraficas.clear();
}

