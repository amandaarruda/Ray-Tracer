#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
using glm::vec3; // Usando a definição de vetor 3D da GLM
#include "color.h"  // Inclui o arquivo de cabeçalho para a classe color
#include <memory> // For std::shared_ptr
#include <algorithm> 
#include <cmath>

#include "stb_image.h"

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
    checker_plane_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

    checker_plane_texture(double scale, const color& c1, const color& c2)
      : checker_plane_texture(scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)) {}

    color value(double u, double v, const glm::vec3& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.x));
        auto yInteger = int(std::floor(inv_scale * p.y));
        auto zInteger = int(std::floor(inv_scale * p.z));

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
    checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

    checker_texture(double scale, const color& c1, const color& c2)
      : checker_texture(scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)) {}

    color value(double u, double v, const glm::vec3& p) const override {
          int u2 = int(std::floor(u * inv_scale));
          int v2 = int(std::floor(v * inv_scale));
      
          bool isEven = (u2 + v2) % 2 == 0;
      
          return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    double inv_scale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};

class image_texture : public texture {
public:
    image_texture(const std::string& filename)
    {
        // Carregar a imagem usando stb_image
        data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "ERRO: não foi possível dar load na imagem '" << filename << "'." << std::endl;
            width = height = channels = 0;
        }
    }

    ~image_texture() {
        stbi_image_free(data); // Liberar a memória da imagem
    }

    color value(double u, double v, const glm::vec3& p) const override {
        // Se a textura não está carregada, retorna uma cor de fallback
        if (data == nullptr) {
            return color(1.0, 0.0, 1.0); // Magenta para indicar erro
        }

        // Clamping para u e v para evitar extrapolações
        u = std::fmod(u, 1.0);
        v = std::fmod(v, 1.0);
        if (u < 0.0) u += 1.0;
        if (v < 0.0) v += 1.0;

        // Conversão de coordenadas UV para coordenadas de pixel i e j
        // O valor 0.001 subtraído no cálculo de j é para evitar a situação em que v 
        // resulta exatamente na borda superior da imagem, o que poderia causar um problema de arredondamento.
        int i = static_cast<int>(u * width);
        int j = static_cast<int>((1 - v) * height - 0.001);

        // conter i e j nos limites
        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        // Calcular a posição no array de dados
        unsigned char* pixel = data + j * width * channels + i * channels;

        // Retornar a cor normalizada
        return color(pixel[0] / 255.0, pixel[1] / 255.0, pixel[2] / 255.0);
    }

private:
    unsigned char* data;
    int width, height, channels;
};


#endif
