#include "Opciones.h"
#include <fstream>
#include <sstream>



Opciones::Opciones(sf::Vector2f posicion, sf::Vector2f tamano, const std::string& textoStr, const std::string& rutaImagen) {
    if (!rutaImagen.empty()) {
        if (!textura.loadFromFile(rutaImagen)) {
            std::cerr << "Error cargando imagen: " << rutaImagen << std::endl;
        }
        sprite.setTexture(textura);
        sprite.setPosition(posicion);
        sprite.setScale(
            tamano.x / textura.getSize().x,
            tamano.y / textura.getSize().y
        );
    } else {
        sprite.setPosition(posicion);
        sprite.setColor(sf::Color::Blue);
    }

    if (!fuenteT.loadFromFile("Resourses/Font/PressStart2P-Regular.ttf")) {

    }

    textoPuntaje.setFont(fuenteT);
    textoPuntaje.setCharacterSize(24);
    textoPuntaje.setFillColor(sf::Color::White);
    textoPuntaje.setPosition(900, 130);

    textoPuntajeSubmenu.setFont(fuenteT);
    textoPuntajeSubmenu.setCharacterSize(24);
    textoPuntajeSubmenu.setFillColor(sf::Color::White);
    textoPuntajeSubmenu.setPosition(620, 397);


    if (!fuente.loadFromFile("Resourses/Font/PressStart2P-Regular.ttf")) {

    }

    textoTiempo.setFont(fuente);
    textoTiempo.setCharacterSize(24);
    textoTiempo.setFillColor(sf::Color::White);
    textoTiempo.setPosition(900, 70);

    textoTiempoSubmenu.setFont(fuente);
    textoTiempoSubmenu.setCharacterSize(24);
    textoTiempoSubmenu.setFillColor(sf::Color::White);
    textoTiempoSubmenu.setPosition(620, 297);

}
void Opciones::actualizarPuntaje(int nuevoPuntaje) {
    std::string texto = std::to_string(nuevoPuntaje);
    textoPuntaje.setString(texto);
    textoPuntajeSubmenu.setString(texto);
}
void Opciones::actualizarTiempo(float tiempoEnSegundos) {
    int minutos = static_cast<int>(tiempoEnSegundos) / 60;
    int segundos = static_cast<int>(tiempoEnSegundos) % 60;

    std::string tiempoStr = (minutos < 10 ? "0" : "") + std::to_string(minutos) + ":"
                          + (segundos < 10 ? "0" : "") + std::to_string(segundos);

    textoTiempo.setString(tiempoStr);
    textoTiempoSubmenu.setString(tiempoStr);
}

void Opciones::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
    ventana.draw(texto);
    ventana.draw(textoPuntaje);
    ventana.draw(textoTiempo);

}
void Opciones::dibujarSubmenu(sf::RenderWindow& ventana) {

    ventana.draw(textoPuntajeSubmenu);
    ventana.draw(textoTiempoSubmenu);

}

bool Opciones::fueClickeado(sf::Vector2f mousePos) {
    return sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}
//PUNTAJES

void Opciones::cargarTop5() {

    top5Puntajes.clear();
    std::ifstream file(archivoPuntajes);
    if (!file.is_open()) {
        std::ofstream crearArchivo(archivoPuntajes);
        return;
    }
    Puntaje entry;
    while (file >> entry.nombre >> entry.puntos >> entry.tiempo) {
        top5Puntajes.push_back(entry);
    }
    file.close();
    std::sort(top5Puntajes.begin(), top5Puntajes.end(), [](const Puntaje& a, const Puntaje& b) {
        return a.puntos > b.puntos;
    });
    if (top5Puntajes.size() > 5)
        top5Puntajes.resize(5);
}

void Opciones::guardarTop5() {

    std::ofstream file(archivoPuntajes);
    if (!file.is_open()) {
        std::cerr << "Error al abrir archivo para guardar puntajes.\n";
        return;
    }
    for (const auto& e : top5Puntajes) {
        file << e.nombre << " " << e.puntos << " " << e.tiempo << "\n";
    }
}

void Opciones::insertarNuevoPuntaje(const Puntaje& nuevo) {
    top5Puntajes.push_back(nuevo);
    std::sort(top5Puntajes.begin(), top5Puntajes.end(), [](const Puntaje& a, const Puntaje& b) {
        return a.puntos > b.puntos;
    });
    if (top5Puntajes.size() > 5)
        top5Puntajes.resize(5);
    guardarTop5();
}
void Opciones::dibujarTop5(sf::RenderWindow& ventana) {
     for (auto& texto : textosTop5) {
        ventana.draw(texto);
    }
}
void Opciones::actualizarTextosTop5() {
    textosTop5.clear();

    float y = 150.f;
    for (size_t i = 0; i < top5Puntajes.size(); ++i) {
        sf::Text texto;
        texto.setFont(fuente);
        texto.setCharacterSize(28);
        texto.setFillColor(sf::Color::Black);

        const auto& e = top5Puntajes[i];

        int minutos = static_cast<int>(e.tiempo) / 60;
        int segundos = static_cast<int>(e.tiempo) % 60;

        std::ostringstream oss;
        oss << (i + 1) << ". " << e.nombre << " - " << e.puntos << " pts - "
            << (minutos < 10 ? "0" : "") << minutos << ":"
            << (segundos < 10 ? "0" : "") << segundos;

        texto.setString(oss.str());
        texto.setPosition(110.f, y);
        y += 40.f;

        textosTop5.push_back(texto);
    }
}
void Opciones::cargarPuntajes() {
    std::ifstream file("puntajes.txt");
    if (!file.is_open()) {
        std::cout << "No se pudo abrir puntajes.txt" << std::endl;
        return;
    }

    top5Puntajes.clear();
    std::string nombre;
    int puntos;
    float tiempo;

    while (file >> nombre >> puntos >> tiempo) {
        top5Puntajes.push_back({nombre, puntos, tiempo});
    }

    file.close();
}
void Opciones::dibujarSubmenuPuntajes(sf::RenderWindow& ventana) {

if (puntosSpritePtr){
        ventana.draw(*puntosSpritePtr);
    }
    float y = 310.f;
    for (const auto& entrada : top5Puntajes) {
        sf::Text texto;
        texto.setFont(fuente);
        texto.setCharacterSize(20);
        texto.setFillColor(sf::Color::White);

        int minutos = static_cast<int>(entrada.tiempo) / 60;
        int segundos = static_cast<int>(entrada.tiempo) % 60;

        std::ostringstream oss;
        oss << entrada.nombre << " - " << entrada.puntos << " pts - "
            << (minutos < 10 ? "0" : "") << minutos << ":"
            << (segundos < 10 ? "0" : "") << segundos;

        texto.setString(oss.str());
        texto.setPosition(250.f, y);

        ventana.draw(texto);

        y += 40.f;
    }
}
