#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"  
#include "../../External/glm/glm.hpp"  

class camera {
public:
    // Construtor da classe camera que inicializa a câmera com base na posição, direção e configuração da imagem.
    camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vres, float hres, float dist);

    // Método que gera um raio com base nas coordenadas (s, t) do plano da imagem.
    ray get_ray(float s, float t) const;

private:
    glm::vec3 origem;  // Ponto de origem da câmera (localização da câmera).
    glm::vec3 lower_left_corner;  // Canto inferior esquerdo do plano da imagem na cena.
    glm::vec3 horizontal;  // Vetor que representa a largura do plano da imagem.
    glm::vec3 vertical;  // Vetor que representa a altura do plano da imagem.
    glm::vec3 u, v, w;  // Vetores de base para o sistema de coordenadas da câmera: 'u' é o vetor para a direita, 'v' é o vetor para cima, e 'w' é o vetor para frente (direção da visão).
};

#endif
