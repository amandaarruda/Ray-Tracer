#include "plane.h"  
#include <cmath>  

// Método para verificar se um raio atinge o plano
bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // Calcula o parâmetro 't' da interseção do raio com o plano
    float denom = glm::dot(r.direction(), normal);
    if (fabs(denom) > 1e-6) {
        float t = glm::dot(plane_point - r.location(), normal) / denom;

        // Verifica se a interseção está dentro do intervalo válido
        if (t < t_max && t > t_min) {
            rec.t = t;  // Atualiza o parâmetro 't' do registro de interseção
            rec.p = r.point_at_parameter(t);  // Calcula o ponto de interseção
            rec.normal = normal;  // Define o vetor normal ao ponto de interseção
            
            // Calcula coordenadas de textura u e v
            // Ajuste essas fórmulas conforme a projeção desejada
            rec.u = rec.p.x;  // Exemplo simples, ajuste conforme necessário
            rec.v = rec.p.y;  // Exemplo simples, ajuste conforme necessário

            // Obtém a cor da textura usando as coordenadas u e v
            rec.cor = tex->value(rec.u, rec.v, rec.p);
            
            // Informações sobre o material do objeto
            rec.kdif = objMaterial->kd;
            rec.kamb = objMaterial->ka;
            rec.kespc = objMaterial->ks;
            rec.rug = objMaterial->n;

            // Informações dos materiais para a entrega 5, no momento da entrega 4 usaremos os valores 0
            rec.kref = objMaterial->kr;
            rec.ktrans = objMaterial->kt;

            return true;  // Retorna true para indicar que houve uma interseção
        }
    }
    return false;  // Retorna false se não houve interseção
}
