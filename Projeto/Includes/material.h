#ifndef MATERIAL_H
#define MATERIAL_H

#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
#include "image_texture.h" 

using glm::vec3; // Usando a definição de vetor 3D da GLM

class material
{
    public:
        // Declaração de coeficientes
        /* 
        * kd = coeficiente de difusão
        * ka = coeficiente de luz ambiental (reação do objeto à luz ambiente)
        * ks = coeficiente de especularidade
        * kr = coeficiente de reflexão
        * kt = coeficiente de transmissão
        * n = rugosidade da superfície 
        */
        float kd, ka, ks, kr, kt, n;
        ImageTexture* texture; // Adiciona um ponteiro para a textura de imagem
        
        // Construtor padrão, construtor parametrizado
        material(); 
        material(float ckd, float cka, float cks, float ckr, float ckt, float cn, ImageTexture* tex = nullptr); // c = constructor
};

#endif
