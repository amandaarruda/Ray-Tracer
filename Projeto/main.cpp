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
#include "./Includes/texture.h"
#include "./Includes/light.h"
#include <cmath>
#include "float.h"
#define M_PI 3.14159265358979323846  // Define a constante M_PI como o valor de pi

using std::vector;

// Materiais básicos de teste
// "matte" significa fosco e sem brilho
// glossy, com muito brilho
//                               d     a     s     r     t     n 
material* matte = new material(0.8f, 0.4f, 0.1f, 0.0f, 0.1f, 1.0f);
material* glossy = new material(0.9f, 0.4f, 0.9f, 0.8f, 1.0f, 50.0f);

material* glass = new material(0.1f, 0.2f, 0.05f, 0.5f, 1.0f, 1.0f);

material* mirror = new material(0.01f, 0.1f, 0.5f, 1.0f, 0.1f, 10.0f);


material* mattePlane = new material(0.2f, 0.4f, 0.1f, 0.0f, 0.0f, 1.0f);
material* glossyPlane = new material(0.8f, 0.4f, 0.5f, 0.3f, 0.0f, 30.0f);

std::shared_ptr<texture> checker = std::make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

// Luzes de cena
// Luz ambiente branca e pontos de luz local
color white = color(1,1,1);


Environment* ambientLight = new Environment(color(0.1f, 0.1f, 0.1f));

Light* light_point1 = new Light(glm::vec3(0,1,4),white);
Light* light_point2 = new Light(glm::vec3(16,1,-16),white);

Light* light_point3 = new Light(glm::vec3(-1,-1,6),white);




vector<Light*> scene_lights;

// Define uma cor vermelha, verde e azul normalizada
const color red = glm::vec3(255.99, 0.0, 0.0);
const color green = glm::vec3(0.0, 255.99, 0.0);
const color blue = glm::vec3(0.0, 0.0, 255.99);

const color marineBlue = glm::vec3(0.0, 0.0, 80);



const color whiteMaterial = glm::vec3(255.99, 255.99, 255.99);
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
    // Pegando ângulo entre vetor de incidência(-1) e normal
    float cos_i = glm::dot(-incident, normal);

    // sen² + cos² = 1 -> senI² = (1 - cosI²)
    // senT = eta * senI
    // senT² = eta² * senI²
    float sin2_t = eta * eta * (1.0f - cos_i * cos_i);
    
    // Caso aconteça, refração total ocorreria (luz "presa"), sendo desconsiderado
    if (sin2_t > 1.0f) {
        return glm::vec3(0.0f);
    }

    // Extraindo cos_T
    float cos_t = sqrt(1.0f - sin2_t);

    // Vetor direção final, escalando, aplicando índice de refração com eta e tendendo a normal
    return eta * incident + (eta * cos_i - cos_t) * normal;
}


// Função para calcular a cor de um raio, dependendo se ele atinge algum objeto no mundo ou não
color ray_color(const ray& r, hitable *world, vec3 cam_position, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001f, FLT_MAX, rec)) {
        vec3 phong_color = phong(rec, ambientLight->getAmbientLight(), scene_lights, cam_position);

        if (depth < 2) {
            // Reflexão
            vec3 V = normalize(r.direction());
            vec3 N = rec.normal;
            vec3 reflected_dir = V - 2.0f * glm::dot(V, N) * N; //
            ray reflected_ray(rec.p, reflected_dir);
            phong_color += rec.kref * ray_color(reflected_ray, world, cam_position, depth + 1);

            // Refração (Lei de Snells)
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
    hitable* list[4];

    //
    Transform transform;
    transform.setTransformationMatrix( glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0.5f, 0)); // Translação

    glm::vec3 centerRedSphere(5, 1, -6);

    list[0] = new sphere(glm::vec3(-4, 0.0, -4), 1.5, red, matte);
    list[1] = new sphere(glm::vec3(0, 0.0, -4), 1.5, blue, glass);
    list[2] = new sphere(glm::vec3(4, 0.0, -4), 1.5, black, mirror);

    list[3] = new plane(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0), checker, glossyPlane);

    
    // Cria o mundo com a lista de objetos
    hitable* world = new hitable_list(list, 4);

    scene_lights.push_back(light_point1);
    scene_lights.push_back(light_point2);
    scene_lights.push_back(light_point3);

    
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
