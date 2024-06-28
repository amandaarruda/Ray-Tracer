#include "transform.h"
#include <cmath>
#include <array>

// Construtor da classe Transform.
// Inicializa a matriz de transformação como uma matriz identidade 4x4.
Transform::Transform() : transformationMatrix{{
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
}} {}

// Define a matriz de transformação com base nos ângulos de Euler para rotação e um vetor de translação.
// Primeiro, são calculadas as matrizes de rotação individuais para os eixos X, Y e Z usando os ângulos de Euler fornecidos.
// Em seguida, essas matrizes são combinadas multiplicando-as na ordem Z * Y * X para obter a matriz de rotação final.
// Finalmente, a matriz de translação é combinada com a matriz de rotação para formar a matriz de transformação completa.
void Transform::setTransformationMatrix(const std::array<float, 3>& eulerAngles, const std::array<float, 3>& translation) {
    // Calcula a matriz de rotação para o eixo X
    float cosX = cos(eulerAngles[0]);
    float sinX = sin(eulerAngles[0]);
    std::array<std::array<float, 4>, 4> rotationX = {{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, cosX, -sinX, 0.0f},
        {0.0f, sinX, cosX, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};

    // Calcula a matriz de rotação para o eixo Y
    float cosY = cos(eulerAngles[1]);
    float sinY = sin(eulerAngles[1]);
    std::array<std::array<float, 4>, 4> rotationY = {{
        {cosY, 0.0f, sinY, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {-sinY, 0.0f, cosY, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};

    // Calcula a matriz de rotação para o eixo Z
    float cosZ = cos(eulerAngles[2]);
    float sinZ = sin(eulerAngles[2]);
    std::array<std::array<float, 4>, 4> rotationZ = {{
        {cosZ, -sinZ, 0.0f, 0.0f},
        {sinZ, cosZ, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};

    // Combina as matrizes de rotação X, Y e Z em uma única matriz de rotação
    std::array<std::array<float, 4>, 4> rotationMatrix;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            rotationMatrix[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                rotationMatrix[i][j] += rotationX[i][k] * rotationY[k][j];
            }
        }
    }

    std::array<std::array<float, 4>, 4> finalRotationMatrix;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            finalRotationMatrix[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                finalRotationMatrix[i][j] += rotationMatrix[i][k] * rotationZ[k][j];
            }
        }
    }

    // Define a matriz de translação
    std::array<std::array<float, 4>, 4> translationMatrix = {{
        {1.0f, 0.0f, 0.0f, translation[0]},
        {0.0f, 1.0f, 0.0f, translation[1]},
        {0.0f, 0.0f, 1.0f, translation[2]},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};

    // Combina a matriz de rotação final com a matriz de translação para obter a matriz de transformação completa
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            transformationMatrix[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                transformationMatrix[i][j] += finalRotationMatrix[i][k] * translationMatrix[k][j];
            }
        }
    }
}

// Aplica a matriz de transformação a um vetor 3D e retorna o vetor transformado.
// O vetor 3D é estendido para um vetor 4D adicionando um componente de 1.0.
// Em seguida, a matriz de transformação é multiplicada pelo vetor 4D, e o vetor resultante é retornado.
std::array<float, 3> Transform::applyTransformation(const std::array<float, 3>& vec) const {
    std::array<float, 4> vec4 = {vec[0], vec[1], vec[2], 1.0f};  // Extende o vetor 3D para 4D.
    std::array<float, 4> result = {0.0f, 0.0f, 0.0f, 0.0f};  // Inicializa o vetor resultado.

    // Multiplica a matriz de transformação pelo vetor 4D.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i] += transformationMatrix[i][j] * vec4[j];
        }
    }

    // Retorna o vetor transformado como um vetor 3D.
    return {result[0], result[1], result[2]};
}

// Retorna a matriz de transformação atual.
std::array<std::array<float, 4>, 4> Transform::getTransformationMatrix() const {
    return transformationMatrix;
}
