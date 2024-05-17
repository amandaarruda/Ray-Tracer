#include "hitable_list.h"  

// Método para verificar se um raio atinge algum objeto na lista de objetos hitable
bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;  // Cria um registro temporário para armazenar informações sobre a interseção
    bool hit_anything = false;  // Inicializa a variável para indicar se houve alguma interseção
    double closest_so_far = t_max;  // Inicializa a variável para armazenar a distância do objeto mais próximo

    // Itera sobre todos os objetos na lista
    for (int i = 0; i < list_size; i++) {
        // Verifica se o raio atinge o objeto atual e se a interseção ocorre mais próxima do que a interseção atual mais próxima
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;  // Atualiza a flag para indicar que houve uma interseção
            closest_so_far = temp_rec.t;  // Atualiza a distância do objeto mais próximo
            rec = temp_rec;  // Atualiza o registro de interseção com as informações do objeto atingido
        }
    }
    return hit_anything;  // Retorna true se houve uma interseção, caso contrário, retorna false
}
