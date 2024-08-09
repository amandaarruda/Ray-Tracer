#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
using glm::vec3; // Usando a definição de vetor 3D da GLM
#include "color.h"  // Inclui o arquivo de cabeçalho para a classe color
#include <memory> // For std::shared_ptr
#include <algorithm> 
#include <cmath>

#include "rtw_stb_image.h"

class texture {
  public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const glm::vec3& p) const = 0;
};

class solid_color : public texture {
  public:
    solid_color(const color& albedo) : albedo(albedo) {}

    solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)) {}

    color value(double u, double v, const glm::vec3& p) const override {
        return albedo;
    }

  private:
    color albedo;
};

class checker_plane_texture : public texture {
  public:
    // Construtor que aceita um fator de escala e dois ponteiros para texturas (ou cores)
    checker_plane_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

    // Construtor que aceita um fator de escala e duas cores, e cria texturas sólidas para elas
    checker_plane_texture(double scale, const color& c1, const color& c2)
      : checker_plane_texture(scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)) {}

    // Método que retorna a cor baseada na textura de xadrez para as coordenadas fornecidas
    color value(double u, double v, const glm::vec3& p) const override {
        // Calcular os inteiros x, y e z baseados nas coordenadas do ponto e na escala
        auto xInteger = int(std::floor(inv_scale * p.x));
        auto yInteger = int(std::floor(inv_scale * p.y));
        auto zInteger = int(std::floor(inv_scale * p.z));

        // Determinar se a célula atual é par ou ímpar e retornar a cor baseada nisso
        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    double inv_scale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};

class checker_texture : public texture {
  public:
    // Construtor que aceita um fator de escala e dois ponteiros para texturas (ou cores)
    checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

    // Construtor que aceita um fator de escala e duas cores, e cria texturas sólidas para elas
    checker_texture(double scale, const color& c1, const color& c2)
      : checker_texture(scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)) {}

    color value(double u, double v, const glm::vec3& p) const override {
          int u2 = int(std::floor(u * inv_scale));
          int v2 = int(std::floor(v * inv_scale));
      
          // Determinar se a célula atual é par ou ímpar e retornar a cor baseada nisso
          bool isEven = (u2 + v2) % 2 == 0;
          return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    double inv_scale; // Inverso da escala, usado para calcular as coordenadas da célula
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};

class image_texture : public texture {
  public:
    image_texture(const char* filename) : image(filename) {}

    color value(double u, double v, const glm::vec3& p) const override {
        // Se não temos textura, retornamos um ciano só para não ficar sem textura.
        if (image.height() <= 0) return color(0,1,1);

        u = glm::clamp(u, 0.0, 1.0);
        v = 1.0 - glm::clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        // Convertemos coordenadas de textura a índices de pixel de imagem
        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i,j);

        // Converter valores de pixel de [0, 255] para [0, 1]
        auto color_scale = 1.0 / 255.0;
        return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }

  private:
    rtw_image image;
};

class image_plane_texture : public texture {
public:
    image_plane_texture(const char* filename) : image(filename) {}

    color value(double u_coord, double v_coord, const glm::vec3& p) const override {
        // Se não temos textura, retornamos um ciano só para não ficar sem textura.
        if (image.height() <= 0) return color(0,1,1);

        // Computamos coordenadas UV baseadas no x e y de p
        double x = p.x;
        double z = p.z;

        // Normalizar as coordenadas para dentro do range [0,1] 
        double u = x - std::floor(x);  // Local variable u
        double v = z - std::floor(z);  // Local variable v

        // clamp e translação no V às coordenadas da imagem
        v = 1.0 - glm::clamp(v, 0.0, 1.0);
        u = glm::clamp(u, 0.0, 1.0);

        // Convertemos coordenadas de textura a índices de pixel de imagem
        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i, j);

        // Converter valores de pixel de [0, 255] para [0, 1]
        auto color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    rtw_image image;
};


#endif
