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
#include "./Includes/transform.h"
#include "./Includes/material.h"
#include "./Includes/environment.h"
#include "./Includes/light.h"
#include <cmath>
#include "float.h"
#define M_PI 3.14159265358979323846  // Define a constante M_PI como o valor de pi

using std::vector;

// Materiais básicos de teste
// "matte" significa fosco e sem brilho
// glossy, com muito brilho
//                               d     a     s     r     t     n 
material* matte = new material(0.8f, 0.5f, 0.1f, 0.0f, 0.1f, 10.0f);
material* glossy = new material(0.9f, 0.1f, 0.9f, 0.8f, 0.0f, 50.0f);

// Glass: Material com alto índice de refração
// Mirror: Material com alto índice de reflexão
material* glass = new material(0.1f, 0.2f, 0.1f, 0.1f, 1.0f, 1.0f);
material* mirror = new material(0.01f, 0.1f, 0.5f, 1.0f, 0.1f, 10.0f);

// Materiais Matt e Glossy personalizados para os planos
material* mattePlane = new material(0.2f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f);
material* glossyPlane = new material(0.8f, 0.1f, 0.5f, 0.3f, 0.0f, 30.0f);


// Luzes de cena
// Luz ambiente branca e pontos de luz local
color white = color(1,1,1);

Environment* ambientLight = new Environment(color(0.1f, 0.1f, 0.1f));

Light* light_point1 = new Light(glm::vec3(6,0,0 -1),white);
Light* light_point2 = new Light(glm::vec3(-9, 0,-2 -1),white);

vector<Light*> scene_lights;

// Define uma cor vermelha, verde e azul normalizada
const color red = glm::vec3(255.99, 0.0, 0.0);
const color green = glm::vec3(0.0, 255.99, 0.0);
const color blue = glm::vec3(0.0, 0.0, 255.99);

// Cores extras
const color slate = glm::vec3(0.5, 0.5, 0.5);
const color black = glm::vec3(0.01, 0.01, 0.01);



vec3 phong(hit_record rec, color amb_light, vector<Light*> point_lights, vec3 viewer_pos){

    // Parte ambiental da iluminação de Phong
    vec3 ambient_factor = rec.kamb * amb_light;

    // Este 'sum' será a soma dos componentes difuso e especular para cada luz da cena.
    vec3 sum = vec3(0.0f, 0.0f, 0.0f);
    for(Light* cur_light : point_lights){

        // Vetor normalizado que vai do ponto de interseção em direção à posição da luz
        vec3 L = normalize(cur_light->getPosition() - rec.p);

        // O produto (N . L) da equação de Phong, clamped para não ser negativo
        float diffuse_dot = glm::dot(rec.normal, L);
        diffuse_dot = glm::clamp(diffuse_dot, 0.0f, 1.0f);

        // Parte difusa da iluminação de Phong
        vec3 diffuse_factor = cur_light->getIntensity() * rec.cor * rec.kdif * diffuse_dot;

        // Vetor V vai do ponto de interseção até a posição do observador (câmera)
        vec3 V = normalize(viewer_pos - rec.p);

        // Vetor R é o vetor de reflexão, sendo 2.N (N. L) - L 
        vec3 R = 2.0f * (rec.normal * glm::dot(rec.normal, L)) - L; 

        // O produto (R . V) da equação de Phong
        float specular_dot = glm::dot(R, V);
        specular_dot = glm::clamp(specular_dot, 0.0f, 1.0f);

        // A reflexão elevada à potência do coeficiente de rugosidade
        float reflection = glm::pow(specular_dot, rec.rug);

        // Parte especular da iluminação de Phong
        vec3 specular_factor = cur_light->getIntensity() * rec.kespc * reflection;

        // Adiciona os fatores difuso e especular ao somatório
        sum += diffuse_factor + specular_factor;
    }

    // Resultado final, combinando parte ambiental, difusa e especular
    vec3 result = ambient_factor + sum;
    result = clamp(result, 0.0f, 1.0f);  // Clamp para garantir que o resultado esteja dentro do intervalo [0, 1]

    return result;
}

glm::vec3 snells_refract(const glm::vec3& incident, const glm::vec3& normal, float eta) {
    float cos_i = glm::dot(-incident, normal);
    float sin2_t = eta * eta * (1.0f - cos_i * cos_i);
    
    if (sin2_t > 1.0f) {
        return glm::vec3(0.0f);
    }
    
    float cos_t = sqrt(1.0f - sin2_t);
    return eta * incident + (eta * cos_i - cos_t) * normal;
}


// Função para calcular a cor de um raio, dependendo se ele atinge algum objeto no mundo ou não
color ray_color(const ray& r, hitable *world, vec3 cam_position, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001f, FLT_MAX, rec)) {
        vec3 phong_color = phong(rec, ambientLight->getAmbientLight(), scene_lights, cam_position);

        if (depth < 5) {
            // Reflexão (Manualmente)
            vec3 V = normalize(r.direction());
            vec3 N = rec.normal;
            vec3 reflected_dir = V - 2.0f * glm::dot(V, N) * N;
            ray reflected_ray(rec.p, reflected_dir);
            phong_color += rec.kref * ray_color(reflected_ray, world, cam_position, depth + 1);

            // Refração (Com Snells Formula)
            if (rec.ktrans > 0.0f) {
                vec3 refracted_dir;
                float eta = rec.ktrans; // Índice de refração
                float cos_i = glm::dot(-V, N);

                if (cos_i > 0) {
                    // Raio entrando no objeto
                    refracted_dir = snells_refract(V, N, eta);
                } else {
                    // Raio saindo do objeto, inverter o índice de refração
                    refracted_dir = snells_refract(V, -N, 1.0f / eta);
                }

                ray refracted_ray(rec.p, refracted_dir);
                phong_color += rec.ktrans * ray_color(refracted_ray, world, cam_position, depth + 1);
            }
        }

        return phong_color;
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
    hitable* list[7];

    //
    Transform transform;
    transform.setTransformationMatrix( glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0.5f, 0)); // Translação

    glm::vec3 centerRedSphere(5, 1, -6);

    list[0] = new sphere(transform.applyTransformation(centerRedSphere), 2, red, glossy);
    list[1] = new sphere(glm::vec3(5, -1.0, -6), 2.5, green, glass);
    list[2] = new plane(glm::vec3(0, 0, -20), glm::vec3(0, 0, 1), slate, mattePlane);



    // Plano afetado pela Transformação Afim
    glm::vec3 eulerAngles(0.0f, 0.0f, glm::radians(1.0f)); // Indicando rotação de 1° no eixo Z

    transform.setTransformationMatrix(eulerAngles, glm::vec3(0, -0.1, 0));

    glm::vec3 posVetor = glm::vec3(0, -3, 0);
    glm::vec3 normalVetor = glm::vec3(0, 1, 0);

    glm::vec3 newNormal = transform.applyTransformation(normalVetor);
    glm::vec3 newPoint = transform.applyTransformation(posVetor);

    //


    list[3] = new plane(newPoint, newNormal, slate, glossyPlane);

    int v_losango = 6;  // Quantidade de vértices na mesh
    int t_losango = 8;  // Quantidade de triângulos na mesh

    //Losango position - FRONT
    // glm::vec3 pontos_losango[v_losango] = {
    //     glm::vec3(-0.005f, 0.0f, 1.0f - 1.1f), // Front one?
    //     glm::vec3(1.0f, 0.0f, 0.0f - 1.5f), // Right one
    //     glm::vec3(0.0f, 1.0f, 0.0f - 2.0f), // Top one
    //     glm::vec3(-1.0f, 0.0f, 0.0f - 2.5f), // Left one
    //     glm::vec3(0.0f, -1.0f, 0.0f - 2.0f), // Bottom one
    //     glm::vec3(0.005f, 0.0f, -1.1f - 1.0f)
    // };

    // //Losango position - BACK
    glm::vec3 pontos_losango[v_losango] = {
        glm::vec3(-0.005f, 0.0f + 0.5, 1.0 - 1.1f - 3.0), // Front one?
        glm::vec3(1.0f, 0.0f + 0.5, 0.0f - 1.5f - 3.0), // Right one
        glm::vec3(0.0f, 1.0f + 0.5, 0.0f - 2.0f - 3.0), // Top one
        glm::vec3(-1.0f, 0.0f + 0.5, 0.0f - 2.5f - 3.0), // Left one
        glm::vec3(0.0f, -1.0f + 0.5, 0.0f - 2.0f - 3.0), // Bottom one
        glm::vec3(0.005f, 0.0f + 0.5, -1.1f - 1.0f - 3.0)
    };

    // Lista com triplas de índices de vértices do losango
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

    tmesh* losango_mesh = new tmesh(v_losango, t_losango, pontos_losango, vertices_index_losango, blue, glass);
    list[4] = losango_mesh;  // add losango na mesh

    // Segunda mesh é uma pirâmide simples
    int v_piramide = 5;  // Quantidade de vértices na mesh
    int t_piramide = 6;  // Quantidade de triângulos na mesh
    // Lista de vértices dos triângulos
    glm::vec3 pontos_piramide[v_piramide] = {

        glm::vec3(-2.5 -0.5 , 2, -3),
        glm::vec3(-3 -0.5, -1, -4),
        glm::vec3(-1 -0.5, -1, -4),
        glm::vec3(-1 -0.5, -1, -2),
        glm::vec3(-3 -0.5, -1, -2)
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

    tmesh* triangulos_2 = new tmesh(v_piramide, t_piramide, pontos_piramide, vertices_index_piramide, black, mirror);
    list[5] = triangulos_2;  // Adiciona a segunda malha à lista


    // list[6] = new sphere(glm::vec3(0, 4.0, -6), 1.5, white, mirror); // Esfera Espelho Up
    // list[6] = new sphere(glm::vec3(0, 0, -6), 0.5, white, mirror); // Esfera Espelho Front
    list[6] = new sphere(glm::vec3(0, 0, -3), 0.5, white, mirror); // Esfera Espelho Front

    
    // Cria o mundo com a lista de objetos
    hitable* world = new hitable_list(list, 7);

    scene_lights.push_back(light_point1);
    scene_lights.push_back(light_point2);
    
    camera cam(origin, lookingat, vup, ny, nx, distance);  // Cria uma câmera

    // Loop para gerar a imagem linha por linha
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);  // Coordenada u do pixel normalizada
            float v = float(j) / float(ny);  // Coordenada v do pixel normalizada
            ray r = cam.get_ray(u, v);  // Obtém o raio correspondente ao pixel na câmera
            color pixel_color = ray_color(r, world, cam.get_origin(), 0); // Calcula a cor do raio
            write_color(std::cout, pixel_color);  // Escreve a cor no arquivo PPM
        }
    }

    return 0;  // Retorna 0 para indicar que o programa foi executado com sucesso
}