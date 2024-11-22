// Pedro Fuziwara Filho - 13676840
// Murilo Vinicius da Silva - 14600030

#ifndef GRAFO
#define GRAFO

#include "registro.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <list>

class Aresta
{
    int _peso;
    std::string _nome;
public:
    // construtores
    Aresta(Registro origem);
    Aresta(Aresta&& aresta) 
        : _peso(aresta._peso), _nome(std::move(aresta._nome))
    {aresta._peso = 0;}
    Aresta(const Aresta& aresta) : _peso(aresta._peso), _nome(aresta._nome) {}
    Aresta() : _peso(0), _nome(""){}
    Aresta(std::string nome, int peso); 
    // operadores
    Aresta& operator=(const Aresta& aresta);
    // metodos para acessar parametros
    std::string nome() const { return _nome; } 
    int peso() const { return _peso; } 
    void printa();
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
    // construtores
    Vertice() : _nome(""), _especie(""), _dieta(""), _tipo(""), _habitat(""),
        _grau_entrada(0), _grau_saida(0), _grau(0) {}
    Vertice(Registro registro);
    Vertice(Vertice&& vertice);
    Vertice(const Vertice& vertice);
    Vertice(const std::string& nome)
        : _nome(nome), _especie(""), _dieta(""), _tipo(""), _habitat(""),
          _grau_entrada(0), _grau_saida(0), _grau(0) {}
    // operadores
    Vertice& operator=(const Vertice& vertice);
    // metodos para acessar parametros
    std::string nome(){return _nome;}
    std::string especie(){return _especie;}
    std::string dieta(){return _dieta;}
    std::string tipo(){return _tipo;}
    std::string habitat(){return _habitat;}
    std::list<Aresta> arestas(){return _arestas;}
    int grau_entrada(){return _grau_entrada;}
    int grau_saida(){return _grau_saida;}
    int grau(){return _grau;}
    // metodos para o grafo
    void insertion_sort(const Aresta aresta);
    void poe_no_fim(const Aresta& aresta);
    void incrementa_entrada(){_grau_entrada++;}
    void incrementa_saida(){_grau_saida++;}
    void incrementa_grau(){_grau++;}
    void printa_vertice();
    void ordena_arestas();
    bool caca(std::string presa);
    void limpa_arestas();                       
    void adiciona_aresta(const Aresta& aresta); 
};

class Grafo
{
    std::vector<Vertice> _adjacencias;
public:
    // construtores
    Grafo() : _adjacencias(){}
    Grafo(const Grafo& grafo) : _adjacencias(grafo._adjacencias){}
    Grafo(Grafo&& grafo) noexcept : _adjacencias(std::move(grafo._adjacencias)){}
    Grafo(std::ifstream& arquivo);
    Grafo& operator=(const Grafo& grafo);
    // acessa parametros do grafo
    Vertice vertice(long unsigned int posicao){return _adjacencias[posicao];};
    int tamanho(){return _adjacencias.size();}
    std::vector<Vertice> adjacencias(){return _adjacencias;}
    // interage com grafo
    void insere_sunlight(Aresta aresta, int origem);
    void insere_aresta(Aresta aresta, int origem, int destino);
    void printa_grafo();
    void escreve(std::string nome);
    void printa_vertices();
    void printa_cacadores(std::string presa);
    int conta_ciclos_simples();
    int obter_indice_vertice(const std::string& nome);
    std::pair<std::vector<std::string>, int> menor_caminho(const std::string& origem_nome, const std::string& destino_nome);
    int conta_componentes_fortemente_conexos(); // Nova funcionalidade
private:
    void DFS(int v, std::vector<bool>& visitado, std::vector<int>& pilha, bool push_to_stack);                       
    Grafo gerar_transposto();                                             
};

#endif