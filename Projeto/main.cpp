#include <iostream>
#include "./Includes/ray.h"   
#include "./Includes/color.h" 
#include "./Includes/camera.h"

int main() {
    int nx = 500; // Largura da imagem
    int ny = 500; // Altura da imagem

    std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // Imprime o cabeçalho do arquivo PPM

  // proximas coisas aqui
    camera cam(origin, lookingat, vup, ny, nx, distance);  // Cria uma câmera


    return 0; // Retorna 0 para indicar que o programa foi executado com sucesso
}
