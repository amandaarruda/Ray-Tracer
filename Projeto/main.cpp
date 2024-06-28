#include <iostream>
#include <fstream>
#include <array>
#include "./Includes/ray.h"
#include "./Includes/color.h"
#include "./Includes/hitable.h"
#include "./Includes/sphere.h"
#include "./Includes/hitable_list.h"
#include "./Includes/plane.h"
#include "./Includes/camera.h"
#include "./Includes/trianglemesh.h"
#include "./Includes/transform.h"
#include <cmath>
#include <cfloat>

#define M_PI 3.14159265358979323846  // Define a constante M_PI como o valor de pi

// Define cores vermelha, verde e azul normalizadas
const color red = glm::vec3(255.99, 0.0, 0.0);
const color green = glm::vec3(0.0, 255.99, 0.0);
const color blue = glm::vec3(0.0, 0.0, 255.99);

// Função para calcular a cor de um raio, dependendo se ele atinge algum objeto no mundo ou não
color ray_color(const ray& r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.0f, FLT_MAX, rec)) {
        return rec.cor;  // Retorna a cor do objeto atingido pelo raio
    }
    color backgroundColor = glm::vec3(0.0, 0.0, 0.0); // Cor preta para o background
    return backgroundColor;  // Retorna a cor de fundo se o raio não atingir nenhum objeto
}

// Função para renderizar a imagem em um arquivo PPM
void render_image(const char* filename, hitable* world, const camera& cam, int nx, int ny) {
    std::ofstream out(filename);  // Abre o arquivo de saída
    out << "P3\n" << nx << " " << ny << "\n255\n";  // Escreve o cabeçalho do arquivo PPM

    // Loop para gerar a imagem linha por linha
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);  // Coordenada u do pixel normalizada
            float v = float(j) / float(ny);  // Coordenada v do pixel normalizada
            ray r = cam.get_ray(u, v);  // Obtém o raio correspondente ao pixel na câmera
            color pixel_color = ray_color(r, world);  // Calcula a cor do raio
            write_color(out, pixel_color);  // Escreve a cor no arquivo PPM
        }
    }

    out.close();  // Fecha o arquivo de saída
}

int main() {
    std::cout << "Starting program..." << std::endl;

    int nx = 500;  // Largura da imagem
    int ny = 500;  // Altura da imagem
    std::cout << "Image dimensions: " << nx << "x" << ny << std::endl;

    glm::vec3 origin(0.0f, 0.0f, 0.0f);  // Origem da câmera
    glm::vec3 lookingat(0.0f, 0.0f, -1.0f);  // Ponto para onde a câmera está olhando
    glm::vec3 vup(0.0f, 1.0f, 0.0f);  // Vetor "up" da câmera
    float distance = 0.3f;  // Distância entre a câmera e o plano da imagem

    hitable* list[6];  // Lista de objetos hitable

    // Transformação inicial para transladar a esfera vermelha
    Transform transform;
    transform.setTransformationMatrix({0.0f, 0.0f, 0.0f}, {0, 0.5f, 0}); // Translação

    std::array<float, 3> centerRedSphere = {5, 0, -6};
    std::array<float, 3> transformedCenterRedSphere = transform.applyTransformation(centerRedSphere);
    list[0] = new sphere(glm::vec3(transformedCenterRedSphere[0], transformedCenterRedSphere[1], transformedCenterRedSphere[2]), 2, red); // Esfera vermelha transladada

    list[1] = new sphere(glm::vec3(5, -2, -6), 2.5, green); // Segunda esfera verde

    list[2] = new plane(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1), blue); // Plano azul no fundo

    // Plano afetado pela Transformação Afim
    std::array<float, 3> eulerAngles = {0.0f, 0.0f, glm::radians(10.0f)}; // Indicando rotação de 10° no eixo Z
    transform.setTransformationMatrix(eulerAngles, {0, 0, 0});

    std::array<float, 3> posVetor = {0, -3, 0};
    std::array<float, 3> normalVetor = {0, 1, 0};
    std::array<float, 3> newNormal = transform.applyTransformation(normalVetor);
    std::array<float, 3> newPoint = transform.applyTransformation(posVetor);
    list[3] = new plane(glm::vec3(newPoint[0], newPoint[1], newPoint[2]), glm::vec3(newNormal[0], newNormal[1], newNormal[2]), red); // Plano vermelho transformado

    int v_losango = 6;  // Quantidade de vértices na mesh do losango
    int t_losango = 8;  // Quantidade de triângulos na mesh do losango

    // Lista de vértices dos triângulos do losango
    glm::vec3 pontos_losango[v_losango] = {
        glm::vec3(0.0f, 0.0f, 1.0f - 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f - 1.5f), // Lado direito
        glm::vec3(0.0f, 1.0f, 0.0f - 2.0f), // Topo
        glm::vec3(-1.0f, 0.0f, 0.0f - 2.5f),
        glm::vec3(0.0f, -1.0f, 0.0f - 2.0f),
        glm::vec3(0.0f, 0.0f, -1.0f - 2.0f)
    };

    // Lista de trincas de índices de vértices do losango
    triple vertices_index_losango[t_losango] = {
        triple(0, 1, 2),
        triple(0, 2, 3),
        triple(0, 3, 4),
        triple(0, 4, 1),
        triple(1, 2, 5),
        triple(2, 3, 5),
        triple(3, 4, 5),
        triple(4, 1, 5)
    };

    // Criação da primeira mesh (losango)
    tmesh* losango_mesh = new tmesh(v_losango, t_losango, pontos_losango, vertices_index_losango, green + red);
    list[4] = losango_mesh;  // Adiciona o losango à lista de objetos

    // Criação da segunda mesh (pirâmide simples)
    int v_piramide = 5;  // Quantidade de vértices na mesh da pirâmide
    int t_piramide = 6;  // Quantidade de triângulos na mesh da pirâmide

    // Lista de vértices dos triângulos da pirâmide
    glm::vec3 pontos_piramide[v_piramide] = {
        glm::vec3(-2.5, 2, -3),
        glm::vec3(-3, -1, -4),
        glm::vec3(-1, -1, -4),
        glm::vec3(-1, -1, -2),
        glm::vec3(-3, -1, -2)
    };

    // Lista de trincas de índices de vértices da pirâmide
    triple vertices_index_piramide[t_piramide] = {
        triple(0, 1, 2),
        triple(0, 2, 3),
        triple(0, 3, 4),
        triple(0, 4, 1),
        triple(1, 2, 3),
        triple(1, 3, 4)
    };

    // Criação da segunda mesh (pirâmide)
    tmesh* triangulos_2 = new tmesh(v_piramide, t_piramide, pontos_piramide, vertices_index_piramide, blue + green);
    list[5] = triangulos_2;  // Adiciona a pirâmide à lista de objetos

    // Criação do mundo com a lista de objetos
    hitable* world = new hitable_list(list, 6);

    camera cam(origin, lookingat, vup, ny, nx, distance);  // Cria a câmera

    std::cout << "Rendering image before transformation..." << std::endl;
    render_image("output_before.ppm", world, cam, nx, ny);  // Renderiza a imagem antes da transformação

    // Aplica uma transformação adicional e renderiza novamente
    eulerAngles = {0.0f, 0.0f, glm::radians(45.0f)}; // Rotação de 45 graus no eixo Z
    transform.setTransformationMatrix(eulerAngles, {0.0f, 1.0f, 0.0f}); // Translação para cima
    newNormal = transform.applyTransformation(normalVetor);
    newPoint = transform.applyTransformation(posVetor);
    list[3] = new plane(glm::vec3(newPoint[0], newPoint[1], newPoint[2]), glm::vec3(newNormal[0], newNormal[1], newNormal[2]), red); // Atualiza o plano transformado

    std::cout << "Rendering image after transformation..." << std::endl;
    render_image("output_after.ppm", world, cam, nx, ny);  // Renderiza a imagem após a transformação

    std::cout << "Finished rendering images." << std::endl;

    return 0;
}
