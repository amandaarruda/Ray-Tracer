#include "light.h"

// Implementação da classe Light

// Construtor da classe Light
// Inicializa a luz com uma posição específica e uma intensidade de luz
Light::Light(const glm::vec3& position, const glm::vec3& intensity)
    : position(position), intensity(intensity) {}

// Método para obter a posição atual da luz
glm::vec3 Light::getPosition() const {
    return position;
}

// Método para obter a intensidade da luz
// Retorna um vetor de 3 dimensões representando a intensidade da luz em RGB
glm::vec3 Light::getIntensity() const {
    return intensity;
}
