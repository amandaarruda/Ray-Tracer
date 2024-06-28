#include "camera.h" 

// Construtor da classe camera
camera::camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vres, float hres, float dist) {
    w = glm::normalize(lookfrom - lookat);  // Calcula o vetor w como a direção oposta do vetor lookfrom -> lookat
    u = glm::normalize(glm::cross(vup, w));  // Calcula o vetor u como o produto vetorial entre vup e w, normalizado
    v = glm::cross(w, u);  // Calcula o vetor v como o produto vetorial entre w e u

    origem = lookfrom;  // Define a origem da câmera como o ponto lookfrom
    float proporcao = vres / hres;  // Calcula a proporção entre a altura e a largura da tela

    float xsize = 1.0f;  // Define o tamanho horizontal da tela como 1
    float ysize = xsize * proporcao;  // Calcula o tamanho vertical da tela usando a proporção

    float mLargura = xsize / 2;  // Calcula a metade da largura da tela
    float mAltura = mLargura * proporcao;  // Calcula a metade da altura da tela usando a proporção

    float cameraDistance = dist;  // Define a distância entre a câmera e o plano da imagem

    lower_left_corner = glm::vec3(-mLargura, -mAltura, -cameraDistance);  // Calcula o canto inferior esquerdo do plano da imagem

    horizontal = 2 * mLargura * u;  // Calcula o vetor horizontal como o dobro da metade da largura da tela multiplicado pelo vetor u
    vertical = 2 * mAltura * v;  // Calcula o vetor vertical como o dobro da metade da altura da tela multiplicado pelo vetor v
}

// Método para obter um raio que passa pelo ponto (s, t) no plano da imagem
ray camera::get_ray(float s, float t) const {
    return ray(origem, lower_left_corner + s * horizontal + t * vertical - origem);  // Retorna um raio que passa pelo ponto (s, t) no plano da imagem
}
