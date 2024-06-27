#ifndef LIGHT_H
#define LIGHT_H

#include "../../External/glm/glm.hpp" // Inclui o arquivo de cabeçalho para a biblioteca glm

class Light {
public:
    // Construtor da classe Light
    // Inicializa a luz com uma posição específica e uma intensidade de luz
    Light(const glm::vec3& position, const glm::vec3& intensity);

    // Método para obter a posição atual da luz
    glm::vec3 getPosition() const;

    // Método para obter a intensidade da luz
    // Retorna um vetor de 3 dimensões representando a intensidade da luz em RGB
    glm::vec3 getIntensity() const;

private:
    glm::vec3 position;   // Posição da luz no espaço 3D
    glm::vec3 intensity;  // Intensidade da luz em cada canal de cor (RGB)
};

#endif // LIGHT_H
