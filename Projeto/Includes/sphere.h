#ifndef SPHERE_H  
#define SPHERE_H

#include "hitable.h"  // Inclui o arquivo de cabeçalho para a classe hitable
#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
#include "color.h"  // Inclui o arquivo de cabeçalho para a classe color

// Classe para representar uma esfera
class sphere : public hitable {
public:
    // Construtores
    sphere() {}  // Construtor padrão
    sphere(glm::vec3 cen, float r, color c, material* om) : center(cen), radius(r), cor(c), objMaterial(om) {}  // Construtor com parâmetros

    // Método para verificar se um raio atinge a esfera
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    glm::vec3 center;  // Centro da esfera
    float radius;  // Raio da esfera
    material* objMaterial; // Material da esfera
    color cor;  // Cor da esfera
};

#endif  
