#ifndef RAY_H  // Incluindo só uma vez
#define RAY_H

#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm

// Classe para representar um raio
class ray {
public:
    // Construtores
    ray();  // Construtor padrão
    ray(const glm::vec3 &a, const glm::vec3 &b);  // Construtor com parâmetros

    // Métodos para obter a origem, direção e ponto em um raio
    glm::vec3 location() const;
    glm::vec3 direction() const;
    glm::vec3 point_at_parameter(float t) const;

private:
    glm::vec3 A; // localização de onde sairá o raio 
    glm::vec3 B; // direção que o raio seguirá
};

#endif  
