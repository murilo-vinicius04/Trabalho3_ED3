// Pedro Fuziwara Filho - 13676840
// Murilo Vinicius da Silva - 14600030

#ifndef REGISTRO
#define REGISTRO

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

class Registro 
{
    int _populacao;          // campo 0
    float _tamanho;          // campo 1
    char _unidadeMedida;     // campo 2
    int _velocidade;         // campo 3
    std::string _nome;       // campo 4
    std::string _especie;    // campo 5
    std::string _habitat;    // campo 6
    std::string _tipo;       // campo 7
    std::string _dieta;      // campo 8
    std::string _alimento;   // campo 9
    char _removido;          // campo 10
    int _encadenamento;      // campo 11
public:
    Registro(std::ifstream& arquivo, int offset);
    std::string nome() const { return _nome; } 
    std::string especie() const { return _especie; } 
    std::string dieta() const { return _dieta; } 
    std::string tipo() const { return _tipo; } 
    std::string habitat() const { return _habitat; } 
    std::string alimento() const { return _alimento; } 
    int populacao() const { return _populacao; } 
    void printa_formatado();
};

#endif