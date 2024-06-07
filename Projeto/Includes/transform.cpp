#include "transform.h"

Transform::Transform() : transformationMatrix(1.0f) {}

void Transform::setTransformationMatrix(const glm::vec3& eulerAngles) {
    // Define matriz de transformação com os ângulos informados
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), eulerAngles.x, glm::vec3(1, 0, 0));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), eulerAngles.y, glm::vec3(0, 1, 0));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), eulerAngles.z, glm::vec3(0, 0, 1));
    
    transformationMatrix = rotationX * rotationY * rotationZ; // Correct order of rotations
}

glm::vec3 Transform::applyTransformation(const glm::vec3 &vector) const {
    // Multiplica vetor de entrada pela matrizde transformação para gerar o novo vetor
    glm::vec4 transformedVector = transformationMatrix * glm::vec4(vector, 0.0f);
    return glm::vec3(transformedVector);
}

glm::mat4 Transform::getTransformationMatrix() const {
    // Retorna a matriz de transformação
    return transformationMatrix;
}
