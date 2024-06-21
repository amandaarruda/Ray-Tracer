#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitable.h"  // Inclui o arquivo de cabeçalho para a classe hitable
#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
#include "color.h"  // Inclui o arquivo de cabeçalho para a classe color

// Classe para representar um triângulo
class triangle: public hitable {
    public:
    // Construtores
        triangle() {}
        triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 color, material* om); // Construtor com parâmetros
        
        // Método para verificar se um raio atinge o triângulo
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        
        // São esses parâmetros:
        glm::vec3 a;  // Primeiro vértice do triângulo
        glm::vec3 b;  // Segundo vértice do triângulo
        glm::vec3 c;  // Terceiro vértice do triângulo
        color cor;  // Cor do triângulo

        glm::vec3 normal;  // Vetor normal ao plano do triângulo

        glm::vec3 hb;  // Inverso da altura do ponto B
        glm::vec3 hc;  // Inverso da altura do ponto C

        material* objMaterial;
};


#endif
