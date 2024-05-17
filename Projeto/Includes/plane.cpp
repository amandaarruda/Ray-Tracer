#include "plane.h"  
#include <cmath>  

// Método para verificar se um raio atinge o plano
bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // Calcula o parâmetro 't' da interseção do raio com o plano
    float t = glm::dot((plane_point - r.location()), normal) / glm::dot(r.direction(), normal);

    // Verifica se a interseção está dentro do intervalo válido
    if (t < t_max && t > t_min) {
        rec.t = t;  // Atualiza o parâmetro 't' do registro de interseção
        rec.p = r.point_at_parameter(t);  // Calcula o ponto de interseção
        rec.normal = normal;  // Define o vetor normal ao ponto de interseção
        rec.cor = cor;  // Define a cor do objeto
        return true;  // Retorna true para indicar que houve uma interseção
    }
    return false;  // Retorna false se não houve interseção
}

