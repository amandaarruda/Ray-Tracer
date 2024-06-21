#include "sphere.h"  

// Método para verificar se um raio atinge a esfera
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    glm::vec3 oc = r.location() - center;  // Vetor do centro da esfera até a origem do raio
    float a = glm::dot(r.direction(), r.direction());  // Coeficiente 'a' da equação quadrática
    float b = glm::dot(oc, r.direction());  // Coeficiente 'b' da equação quadrática
    float c = glm::dot(oc, oc) - radius * radius;  // Coeficiente 'c' da equação quadrática
    float discriminant = b * b - a * c;  // Discriminante da equação quadrática

    // Verifica se o raio atinge a esfera
    if (discriminant > 0) {
        // Calcula o primeiro ponto de interseção
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;  // Atualiza o parâmetro 't' do registro de interseção
            rec.p = r.point_at_parameter(rec.t);  // Calcula o ponto de interseção
            rec.normal = (rec.p - center) / radius;  // Define o vetor normal ao ponto de interseção
            rec.cor = cor;  // Define a cor do objeto

            rec.kdif = objMaterial->kd;
            rec.kamb = objMaterial->ka;
            rec.kespc = objMaterial->ks;
            rec.rug = objMaterial->n;

            rec.kref = objMaterial->kr;
            rec.ktrans = objMaterial->kt;

            return true;  // Retorna true para indicar que houve uma interseção
        }
        // Calcula o segundo ponto de interseção
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;  // Atualiza o parâmetro 't' do registro de interseção
            rec.p = r.point_at_parameter(rec.t);  // Calcula o ponto de interseção
            rec.normal = (rec.p - center) / radius;  // Define o vetor normal ao ponto de interseção
            rec.cor = cor;  // Define a cor do objeto

            rec.kdif = objMaterial->kd;
            rec.kamb = objMaterial->ka;
            rec.kespc = objMaterial->ks;
            rec.rug = objMaterial->n;

            rec.kref = objMaterial->kr;
            rec.ktrans = objMaterial->kt;


            return true;  // Retorna true para indicar que houve uma interseção
        }
    }
    return false;  // Retorna false se não houve interseção
}

