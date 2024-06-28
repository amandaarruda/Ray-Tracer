#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <array>

class Transform {
public:
    Transform();  // Construtor que inicializa a matriz de transformação como uma matriz identidade 4x4.

    // Define a matriz de transformação com base nos ângulos de Euler para rotação e um vetor de translação.
    void setTransformationMatrix(const std::array<float, 3>& eulerAngles, const std::array<float, 3>& translation);
    
    // Aplica a transformação definida a um vetor 3D e retorna o vetor transformado.
    std::array<float, 3> applyTransformation(const std::array<float, 3>& vec) const;
    
    // Retorna a matriz de transformação atual.
    std::array<std::array<float, 4>, 4> getTransformationMatrix() const;

private:
    std::array<std::array<float, 4>, 4> transformationMatrix;  // Matriz de transformação 4x4.
};

#endif
