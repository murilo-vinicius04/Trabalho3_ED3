#include "grafo.hpp"

Vertice::Vertice(Registro especie)
{
    _nome = especie.nome();
    _especie = especie.especie();
    _dieta = especie.dieta();
    _tipo = especie.tipo();
    _habitat = especie.dieta();
    _grau_entrada = 0;
    _grau_saida = 0;
    _grau = 0;
}

Aresta::Aresta(Registro destino)
{
    _peso = destino.populacao();
    _nome = destino.nome();
}

Grafo::Grafo(std::vector<Registro> registros)
{
    for (auto registro = registros.begin(); registro != registros.end(); registro++)
    {
        if (registro == registros.begin())
            _adjacencias.push_back(Vertice(*registro));
        else if (_adjacencias.back().nome() != registro->nome())
            _adjacencias.push_back(Vertice(*registro));
    }
}

void Grafo::insere_aresta(std::vector<Registro> registros, int origem, int destino)
{
    Aresta aresta(registros[destino]);
    _adjacencias[origem].poe_no_fim(aresta);
    _adjacencias[origem].incrementa_saida();
    _adjacencias[origem].incrementa_grau();
    _adjacencias[destino].incrementa_entrada();
    _adjacencias[destino].incrementa_grau();
}

void Grafo::printa_grafo()
{
    for (auto vertice : _adjacencias)
    {
        for (auto aresta : vertice.arestas()) 
        {
            std::cout << vertice.nome() << ", " << vertice.especie() << ", " << vertice.habitat()
                      << ", " << vertice.dieta() << ", " << vertice.tipo() << ", "
                      << vertice.grau_entrada() << ", " << vertice.grau_saida() << ", " << vertice.grau()
                      << ", " << aresta.nome() << ", " << aresta.peso() << "\n";
        }
    }
}
