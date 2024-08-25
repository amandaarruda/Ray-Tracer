#include "bezier_surface.h"
#include "../../External/glm/gtc/matrix_transform.hpp" 
#include <cmath> 

bezier_surface::bezier_surface(glm::vec3 p00, glm::vec3 p01, glm::vec3 p02, glm::vec3 p03, material* m) {
    // Construtor da classe bezier_surface que inicializa os pontos de controle e o ponteiro para o material.
    control_points[0] = p00;  // Define o primeiro ponto de controle da superfície de Bézier.
    control_points[1] = p01;  // Define o segundo ponto de controle da superfície de Bézier.
    control_points[2] = p02;  // Define o terceiro ponto de controle da superfície de Bézier.
    control_points[3] = p03;  // Define o quarto ponto de controle da superfície de Bézier.
    mat_ptr = m;  // Associa o material fornecido à superfície de Bézier.
}

glm::vec3 bezier_surface::bezier_surface_point(float u, float v) const {
    // Calcula um ponto na superfície de Bézier baseado nos parâmetros (u, v) utilizando interpolação linear (lerp).
    glm::vec3 p0 = glm::mix(control_points[0], control_points[1], u);  // Interpola entre os primeiros dois pontos de controle.
    glm::vec3 p1 = glm::mix(control_points[2], control_points[3], u);  // Interpola entre os últimos dois pontos de controle.
    return glm::mix(p0, p1, v);  // Interpola entre os dois pontos resultantes para obter o ponto final na superfície.
}

glm::vec3 bezier_surface::surface_normal(float u, float v) const {
    // Calcula a normal da superfície no ponto (u, v) usando diferenciação numérica.
    float delta = 0.001f;  // Pequeno deslocamento usado para aproximar as derivadas parciais.
    glm::vec3 p = bezier_surface_point(u, v);  // Calcula o ponto na superfície de Bézier para os valores atuais de u e v.
    glm::vec3 p_u = bezier_surface_point(u + delta, v);  // Calcula um ponto ligeiramente deslocado em u.
    glm::vec3 p_v = bezier_surface_point(u, v + delta);  // Calcula um ponto ligeiramente deslocado em v.
    glm::vec3 normal = glm::cross(p_v - p, p_u - p);  // Calcula o produto vetorial dos vetores tangentes para obter a normal.
    return glm::normalize(normal);  // Normaliza o vetor normal para que tenha comprimento 1.
}

bool bezier_surface::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // Verifica se um raio intersecta a superfície de Bézier e atualiza o registro de interseção se for o caso.
    
    int sample_count = 30;  // Define o número de amostras para a discretização da superfície de Bézier.
    for (int i = 0; i <= sample_count; i++) {
        for (int j = 0; j <= sample_count; j++) {
            float u = float(i) / float(sample_count);  // Calcula o parâmetro u baseado no índice da amostra.
            float v = float(j) / float(sample_count);  // Calcula o parâmetro v baseado no índice da amostra.

            glm::vec3 p = bezier_surface_point(u, v);  // Calcula o ponto na superfície de Bézier para os parâmetros (u, v).
            glm::vec3 normal = surface_normal(u, v);  // Calcula a normal da superfície no ponto (u, v).
            
            // Calcula o parâmetro t da equação do raio onde o raio intersecta o plano tangente à superfície.
            float t = glm::dot(p - r.location(), normal) / glm::dot(r.direction(), normal);
            
            // Verifica se a interseção está dentro dos limites permitidos para t.
            if (t < t_max && t > t_min) {
                glm::vec3 p_hit = r.point_at_parameter(t);  // Calcula o ponto de interseção no espaço tridimensional.
                
                // Verifica se o ponto calculado está próximo o suficiente da superfície real de Bézier.
                if (glm::length(p_hit - p) < 0.01f) {  // Critério para determinar uma interseção válida.
                    rec.t = t;  // Armazena o valor de t no registro de interseção.
                    rec.p = p_hit;  // Armazena o ponto de interseção no registro de interseção.
                    rec.normal = normal;  // Armazena a normal da superfície no ponto de interseção.
                    rec.cor = color(0.8f, 0.3f, 0.3f);  // Define a cor da superfície como um tom rosado.
                    
                    // Copia as propriedades do material para o registro de interseção.
                    rec.kdif = mat_ptr->kd;  // Coeficiente de difusão.
                    rec.kamb = mat_ptr->ka;  // Coeficiente de ambiente.
                    rec.kespc = mat_ptr->ks;  // Coeficiente especular.
                    rec.rug = mat_ptr->n;  // Rugosidade da superfície.
                    rec.kref = mat_ptr->kr;  // Coeficiente de reflexão.
                    rec.ktrans = mat_ptr->kt;  // Coeficiente de transmissão (transparência).
                    return true;  // Retorna true, indicando que o raio atingiu a superfície.
                }
            }
        }
    }
    return false;  // Retorna false se nenhuma interseção válida for encontrada.
}
