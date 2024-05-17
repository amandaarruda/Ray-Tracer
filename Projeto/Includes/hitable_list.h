#ifndef HITABLE_LIST_H  // Garantir que o arquivo só será incluído uma vez
#define HITABLE_LIST_H

#include "hitable.h" 

// Classe para representar uma lista de objetos hitable
class hitable_list : public hitable {
public:
    // Construtores
    hitable_list() {}  // Construtor padrão
    hitable_list(hitable** l, int n) : list(l), list_size(n) {}  // Construtor com parâmetros

    // Método para verificar se um raio atinge algum objeto na lista de objetos hitable
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    hitable** list;  // Ponteiro para a lista de objetos hitable
    int list_size;  // Tamanho da lista de objetos hitable
};

#endif 
