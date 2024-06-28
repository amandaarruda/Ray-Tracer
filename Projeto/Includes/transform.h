#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "../../External/glm/glm.hpp"
#include "../../External/glm/gtc/matrix_transform.hpp"
class Transform {
public:
    // Construtor vazio
    Transform();

    // Método para modificar a matriz baseada nos ângulos informados
    void setTransformationMatrix(const glm::vec3& eulerAngles, const glm::vec3& translation);

    // Método para retornar o vetor multiplicado pela matriz de transformação
    glm::vec3 applyTransformation(const glm::vec3& vec) const;
    
    // Método para retornar a atual matriz de transformação
    glm::mat4 getTransformationMatrix() const;
private:
    glm::mat4 transformationMatrix; // Definindo a matriz de transformação
};
#endif
