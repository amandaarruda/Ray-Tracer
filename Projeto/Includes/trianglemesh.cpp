#include "trianglemesh.h"

// Construtor da classe tmesh
tmesh::tmesh(int n_vertices, int n_triangulos, glm::vec3 vertices[], triple vertices_index[], color cor, material* om)
    : n_vertices(n_vertices), n_triangulos(n_triangulos), cor(cor), objMaterial(om) {
    // Itera sobre todos os triângulos na malha
    for (int i = 0; i < n_triangulos; i++) {
        // Obtém os vértices do triângulo atual usando os índices fornecidos
        glm::vec3 A = vertices[std::get<0>(vertices_index[i])];
        glm::vec3 B = vertices[std::get<1>(vertices_index[i])];
        glm::vec3 C = vertices[std::get<2>(vertices_index[i])];
        // Cria um novo triângulo com os vértices e a cor especificados, e adiciona à lista de triângulos
        triangulos.push_back(triangle(A, B, C, cor, objMaterial));
    }
}

// Método para verificar se um raio atinge a malha de triângulos
bool tmesh::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    bool mesh_hit = false; // Define uma flag para indicar se a malha foi atingida pelo raio
    double closest_so_far = t_max; // Mantém o valor do parâmetro 't' mais próximo encontrado até o momento
    // Itera sobre todos os triângulos na malha
    for (const triangle &t : triangulos) {
        // Verifica se o raio atinge o triângulo atual e atualiza as informações de interseção se necessário
        if (t.hit(r, t_min, closest_so_far, rec)) {
            mesh_hit = true; // Define a flag como verdadeira pois houve interseção com pelo menos um triângulo
            closest_so_far = rec.t; // Atualiza o parâmetro 't' mais próximo encontrado até o momento
        }
    }
    return mesh_hit; // Retorna verdadeiro se algum triângulo foi atingido pelo raio
}
