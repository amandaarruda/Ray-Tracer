#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "color.h"

class ImageTexture {
public:
    unsigned char* data;
    int width, height, channels;

    ImageTexture(const std::string& filename) {
        data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load texture file: " << filename << std::endl;
            width = height = channels = 0;
        }
    }

    ~ImageTexture() {
        if (data) {
            stbi_image_free(data);
        }
    }

    color value(float u, float v) const {
        if (!data) return color(0, 1, 1); // Retorna uma cor default (magenta) caso a textura não carregue

        u = glm::clamp(u, 0.0f, 1.0f);
        v = 1.0f - glm::clamp(v, 0.0f, 1.0f); // Inverte v para corresponder às coordenadas da imagem

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        const float color_scale = 1.0f / 255.0f;
        int pixel_index = (j * width + i) * channels;

        return color(
            color_scale * data[pixel_index],
            color_scale * data[pixel_index + 1],
            color_scale * data[pixel_index + 2]
        );
    }
};

#endif
