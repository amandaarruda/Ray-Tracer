#include "ray.h"  
#include "../../External/glm/glm.hpp"  

// Construtor padrão da classe ray
ray::ray() {}

// Construtor da classe ray com parâmetros
ray::ray(const glm::vec3 &a, const glm::vec3 &b) : A(a), B(b) {}

// Método para obter a localização da origem do raio
glm::vec3 ray::location() const {
    return A;
}

// Método para obter a direção do raio
glm::vec3 ray::direction() const {
    return B;
}

// Método para calcular o ponto em um raio em um determinado parâmetro 't'
glm::vec3 ray::point_at_parameter(float t) const {
    return A + t * B;
}
