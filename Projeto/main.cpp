#include <iostream>
#include "../External/glm/glm.hpp"
#include "../External/glm/gtc/matrix_transform.hpp"
#include "./Includes/ray.h"
#include "./Includes/color.h"
#include "./Includes/hitable.h"
#include "./Includes/sphere.h"
#include "./Includes/hitable_list.h"
#include "./Includes/plane.h"
#include "./Includes/camera.h"
#include "./Includes/trianglemesh.h" 
#include <cmath>
#include "float.h"

#define M_PI 3.14159265358979323846  // Define a constante M_PI como o valor de pi

// Define uma cor vermelha, verde e azul normalizada
const color red = glm::vec3(255.99, 0.0, 0.0);
const color green = glm::vec3(0.0, 255.99, 0.0);
const color blue = glm::vec3(0.0, 0.0, 255.99);

// Função para calcular a cor de um raio, dependendo se ele atinge algum objeto no mundo ou não
color ray_color(const ray& r, hitable *world)
{
    hit_record rec;
    if(world->hit(r, 0.0f, FLT_MAX, rec)){  // Testa se o raio atinge algum objeto no mundo
        return rec.cor;  // Retorna a cor do objeto atingido pelo raio
    }

    color backgroundColor = glm::vec3(0.0,0.0,0.0); // Cor preta para o background
    return backgroundColor;  // Retorna a cor de fundo se o raio não atingir nenhum objeto
}

int main() {
    int nx = 500;  // Largura da imagem
    int ny = 500;  // Altura da imagem

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";  // Imprime o cabeçalho do arquivo PPM

    glm::vec3 origin(0.0f, 0.0f, 0.0f);  // Origem da câmera
    glm::vec3 lookingat(0.0f, 0.0f, -1.0f);  // Ponto para onde a câmera está olhando
    glm::vec3 vup(0.0f, 1.0f, 0.0f);  // Vetor de "up" da câmera
    float distance = 0.3f;  // Distância entre a câmera e o plano da imagem

    // Cria uma lista de objetos hitable, incluindo duas esferas, dois planos e duas malhas
    hitable* list[5];
    list[0] = new sphere(glm::vec3(0, 0, -1), 0.5, red);
    list[1] = new sphere(glm::vec3(-1, 0, -2), 0.5, green);
    list[2] = new plane(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1), blue);

    // Primeira mesh é do Icosaedro (polígono com 20 faces)
    int v_icosaedro = 12;  // Quantidade de vértices na mesh
    int t_icosaedro = 20;  // Quantidade de triângulos na mesh
    // Lista de vértices dos triângulos
    glm::vec3 pontos_icosaedro[v_icosaedro] = {
        glm::vec3(-2.0, -0.9742688878808665, -1.1493491916479601),
        glm::vec3(-2.0, -2.0257311121191335, -1.1493491916479601),
        glm::vec3(-1.1493491916479597, -1.5, -1.4742688878808665),
        glm::vec3(-1.474268887880866, -0.6493491916479601, -2.0),
        glm::vec3(-2.525731112119134, -0.6493491916479601, -2.0),
        glm::vec3(-2.8506508083520403, -1.5, -1.4742688878808665),
        glm::vec3(-2.0, -0.9742688878808665, -2.85065080835204),
        glm::vec3(-2.0, -2.0257311121191335, -2.85065080835204),
        glm::vec3(-2.8506508083520403, -1.5, -2.5257311121191335),
        glm::vec3(-1.1493491916479597, -1.5, -2.5257311121191335),
        glm::vec3(-1.474268887880866, -2.35065080835204, -2.0),
        glm::vec3(-2.525731112119134, -2.35065080835204, -2.0)
    };
    // Lista com triplas de índices de vértices
    triple vertices_index_icosaedro[t_icosaedro] = {
        triple(0, 1, 2),
        triple(0, 3, 4),
        triple(0, 4, 5),
        triple(0, 2, 3),
        triple(0, 5, 1),
        triple(6, 7, 8),
        triple(6, 3, 9),
        triple(6, 4, 3),
        triple(6, 9, 7),
        triple(6, 8, 4),
        triple(1, 10, 2),
        triple(1, 11, 10),
        triple(1, 5, 11),
        triple(7, 10, 11),
        triple(7, 11, 8),
        triple(7, 9, 10),
        triple(3, 2, 9),
        triple(10, 9, 2),
        triple(4, 8, 5),
        triple(11, 5, 8),
    };

    tmesh* triangulos_1 = new tmesh(v_icosaedro, t_icosaedro, pontos_icosaedro, vertices_index_icosaedro, green + red);
    list[3] = triangulos_1;  // Adiciona a primeira malha à lista

    // Segunda mesh é uma pirâmide simples
    int v_piramide = 5;  // Quantidade de vértices na mesh
    int t_piramide = 6;  // Quantidade de triângulos na mesh
    // Lista de vértices dos triângulos
    glm::vec3 pontos_piramide[v_piramide] = {
        glm::vec3(-2, 0, -3),
        glm::vec3(-3, -1, -4),
        glm::vec3(-1, -1, -4),
        glm::vec3(-1, -1, -2),
        glm::vec3(-3, -1, -2)
    };
    // Lista com triplas de índices de vértices
    triple vertices_index_piramide[t_piramide] = {
        triple(0, 1, 2),
        triple(0, 2, 3),
        triple(0, 3, 4),
        triple(0, 4, 1),
        triple(1, 2, 3),
        triple(1, 3, 4)
    };

    tmesh* triangulos_2 = new tmesh(v_piramide, t_piramide, pontos_piramide, vertices_index_piramide, blue + green);
    list[4] = triangulos_2;  // Adiciona a segunda malha à lista
    
    // Cria o mundo com a lista de objetos
    hitable* world = new hitable_list(list, 5);
    
    camera cam(origin, lookingat, vup, ny, nx, distance);  // Cria uma câmera

    // Loop para gerar a imagem linha por linha
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);  // Coordenada u do pixel normalizada
            float v = float(j) / float(ny);  // Coordenada v do pixel normalizada
            ray r = cam.get_ray(u, v);  // Obtém o raio correspondente ao pixel na câmera
            color pixel_color = ray_color(r, world);  // Calcula a cor do raio
            write_color(std::cout, pixel_color);  // Escreve a cor no arquivo PPM
        }
    }

    return 0;  // Retorna 0 para indicar que o programa foi executado com sucesso
}
