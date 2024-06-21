#ifndef CAMERA_H  // Diretiva de pré-processador para garantir que o arquivo só será incluído uma vez
#define CAMERA_H

#include "ray.h"  // Inclui o arquivo de cabeçalho para a classe ray
#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm

class camera {
public:
    // Construtor da classe camera
    camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vres, float hres, float dist);

    // Método para obter um raio que passa pelo ponto (s, t) no plano da imagem
    ray get_ray(float s, float t);

    // Método para obter a origem da câmera
    glm::vec3 get_origin() const { return origem; }

private:
    glm::vec3 origem;  // Ponto de origem da câmera
    glm::vec3 lower_left_corner;  // Canto inferior esquerdo do plano da imagem
    glm::vec3 horizontal;  // Vetor horizontal que define a largura do plano da imagem
    glm::vec3 vertical;  // Vetor vertical que define a altura do plano da imagem
    glm::vec3 u, v, w;  // Vetores ortonormais que definem a base da câmera
};

#endif  // Fecha a diretiva de pré-processador

