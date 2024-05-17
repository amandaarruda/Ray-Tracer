#ifndef COLOR_H  // Diretiva de pré-processador para garantir que o arquivo só será incluído uma vez
#define COLOR_H

#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
#include <iostream>  // Inclui o arquivo de cabeçalho para entrada e saída padrão

// Define um alias 'color' para o tipo glm::vec3
using color = glm::vec3;

// Declaração da função para escrever uma cor no formato PPM
void write_color(std::ostream& out, color pixel_color);

#endif  // Fecha a diretiva de pré-processador
