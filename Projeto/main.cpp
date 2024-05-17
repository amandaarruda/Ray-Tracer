#include <iostream>
#include "./Includes/ray.h"   
#include "./Includes/color.h" 
#include "./Includes/camera.h"

#define M_PI 3.14159265358979323846  // Define a constante M_PI como o valor de pi

// Define uma cor vermelha, verde e azul normalizada
const color red = glm::vec3(255.99, 0.0, 0.0);
const color green = glm::vec3(0.0, 255.99, 0.0);
const color blue = glm::vec3(0.0, 0.0, 255.99);


int main() {
    int nx = 500; // Largura da imagem
    int ny = 500; // Altura da imagem

    std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // Imprime o cabeçalho do arquivo PPM

    glm::vec3 origin(0.0f, 0.0f, 0.0f);  // Origem da câmera
    glm::vec3 lookingat(0.0f, 0.0f, -1.0f);  // Ponto para onde a câmera está olhando
    glm::vec3 vup(0.0f, 1.0f, 0.0f);  // Vetor de "up" da câmera
    float distance = 0.3f;  // Distância entre a câmera e o plano da imagem

  // proximas coisas aqui
    camera cam(origin, lookingat, vup, ny, nx, distance);  // Cria uma câmera


    return 0; // Retorna 0 para indicar que o programa foi executado com sucesso
}
