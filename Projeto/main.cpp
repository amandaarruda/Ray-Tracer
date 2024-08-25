#include <iostream>
#include "../External/glm/glm.hpp"
#include "../External/glm/gtc/matrix_transform.hpp"
#include "./Includes/ray.h"
#include "./Includes/color.h"
#include "./Includes/hitable.h"
#include "./Includes/hitable_list.h"
#include "./Includes/bezier_surface.h"
#include "./Includes/camera.h"
#include "./Includes/material.h"
#include "./Includes/environment.h"
#include "./Includes/light.h"
#include "./Includes/plane.h"
#include <cmath>
#include <vector> 
#include <float.h>

#define M_PI 3.14159265358979323846  // Define o valor de pi se não estiver definido anteriormente.

using std::vector;  // Usa o namespace padrão para a classe 'vector', evitando a necessidade de prefixar com 'std::'.

// Cria materiais para os objetos da cena.
material* basicMaterial = new material(0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f);  // Material básico com propriedades de difusão e especular.

color white = color(1, 1, 1);  // Define a cor branca.
Environment* ambientLight = new Environment(color(0.1f, 0.1f, 0.1f));  // Cria uma fonte de luz ambiente com intensidade baixa.
Light* light_point = new Light(glm::vec3(0, 1, 4), white);  // Cria uma fonte de luz pontual na posição (0, 1, 4) com cor branca.

vector<Light*> scene_lights;  // Vetor para armazenar as fontes de luz na cena.

glm::vec3 phong(hit_record rec, color amb_light, vector<Light*> point_lights, vec3 viewer_pos) {
    // Calcula a cor resultante para um ponto na superfície usando o modelo de iluminação Phong.
    vec3 ambient_factor = rec.kamb * amb_light;  // Calcula a contribuição da luz ambiente.
    vec3 sum = vec3(0.0f, 0.0f, 0.0f);  // Inicializa a soma das contribuições de luz difusa e especular.
    
    // Itera sobre todas as fontes de luz pontuais.
    for (Light* cur_light : point_lights) {
        vec3 L = normalize(cur_light->getPosition() - rec.p);  // Vetor da superfície para a posição da luz.
        float diffuse_dot = glm::dot(rec.normal, L);  // Calcula o termo difuso da iluminação.
        diffuse_dot = glm::clamp(diffuse_dot, 0.0f, 1.0f);  // Garante que o valor esteja entre 0 e 1.
        vec3 diffuse_factor = cur_light->getIntensity() * rec.cor * rec.kdif * diffuse_dot;  // Calcula a contribuição da luz difusa.
        
        vec3 V = normalize(viewer_pos - rec.p);  // Vetor da superfície para a posição do visualizador.
        vec3 R = 2.0f * (rec.normal * glm::dot(rec.normal, L)) - L;  // Calcula o vetor refletido.
        float specular_dot = glm::dot(R, V);  // Calcula o termo especular da iluminação.
        specular_dot = glm::clamp(specular_dot, 0.0f, 1.0f);  // Garante que o valor esteja entre 0 e 1.
        float reflection = glm::pow(specular_dot, rec.rug);  // Calcula a reflexão especular com base no coeficiente de rugosidade.
        vec3 specular_factor = cur_light->getIntensity() * rec.kespc * reflection;  // Calcula a contribuição da luz especular.
        
        sum += diffuse_factor + specular_factor;  // Adiciona a contribuição da luz difusa e especular à soma total.
    }
    
    vec3 result = ambient_factor + sum;  // Soma a contribuição da luz ambiente.
    return glm::clamp(result, 0.0f, 1.0f);  // Garante que o resultado final da cor esteja entre 0 e 1.
}

color ray_color(const ray& r, hitable* world, vec3 cam_position, int depth) {
    // Calcula a cor de um raio ao interagir com a cena.
    hit_record rec;  // Registro de interseção para armazenar informações sobre o impacto do raio.
    hit_record temp_rec;  // Registro temporário para armazenar informações de interseção.
    bool hit_anything = false;  // Flag para verificar se o raio atingiu algum objeto.
    float closest_so_far = FLT_MAX;  // Inicializa o valor mais próximo possível para a verificação de interseção.

    // Verifica a superfície de Bézier primeiro.
    if (world->hit(r, 0.001f, closest_so_far, rec)) {
        hit_anything = true;  // Marca que algo foi atingido.
        closest_so_far = rec.t;  // Atualiza a distância mais próxima.
    }

    // Em seguida, verifica o plano
    if (world->hit(r, 0.001f, closest_so_far, temp_rec)) {
        hit_anything = true;  // Marca que algo foi atingido.
        rec = temp_rec;  // Atualiza o registro de interseção com informações do plano.
    }

    // Se algum objeto foi atingido, calcula a cor usando o modelo de iluminação Phong.
    if (hit_anything) {
        vec3 phong_color = phong(rec, ambientLight->getAmbientLight(), scene_lights, cam_position);
        return phong_color;  // Retorna a cor calculada.
    }
    return color(0.0f, 0.0f, 0.0f);  // Retorna preto se nada for atingido.
}

int main() {
    int nx = 500;  // Largura da imagem em pixels.
    int ny = 500;  // Altura da imagem em pixels.

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";  // Cabeçalho do formato de saída PPM para imagens.

    glm::vec3 origin(0.0f, 0.0f, 3.0f);  // Posição da câmera.
    glm::vec3 lookingat(0.0f, 0.0f, 0.0f);  // Ponto para onde a câmera está olhando.
    glm::vec3 vup(0.0f, 1.0f, 0.0f);  // Vetor para cima para definir a orientação vertical da câmera.
    float distance = 1.0f;  // Distância do plano da imagem à câmera.

    hitable* list[1];  // Lista de objetos hitable na cena.

    // Cria uma superfície de Bézier com pontos de controle e material básico.
    list[0] = new bezier_surface(glm::vec3(0.3f, 0.3f, -1.0f),
                                 glm::vec3(0.6f, 1.3f, -2.0f),
                                 glm::vec3(-1.3f, 0.6f, -1.0f),
                                 glm::vec3(1.6f, -1.6f, 0.0f),
                                 basicMaterial);

    // Cria a lista de objetos hitable.
    hitable* world = new hitable_list(list, 1);
    scene_lights.push_back(light_point);  // Adiciona a fonte de luz à cena.

    // Inicializa a câmera.
    camera cam(origin, lookingat, vup, float(ny), float(nx), distance);

    // Loop para renderizar a imagem.
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);  // Coordenada horizontal na imagem.
            float v = float(j) / float(ny);  // Coordenada vertical na imagem.
            ray r = cam.get_ray(u, v);  // Gera um raio a partir da câmera para o ponto (u, v) no plano da imagem.
            color col = ray_color(r, world, origin, 0);  // Calcula a cor para o raio.
            write_color(std::cout, col);  // Escreve a cor no formato de imagem PPM.
        }
    }
}
