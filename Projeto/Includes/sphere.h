#ifndef SPHERE_H  
#define SPHERE_H

#include "hitable.h"  // Inclui o arquivo de cabeçalho para a classe hitable
#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
#include "color.h"  // Inclui o arquivo de cabeçalho para a classe color
#include "hitable.h"  // Inclui o arquivo de cabeçalho para a classe hitable
#include "texture.h"  // Inclui o arquivo de cabeçalho para a classe hitable
#include <memory> // For std::shared_ptr
#include <cmath> 

// Classe para representar uma esfera
class sphere : public hitable {
public:
    // Construtores
    sphere() {}  // Construtor padrão
    sphere(glm::vec3 cen, float r, std::shared_ptr<texture> tex, material* om) : center(cen), radius(r), tex(tex), objMaterial(om) {}  // Construtor com parâmetros

    // Método para verificar se um raio atinge a esfera
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    glm::vec3 center;  // Centro da esfera
    float radius;  // Raio da esfera
    material* objMaterial; // Material da esfera
    color cor;  // Cor da esfera
    std::shared_ptr<texture> tex;  // Ponteiro para a textura do plano

    static void get_sphere_uv(const glm::vec3& p, float& u, float& v) {
        // p: um dado ponto na esfera de raio 1, centrado na origem.
        // u: valor retornado [0,1] do ângulo ao redor de Y de X=-1.
        // v: valor retornado [0,1] do ângulo de Y=-1 até Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = std::acos(-p.y);
        auto phi = std::atan2(-p.z, p.x) + M_PI;

        u = phi / (2*M_PI);
        v = theta / M_PI;
    }
};

#endif  
