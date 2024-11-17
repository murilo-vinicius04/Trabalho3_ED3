#include "registro.hpp"

Registro::Registro(std::ifstream& arquivo, int offset)
{
    // arruma o ponteiro para o inicio do registro e le os valores
    arquivo.seekg(offset, std::ios::beg);
    arquivo.read(reinterpret_cast<char*>(&_removido), sizeof(_removido));
    arquivo.read(reinterpret_cast<char*>(&_encadenamento), sizeof(_encadenamento));
    // toda especie precisa ter um nome valido, salvamos posicao e vemos se especie comeca com $
    std::streampos posicao = arquivo.tellg();
    arquivo.seekg(posicao + static_cast<std::streamoff>(13));
    if (arquivo.get() == '$')
        return;
    // volta a ler o registro
    arquivo.seekg(posicao);
    arquivo.read(reinterpret_cast<char*>(&_populacao), sizeof(_populacao));
    arquivo.read(reinterpret_cast<char*>(&_tamanho), sizeof(_tamanho));
    arquivo.read(reinterpret_cast<char*>(&_unidadeMedida), sizeof(_unidadeMedida));
    arquivo.read(reinterpret_cast<char*>(&_velocidade), sizeof(_velocidade));
    std::getline(arquivo, _nome, '#');
    std::getline(arquivo, _especie, '#');
    std::getline(arquivo, _habitat, '#');
    std::getline(arquivo, _tipo, '#');
    std::getline(arquivo, _dieta, '#');
    std::getline(arquivo, _alimento, '#');
}
