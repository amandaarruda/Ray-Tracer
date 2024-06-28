#include "environment.h"

// Implementação da classe Environment

// Construtor da classe Environment que inicializa o ambiente com uma luz ambiente específica.
Environment::Environment(const glm::vec3& ambientLight)
    : ambientLight(ambientLight) {}

// Método getter para obter a luz ambiente atual do ambiente.
glm::vec3 Environment::getAmbientLight() const {
    return ambientLight;
}
