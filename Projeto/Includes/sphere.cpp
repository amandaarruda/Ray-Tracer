#include "sphere.h"
#include <iostream>
#include <sstream>

// Função para carregar uma textura a partir de um arquivo PPM e aplicar na superfície da esfera
void sphere::loadTexture(const std::string& filename) {
    // Abre o arquivo de imagem PPM em modo binário
    std::ifstream file(filename, std::ios::binary);

    // Lê a primeira linha do arquivo, que deve conter o identificador "P6" para o formato PPM binário
    std::string line;
    std::getline(file, line);

    // Lê as linhas seguintes até encontrar a linha com as dimensões da imagem
    // Pula qualquer linha que comece com '#' (comentários no arquivo PPM)
    do {
        std::getline(file, line);
    } while (line[0] == '#');

    // Converte a linha de texto para um fluxo de entrada para extrair as dimensões da imagem
    std::istringstream ss(line);
    ss >> texWidth >> texHeight; // Extrai a largura e a altura da imagem da linha

    // Lê o valor máximo de cor (geralmente 255 para cores de 8 bits por canal)
    int maxColorValue;
    file >> maxColorValue;

    // Ignora o caractere de nova linha após o valor máximo de cor para preparar a leitura dos dados de pixel
    file.ignore(256, '\n');

    // Redimensiona o vetor `textureData` para armazenar todas as cores dos pixels da imagem
    textureData.resize(texWidth * texHeight);

    // Lê os dados de cada pixel (RGB) do arquivo e armazena no vetor `textureData`
    for (int i = 0; i < texWidth * texHeight; ++i) {
        unsigned char rgb[3]; // Array para armazenar os três componentes de cor (R, G, B)
        file.read(reinterpret_cast<char*>(rgb), 3); // Lê três bytes por pixel, um para cada cor
        // Converte os valores de cor de [0, 255] para [0, 1] e cria um objeto `color`
        textureData[i] = color(rgb[0] / 255.0f, rgb[1] / 255.0f, rgb[2] / 255.0f);
    }

    // Indica que a esfera possui uma textura carregada, que pode ser usada no mapeamento de textura
    hasTexture = true;
}

// Função para calcular as coordenadas UV de um ponto `p` na superfície da esfera
// As coordenadas UV são usadas para mapear a textura corretamente na superfície 3D
void sphere::getUV(const glm::vec3& p, float& u, float& v) const {
    // Calcula o ângulo `phi`, que é o ângulo polar na projeção da esfera no plano XZ
    float phi = atan2(p.z, p.x); // Retorna o ângulo em radianos

    // Calcula o ângulo `theta`, que é o ângulo zenital a partir do eixo Y
    float theta = asin(p.y / radius); // Retorna o ângulo em radianos, normalizando pela divisão do raio da esfera

    // Converte `phi` e `theta` para coordenadas UV no intervalo [0, 1]
    u = 1.0f - (phi + M_PI) / (2 * M_PI); // U é normalizado de 0 a 1, mapeando a longitude da esfera
    v = (theta + M_PI/2) / M_PI;          // V é normalizado de 0 a 1, mapeando a latitude da esfera
}

// Função para obter a cor da textura correspondente a uma dada coordenada UV
// As coordenadas UV determinam o local na textura de onde a cor deve ser extraída
color sphere::getTextureColor(float u, float v) const {
    // Converte as coordenadas UV para índices de pixel na imagem de textura
    int i = static_cast<int>(u * texWidth); // Converte a coordenada U para a posição horizontal (coluna) na imagem
    int j = static_cast<int>((1 - v) * texHeight - 0.001); // Converte a coordenada V para a posição vertical (linha) na imagem

    // Garante que os índices `i` e `j` estejam dentro dos limites válidos da textura
    // Isso evita acessos fora dos limites que causariam erros de memória
    i = std::max(0, std::min(texWidth - 1, i));
    j = std::max(0, std::min(texHeight - 1, j));

    // Retorna a cor do pixel correspondente na textura, indexado por `i` e `j`
    return textureData[j * texWidth + i];
}

// Função que verifica se um raio `r` atinge a esfera, calculando a interseção e as propriedades no ponto de impacto
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // Calcula o vetor que vai da origem do raio até o centro da esfera
    glm::vec3 oc = r.location() - center;
    
    // Calcula os coeficientes da equação quadrática da interseção entre o raio e a esfera
    float a = glm::dot(r.direction(), r.direction()); // Coeficiente `a` da equação quadrática: direção do raio ao quadrado
    float b = glm::dot(oc, r.direction());            // Coeficiente `b` da equação quadrática: origem ao centro . direção do raio
    float c = glm::dot(oc, oc) - radius * radius;     // Coeficiente `c`: distância do raio ao centro menos o raio da esfera ao quadrado
    float discriminant = b * b - a * c;               // Calcula o discriminante para determinar se há interseção

    // Se o discriminante for maior que zero, o raio intersecta a esfera em dois pontos (raízes reais)
    if (discriminant > 0) {
        // Calcula a solução mais próxima (raiz menor)
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            // Se a solução está dentro do intervalo válido, armazena informações sobre a interseção
            rec.t = temp; // Tempo de interseção
            rec.p = r.point_at_parameter(rec.t); // Ponto de interseção no espaço 3D
            rec.normal = (rec.p - center) / radius; // Vetor normal da esfera no ponto de interseção

            // Define as propriedades materiais no ponto de interseção
            rec.kdif = objMaterial->kd;    // Coeficiente de difusão
            rec.kamb = objMaterial->ka;    // Coeficiente de reflexão ambiente
            rec.kespc = objMaterial->ks;   // Coeficiente de especularidade
            rec.rug = objMaterial->n;      // Rugosidade da superfície

            rec.kref = objMaterial->kr;    // Coeficiente de reflexão
            rec.ktrans = objMaterial->kt;  // Coeficiente de transparência

            // Calcula as coordenadas UV no ponto de interseção
            float u, v;
            getUV((rec.p - center) / radius, u, v); // Normaliza o ponto na esfera antes de calcular UV
            rec.uv = glm::vec2(u, v); // Armazena as coordenadas UV na estrutura de interseção

            // Se a esfera tem uma textura, usa a cor da textura; caso contrário, usa a cor base da esfera
            if (hasTexture) {
                rec.cor = getTextureColor(u, v); // Cor da textura mapeada pelas coordenadas UV
            } else {
                rec.cor = cor; // Cor padrão da esfera
            }

            return true; // O raio intersecta a esfera com a solução encontrada
        }

        // Verifica a segunda solução (raiz maior)
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            // Se a segunda solução também está dentro do intervalo válido, armazena as informações
            rec.t = temp; // Tempo de interseção
            rec.p = r.point_at_parameter(rec.t); // Ponto de interseção no espaço 3D
            rec.normal = (rec.p - center) / radius; // Vetor normal da esfera no ponto de interseção

            // Define as propriedades materiais no ponto de interseção
            rec.kdif = objMaterial->kd;    // Coeficiente de difusão
            rec.kamb = objMaterial->ka;    // Coeficiente de reflexão ambiente
            rec.kespc = objMaterial->ks;   // Coeficiente de especularidade
            rec.rug = objMaterial->n;      // Rugosidade da superfície

            rec.kref = objMaterial->kr;    // Coeficiente de reflexão
            rec.ktrans = objMaterial->kt;  // Coeficiente de transparência

            // Calcula as coordenadas UV no ponto de interseção
            float u, v;
            getUV((rec.p - center) / radius, u, v); // Normaliza o ponto na esfera antes de calcular UV
            rec.uv = glm::vec2(u, v); // Armazena as coordenadas UV na estrutura de interseção

            // Se a esfera tem uma textura, usa a cor da textura; caso contrário, usa a cor base da esfera
            if (hasTexture) {
                rec.cor = getTextureColor(u, v); // Cor da textura mapeada pelas coordenadas UV
            } else {
                rec.cor = cor; // Cor padrão da esfera
            }

            return true; // O raio intersecta a esfera com a solução encontrada
        }
    }

    // Se o discriminante for menor ou igual a zero, o raio não intersecta a esfera (não há soluções reais)
    return false;
}
