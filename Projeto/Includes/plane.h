#ifndef PLANE_H  // Garantindo que o arquivo só será incluído uma vez
#define PLANE_H

#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
#include "ray.h"  // Inclui o arquivo de cabeçalho para a classe ray
#include "hitable.h"  // Inclui o arquivo de cabeçalho para a classe hitable
#include "texture.h"  // Inclui o arquivo de cabeçalho para a classe hitable
#include <memory> // For std::shared_ptr

// Classe para representar um plano
class plane : public hitable {
public:
    // Construtores
    plane() {}  // Construtor padrão
    plane(glm::vec3 p, glm::vec3 n, std::shared_ptr<texture> tex, material* om) 
        : plane_point(p), normal(normalize(n)), tex(tex), objMaterial(om) {}

    // Método para verificar se um raio atinge o plano
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    glm::vec3 plane_point;  // Ponto pertencente ao plano
    glm::vec3 normal;  // Vetor normal ao plano
    std::shared_ptr<texture> tex;  // Ponteiro para a textura do plano

    material* objMaterial;
};

#endif

