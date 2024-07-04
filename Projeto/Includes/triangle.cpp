#include "./triangle.h"

// Construtor da classe triangle
triangle::triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 color, material* om):a(a), b(b), c(c), cor(color), objMaterial(om) {
    // Calcular e normalizar o vetor normal do triângulo
    normal = normalize(cross((b-a), (c-a)));
    
    // Vetor da aresta a-b
    glm::vec3 u = b - a;
    // Vetor da aresta a-c
    glm::vec3 v = c - a;

    // Projeção do vetor u em v
    glm::vec3 projuv = dot(u,v)/dot(v,v) * v;
    // Projeção do vetor v em u
    glm::vec3 projvu = dot(v,u)/dot(u,u) * u;

    // Altura do ponto B
    hb = u - projuv;
    // Normalização inversa da altura hb
    hb = hb/dot(hb,hb);

    // Altura do ponto C
    hc = v - projvu;
    // Normalização inversa da altura hc
    hc = hc/dot(hc,hc);
};

// Método para verificar se um raio atinge o triângulo
bool triangle::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    // Calcula o parâmetro t para a interseção do raio com o plano do triângulo
    float t = glm::dot((a - r.location()), normal) / glm::dot(r.direction(), normal);

    // Verifica se a interseção está dentro dos limites t_min e t_max
    if (t < t_max && t > t_min) {
        // Calcula o ponto de interseção no plano
        glm::vec3 p = r.point_at_parameter(t);
        
        // Vetor do vértice do triângulo até o ponto de interseção
        glm::vec3 w = p - a;
        
        // Calcula os parâmetros beta e gamma para coordenada baricêntrica
        float beta = glm::dot(w, hb);
        float gamma = glm::dot(w, hc);
        float alpha = 1 - beta - gamma;

        // Verifica se o ponto de interseção está _dentro_ do triângulo
        if (alpha >= 0 && beta >= 0 && gamma >= 0) {
            rec.t = t;
            rec.p = p;

            // Verifica a direção da normal em relação ao raio
            glm::vec3 normal = this->normal;
            if (glm::dot(normal, r.direction()) > 0) {
                normal = -normal;
            }

            // Parâmetro t e ponto de interseção são atualizados
            rec.normal = normal; // Define o vetor normal ao ponto de interseção
            rec.cor = cor; // Define a cor do triângulo
            return true; // Retorna true pois houve uma interseção
        }
        return false; // Retorna false caso o ponto de interseção está fora do triângulo
    }
    return false; // Retorna false caso a interseção está fora dos limites t_min e t_max 
}
