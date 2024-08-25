#ifndef BEZIER_SURFACE_H
#define BEZIER_SURFACE_H

#include "hitable.h" 
#include "material.h"
#include "../../External/glm/glm.hpp"

class bezier_surface : public hitable {  // A classe 'bezier_surface' herda de 'hitable', o que significa que ela pode ser usada onde quer que objetos que podem ser atingidos por raios sejam necessários.
public:
    bezier_surface() {}  // Construtor padrão, inicializa uma superfície de Bézier sem definir pontos de controle ou material.
    
    // Construtor que recebe os pontos de controle da superfície de Bézier e um ponteiro para o material da superfície.
    bezier_surface(glm::vec3 p00, glm::vec3 p01, glm::vec3 p02, glm::vec3 p03, material* m);

    // Método virtual que verifica se um raio atinge a superfície de Bézier. Retorna true se houver interseção,
    // e atualiza o registro de interseção com informações como ponto de impacto, normal e material.
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    
private:
    // Método privado que calcula o ponto na superfície de Bézier correspondente aos parâmetros (u, v).
    glm::vec3 bezier_surface_point(float u, float v) const;
    
    // Método privado que calcula a normal da superfície no ponto (u, v), que é necessária para a iluminação.
    glm::vec3 surface_normal(float u, float v) const;
    
    glm::vec3 control_points[4];  // Array que armazena os quatro pontos de controle da superfície de Bézier.
    material* mat_ptr;  // Ponteiro para o material da superfície de Bézier.
};

#endif
