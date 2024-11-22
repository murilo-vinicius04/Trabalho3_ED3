// Pedro Fuziwara Filho - 13676840
// Murilo Vinicius da Silva - 14600030

#include "registro.hpp"

Registro::Registro(std::ifstream& arquivo, int offset)
{
    // arruma o ponteiro para o inicio do registro e le os valores
    arquivo.seekg(offset, std::ios::beg);
    arquivo.read((char*)&_removido, sizeof(_removido));
    arquivo.read((char*)&_encadenamento, sizeof(_encadenamento));
    // toda especie precisa ter um nome valido, salvamos posicao e vemos se especie comeca com $
    std::streampos posicao = arquivo.tellg();
    arquivo.seekg(posicao + (std::streamoff)13);
    if (arquivo.get() == '$') {
        return;
    }
    // volta a ler o registro
    arquivo.seekg(posicao);
    arquivo.read((char*)&_populacao, sizeof(_populacao));
    arquivo.read((char*)&_tamanho, sizeof(_tamanho));
    arquivo.read((char*)&_unidadeMedida, sizeof(_unidadeMedida));
    arquivo.read((char*)&_velocidade, sizeof(_velocidade));
    std::getline(arquivo, _nome, '#');
    std::getline(arquivo, _especie, '#');
    std::getline(arquivo, _habitat, '#');
    std::getline(arquivo, _tipo, '#');
    std::getline(arquivo, _dieta, '#');
    std::getline(arquivo, _alimento, '#');
}

void Registro::printa_formatado()
{
    std::cout << "nome: " << _nome << std::endl
        << "especie: " << _especie << std::endl
        << "habitat: " << _habitat << std::endl
        << "tipo: " << _tipo << std::endl
        << "dieta: " << _dieta << std::endl
        << "alimento: " << _alimento << std::endl
        << "populacao: " << _populacao << std::endl
        << "tamanho: " << _tamanho << std::endl
        << "velocidade: " << _velocidade 
        << " " << _unidadeMedida << "m" << std::endl;
}