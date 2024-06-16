#include "transform.h"
#include <cmath>

Transform::Transform() : transformationMatrix(1.0f) {}

void Transform::setTransformationMatrix(const glm::vec3& eulerAngles, const glm::vec3& translation) {
    // Define matriz de transformação com os ângulos informados

    // Rotação do eixo X
    float cosX = cos(eulerAngles.x);
    float sinX = sin(eulerAngles.x);
    glm::mat4 rotationX(1.0f);
    rotationX[1][1] = cosX;
    rotationX[1][2] = -sinX;
    rotationX[2][1] = sinX;
    rotationX[2][2] = cosX;

    // Rotação do eixo Y
    float cosY = cos(eulerAngles.y);
    float sinY = sin(eulerAngles.y);
    glm::mat4 rotationY(1.0f);
    rotationY[0][0] = cosY;
    rotationY[0][2] = sinY;
    rotationY[2][0] = -sinY;
    rotationY[2][2] = cosY;

    // Rotação do eixo Z
    float cosZ = cos(eulerAngles.z);
    float sinZ = sin(eulerAngles.z);
    glm::mat4 rotationZ(1.0f);
    rotationZ[0][0] = cosZ;
    rotationZ[0][1] = -sinZ;
    rotationZ[1][0] = sinZ;
    rotationZ[1][1] = cosZ;

    // Combina as matrizes de rotação pra gerar a matriz de rotação final
    glm::mat4 rotationMatrix = rotationX * rotationY * rotationZ;

    // Define a matriz de translação
    glm::mat4 translationMatrix(1.0f);
    translationMatrix[3][0] = translation.x;
    translationMatrix[3][1] = translation.y;
    translationMatrix[3][2] = translation.z;

    // Faz a rotação depois a translação
    transformationMatrix = rotationMatrix * translationMatrix;
}

glm::vec3 Transform::applyTransformation(const glm::vec3 &vector) const {
    // Multiplica vetor de entrada pela matriz de transformação para gerar o novo vetor
    glm::vec4 transformedVector = transformationMatrix * glm::vec4(vector, 1.0f);
    return glm::vec3(transformedVector);
}

glm::mat4 Transform::getTransformationMatrix() const {
    // Retorna a matriz de transformação
    return transformationMatrix;
}
