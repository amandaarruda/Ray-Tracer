#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../../External/glm/glm.hpp"

class Environment {
public:
    // Construtor da classe Environment
    // O construtor recebe a luz ambiente
    Environment(const glm::vec3& ambientLight);
    // Metodo getAmbientLight da classe Environment
    // A luz ambiente é representada como uma cor RGB, com componentes de intervalo 0 a 255.
    glm::vec3 getAmbientLight() const;

private:
    glm::vec3 ambientLight;
};

#endif // ENVIRONMENT_H