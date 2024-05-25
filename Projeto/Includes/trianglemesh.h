#ifndef TMESH_H
#define TMESH_H

#include "hitable.h"
#include "triangle.h"
#include <vector>
#include <tuple>

using triple = std::tuple<int, int, int>; // Define um alias para uma tupla de três inteiros

class tmesh : public hitable {
public:
    // Construtor
    tmesh(int n_vertices, int n_triangulos, glm::vec3 vertices[], triple vertices_index[], color cor);

    // Método para verificar se um raio atinge a malha de triângulos
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    int n_vertices; // Número total de vértices na malha
    int n_triangulos; // Número total de triângulos na malha
    std::vector<triangle> triangulos; // Lista de triângulos que compõem a malha
    color cor; // Cor da malha de triângulos
};

#endif
