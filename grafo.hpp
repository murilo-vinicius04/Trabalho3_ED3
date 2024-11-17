#ifndef GRAFO
#define GRAFO

#include "registro.hpp"
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <vector>
#include <string>
#include <list>

class Aresta
{
    int _peso;
    std::string _nome;
public:
    Aresta(Registro registro);
    std::string nome(){return _nome;}
    int peso(){return _peso;}
};

class Vertice 
{
    // caracteristicas do dinossauro
    std::string _nome;
    std::string _especie;
    std::string _dieta;
    std::string _tipo;
    std::string _habitat;
    // caracteristicas do vertice
    int _grau_entrada;
    int _grau_saida;
    int _grau;
    // lista encadeada
    std::list<Aresta> _arestas;
public:
    Vertice(Registro registro);
    std::string nome(){return _nome;}
    std::string especie(){return _especie;}
    std::string dieta(){return _dieta;}
    std::string tipo(){return _tipo;}
    std::string habitat(){return _habitat;}
    std::list<Aresta> arestas(){return _arestas;}
    int grau_entrada(){return _grau_entrada;}
    int grau_saida(){return _grau_saida;}
    int grau(){return _grau;}
    void poe_no_fim(Aresta aresta){_arestas.push_back(aresta);}
    void incrementa_entrada(){_grau_entrada++;}
    void incrementa_saida(){_grau_saida++;}
    void incrementa_grau(){_grau++;}
};

class Grafo
{
    std::vector<Vertice> _adjacencias;
public:
    Grafo(std::vector<Registro> registros);
    Vertice vertice(int posicao){return _adjacencias[posicao];}
    int tamanho(){return _adjacencias.size();}
    void insere_aresta(std::vector<Registro> registros, int origem, int destino);
    void printa_grafo();
};

#endif
