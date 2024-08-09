#ifndef SPHERE_H  
#define SPHERE_H

#include "hitable.h"  // Inclui o arquivo de cabeçalho para a classe hitable
#include "../../External/glm/glm.hpp" 
#include "color.h"  // Inclui o arquivo de cabeçalho para a classe color
#include <vector>  
#include <string>  
#include <fstream> 

// Classe para representar uma esfera
class sphere : public hitable {
public:
    // Construtores
    sphere() {}  // Construtor padrão
    sphere(glm::vec3 cen, float r, color c, material* om) 
        : center(cen), radius(r), cor(c), objMaterial(om), hasTexture(false) {}  // Construtor com inicialização de membros

    // Método para verificar se um raio atinge a esfera
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    // Método para carregar uma textura de um arquivo PPM
    void loadTexture(const std::string& filename);

    glm::vec3 center;  // Centro da esfera
    float radius;  // Raio da esfera
    material* objMaterial; // Material da esfera
    color cor;  // Cor padrão da esfera

private:
    bool hasTexture;  // Indica se a esfera possui uma textura carregada
    int texWidth, texHeight;  // Dimensões da textura
    std::vector<color> textureData;  // Dados da textura armazenados como um vetor de cores

    // Métodos auxiliares para o mapeamento de textura
    void getUV(const glm::vec3& p, float& u, float& v) const;  // Calcula as coordenadas UV de um ponto na esfera
    color getTextureColor(float u, float v) const;  // Retorna a cor da textura para as coordenadas UV fornecidas
};

#endif
