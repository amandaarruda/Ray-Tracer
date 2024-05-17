#include "color.h"  // Inclui o arquivo de cabeçalho para a classe color

// Função para escrever uma cor no formato PPM
void write_color(std::ostream& out, color pixel_color) {
    // Converte os valores de cor de ponto flutuante para inteiros no intervalo [0, 255]
    int ir = static_cast<int>(255.99 * pixel_color[0]);
    int ig = static_cast<int>(255.99 * pixel_color[1]);
    int ib = static_cast<int>(255.99 * pixel_color[2]);

    // Escreve os valores de cor no formato PPM seguidos por uma quebra de linha
    out << ir << " " << ig << " " << ib << "\n";
}
