#ifndef HITABLE_H  // Diretiva de pré-processador para garantir que o arquivo só será incluído uma vez
#define HITABLE_H

#include "ray.h"  // Inclui o arquivo de cabeçalho para a classe ray
#include "color.h"  // Inclui o arquivo de cabeçalho para a classe color
#include "material.h" // Inclui o arquivo de cabeçalho para a classe material
#include "texture.h" 
#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm

// Estrutura para armazenar informações sobre o ponto de interseção com um objeto
struct hit_record {
    float t;  // Parâmetro 't' do raio no ponto de interseção
    float u;  // Parâmetro 'u' para texturas                                        
    float v;  // Parâmetro 'v' para texturas
    glm::vec3 p;  // Ponto de interseção
    glm::vec3 normal;  // Vetor normal ao ponto de interseção
    color cor;  // Cor do objeto atingido

    float kdif; // kd
    float kamb; // ka
    float kespc; // ks
    float kref;
    float ktrans;
    float rug;
};

// Classe abstrata para representar objetos que podem ser atingidos por um raio
class hitable {
public:
    // Método virtual puro para verificar se um raio atinge o objeto
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif 

