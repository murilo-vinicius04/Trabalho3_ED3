// Pedro Fuziwara Filho - 13676840
// Murilo Vinicius da Silva - 14600030

#include "grafo.hpp"
#include <climits>

// criamos uma aresta a partir de um registro
Aresta::Aresta(Registro origem)
{
    _peso = origem.populacao();
    _nome = origem.alimento();
}

// criamos uma aresta com nome e peso especifico
Aresta::Aresta(std::string nome, int peso)
{
    _nome = nome;
    _peso = peso;
}

// imprimimos os detalhes da aresta
void Aresta::printa()
{
    std::cout << _nome << " " << _peso << std::endl;
}

// sobrecarregamos o operador de atribuicao para arestas
Aresta& Aresta::operator=(const Aresta& aresta)
{
    if (this == &aresta) return *this;
    _peso = aresta._peso;
    _nome = aresta._nome;
    return *this;
}

// criamos um vertice a partir de um registro de especie
Vertice::Vertice(Registro especie)
{
    _nome = especie.nome();
    _especie = especie.especie();
    _dieta = especie.dieta();
    _tipo = especie.tipo();
    _habitat = especie.habitat();
    _grau_entrada = 0;
    _grau_saida = 0;
    _grau = 0;
}

// movemos um vertice para outro objeto vertice
Vertice::Vertice(Vertice&& vertice)
    : _nome(std::move(vertice._nome)), _especie(std::move(vertice._especie)),
    _dieta(std::move(vertice._dieta)), _tipo(std::move(vertice._tipo)),
    _habitat(std::move(vertice._habitat)), _grau_entrada(vertice._grau_entrada),
    _grau_saida(vertice._grau_saida), _grau(vertice._grau), 
    _arestas(std::move(vertice._arestas))
{
    vertice._grau_entrada = 0;
    vertice._grau_saida = 0;
    vertice._grau = 0;
}

// criamos uma copia de um vertice
Vertice::Vertice(const Vertice& vertice)
    : _nome(vertice._nome), _especie(vertice._especie), _dieta(vertice._dieta),
    _tipo(vertice._tipo), _habitat(vertice._habitat), _grau_entrada(vertice._grau_entrada),
    _grau_saida(vertice._grau_saida), _grau(vertice._grau), _arestas(vertice._arestas) {}

// sobrecarregamos o operador de atribuicao para vertices
Vertice& Vertice::operator=(const Vertice& vertice)
{
    if (this == &vertice) return *this;
    _nome = std::move(vertice._nome);
    _especie = std::move(vertice._especie);
    _dieta = std::move(vertice._dieta);
    _tipo = std::move(vertice._tipo);
    _habitat = std::move(vertice._habitat);
    _arestas = std::move(vertice._arestas);
    _grau_entrada = _grau_entrada;
    _grau_saida = _grau_saida;
    _grau = _grau;
    return *this;
}

// imprimimos os detalhes do vertice
void Vertice::printa_vertice()
{
    std::cout << _nome << ", " << _especie << ", " << _habitat
              << ", " << _dieta << ", " << _tipo << ", "
              << _grau_entrada << ", " << _grau_saida 
              << ", " << _grau << std::endl;
}

// ordenamos as arestas alfabeticamente pelo nome
void Vertice::ordena_arestas()
{
    struct
    {
        bool operator()(Aresta a, Aresta b){return a.nome() < b.nome();}
    } compara_aresta;
    _arestas.sort(compara_aresta);
}

// inserimos uma aresta na posicao correta usando insertion sort
void Vertice::insertion_sort(Aresta aresta)
{
    if (_arestas.empty())
        _arestas.push_front(aresta);
    else
    {
        auto posicao = _arestas.begin();
        for (; posicao != _arestas.end(); posicao++)
            if (posicao->nome() >= aresta.nome())
                break;
        _arestas.insert(posicao, aresta);
     }
}

// adicionamos uma aresta no fim da lista de arestas
void Vertice::poe_no_fim(const Aresta& aresta)
{
    _arestas.push_back(aresta);
}

// verificamos se o vertice caca uma presa especifica
bool Vertice::caca(std::string presa)
{
    for (Aresta aresta : _arestas)
        if (aresta.nome() == presa)
            return true;
    return false;
}

// limpamos todas as arestas do vertice
void Vertice::limpa_arestas()
{
    _arestas.clear();
}

// adicionamos uma aresta ao vertice
void Vertice::adiciona_aresta(const Aresta& aresta)
{
    _arestas.push_back(aresta);
}

// construimos o grafo a partir de um arquivo
Grafo::Grafo(std::ifstream& arquivo)
{
    // lemos os registros e ordenamos a partir do nome
    arquivo.seekg(0, std::ios::end);
    int tamanho = arquivo.tellg();
    std::vector<Registro> registros;
    for (int offset = 1600; offset < tamanho; offset += 160)
    {
        Registro registro(arquivo, offset);
        registros.push_back(registro);
    }

    // ordenamos os registros alfabeticamente pelo nome
    struct
    {
        bool operator()(Registro a, Registro b){return a.nome() < b.nome();}
    } compara_nome;    
    std::sort(registros.begin(), registros.end(), compara_nome);

    // criamos os vertices a partir dos registros
    for (const auto& registro : registros)
    {
        if (_adjacencias.empty() || _adjacencias.back().nome() != registro.nome())
        {
            _adjacencias.push_back(Vertice(registro));
        }
    }

    // adicionamos vertices para arestas com peso valido
    for (const auto& registro : registros)
    {
        Aresta aresta(registro);
        if (aresta.peso() != -1) 
        {
            if (obter_indice_vertice(aresta.nome()) == -1)
            {
                Vertice novo_vertice(aresta.nome());
                _adjacencias.push_back(novo_vertice);
            }
        }
    }

    // inserimos as arestas no grafo
    for (const auto& registro : registros)
    {
        Aresta aresta(registro);
        int origem = obter_indice_vertice(registro.nome());
        if (origem == -1)
            continue; 

        int destino = obter_indice_vertice(aresta.nome());
        if (aresta.nome() == "sunlight")
        {
            insere_sunlight(aresta, origem);
        }
        else if (aresta.peso() != -1 && destino != -1)
        {
            insere_aresta(aresta, origem, destino);
        }
    }
}

// sobrecarregamos o operador de atribuicao para grafo
Grafo& Grafo::operator=(const Grafo& grafo)
{
    if (this == &grafo) return *this;
    _adjacencias = grafo._adjacencias;
    return *this;
}

// inserimos uma aresta normal no grafo
void Grafo::insere_aresta(Aresta aresta, int origem, int destino)
{
    // verificamos indices validos
    if (origem < 0 || origem >= _adjacencias.size() || destino < 0 || destino >= _adjacencias.size())
        return;

    // verificamos se a aresta ja existe
    for (const auto& atual : _adjacencias[origem].arestas())
        if (atual.nome() == aresta.nome())
            return;

    // inserimos a aresta ordenadamente
    _adjacencias[origem].insertion_sort(aresta);
    _adjacencias[origem].incrementa_saida();
    _adjacencias[origem].incrementa_grau();

    // atualizamos os graus do destino
    _adjacencias[destino].incrementa_entrada();
    _adjacencias[destino].incrementa_grau();
}

// inserimos uma aresta especial "sunlight" no grafo
void Grafo::insere_sunlight(Aresta aresta, int origem)
{
    int destino = obter_indice_vertice("sunlight");

    for (const auto& atual : _adjacencias[origem].arestas())
        if (atual.nome() == aresta.nome())
            return;

    _adjacencias[origem].insertion_sort(aresta);
    _adjacencias[origem].incrementa_saida();
    _adjacencias[origem].incrementa_grau();
    _adjacencias[destino].incrementa_entrada();
    _adjacencias[destino].incrementa_grau();
}

// imprimimos todos os vertices e suas arestas
void Grafo::printa_grafo()
{
    for (auto vertice : _adjacencias)
    {
        for (auto aresta : vertice.arestas()) 
        {
            std::cout << vertice.nome() << " " << vertice.especie() << " " << vertice.habitat()
                      << " " << vertice.dieta() << " " << vertice.tipo() << " "
                      << vertice.grau_entrada() << " " << vertice.grau_saida() << " " << vertice.grau()
                      << " " << aresta.nome() << " " << aresta.peso() << "\n";
        }
    }
}

// escrevemos os detalhes do grafo em um arquivo
void Grafo::escreve(std::string nome)
{
    std::ofstream arquivo;
    arquivo.open(nome, std::ofstream::out | std::ofstream::trunc);
    for (auto vertice : _adjacencias)
    {
        for (auto aresta : vertice.arestas()) 
        {
            arquivo << vertice.nome() << " " << vertice.especie() << " " << vertice.habitat()
                    << " " << vertice.dieta() << " " << vertice.tipo() << " "
                    << vertice.grau_entrada() << " " << vertice.grau_saida() << " " << vertice.grau()
                    << " " << aresta.nome() << " " << aresta.peso() << "\n";
        }
    }
    arquivo << std::endl;
    arquivo.close();
}

// imprimimos todos os vertices do grafo
void Grafo::printa_vertices()
{
    for (auto vertice : _adjacencias)
    {
        std::cout << vertice.nome() << ", " << vertice.especie() << ", " << vertice.habitat()
                  << ", " << vertice.dieta() << ", " << vertice.tipo() << ", "
                  << vertice.grau_entrada() << ", " << vertice.grau_saida() 
                  << ", " << vertice.grau() << std::endl;
    } 
}

// imprimimos os cacadores de uma presa especifica
void Grafo::printa_cacadores(std::string presa)
{
    // buscamos os cacadores da presa
    std::vector<std::string> cacadores;
    for (int i = 0; i < this->tamanho(); i++)
        if (_adjacencias[i].caca(presa))
            cacadores.push_back(_adjacencias[i].nome());
    // verificamos se encontrou algum cacador
    if (cacadores.empty())
    {
        std::cout << "Registro Inexistente." << std::endl;
        return;
    }
    // ordenamos os cacadores alfabeticamente
    std::sort(cacadores.begin(), cacadores.end());
    // imprimimos os cacadores
    std::cout << presa << ": ";
    for (long unsigned int i = 0; i < cacadores.size(); i++)
    {
       if (i != cacadores.size() - 1)
          std::cout << cacadores[i] << ", ";
       else
          std::cout << cacadores[i] << std::endl; 
    }
}

// buscamos ciclos no grafo usando DFS
void busca_ciclos(Grafo& grafo, int i, int inicio, std::vector<bool>& visitado, int& contador, std::vector<int>& caminho)
{
    visitado[i] = true;
    caminho.push_back(i);

    for (auto& aresta : grafo.adjacencias()[i].arestas())
    {
        int destino = grafo.obter_indice_vertice(aresta.nome());
        if (destino == -1) { 
            continue;
        }
        if (!visitado[destino])
        {
            busca_ciclos(grafo, destino, inicio, visitado, contador, caminho);
        }
        else if (destino == inicio)
        {
            contador++;
        }
    }

    visitado[i] = false;
    caminho.pop_back();
}

// contamos o numero de ciclos simples no grafo
int Grafo::conta_ciclos_simples()
{
    int contador = 0;
    int n = _adjacencias.size();
    std::vector<bool> visitado(n, false);
    std::vector<int> caminho;

    for (int i = 0; i < n; i++)
    {
        busca_ciclos(*this, i, i, visitado, contador, caminho);
        visitado[i] = true;
    }

    return contador;
}

// obtemos o indice de um vertice pelo nome completo
int Grafo::obter_indice_vertice(const std::string& nome_completo)
{
    for (int i = 0; i < _adjacencias.size(); ++i)
    {
        if (_adjacencias[i].nome() == nome_completo || _adjacencias[i].especie() == nome_completo)
            return i;
    }
    return -1; 
}

// encontramos o menor caminho entre duas especies usando Dijkstra
std::pair<std::vector<std::string>, int> Grafo::menor_caminho(const std::string& nome_origem, const std::string& nome_destino)
{
    int indice_origem = obter_indice_vertice(nome_origem);
    int indice_destino = obter_indice_vertice(nome_destino);

    // verificamos se origem ou destino nao existem
    if (indice_origem == -1 || indice_destino == -1) {
        return { {}, -1 };
    }

    std::vector<int> menor_distancia(_adjacencias.size(), INT_MAX);
    std::vector<int> especie_pai(_adjacencias.size(), -1);
    menor_distancia[indice_origem] = 0;

    std::vector<std::pair<int, int>> fila_prioridade;
    fila_prioridade.push_back({0, indice_origem});

    while (!fila_prioridade.empty()) {
        // selecionamos a especie com a menor distancia
        auto proxima_especie = std::min_element(fila_prioridade.begin(), fila_prioridade.end());
        int especie_atual = proxima_especie->second;
        int distancia_atual = proxima_especie->first;
        fila_prioridade.erase(proxima_especie);

        // paramos se chegamos ao destino
        if (especie_atual == indice_destino) break;

        // continuamos se a distancia atual for maior que a menor conhecida
        if (distancia_atual > menor_distancia[especie_atual]) continue;

        // atualizamos as distancias das especies vizinhas
        for (const auto& aresta : _adjacencias[especie_atual].arestas()) {
            int especie_vizinha = obter_indice_vertice(aresta.nome());
            if (especie_vizinha == -1) continue;

            int peso = aresta.peso();
            if (peso == -1) continue;

            if (menor_distancia[especie_atual] + peso < menor_distancia[especie_vizinha]) {
                menor_distancia[especie_vizinha] = menor_distancia[especie_atual] + peso;
                especie_pai[especie_vizinha] = especie_atual;
                fila_prioridade.push_back({menor_distancia[especie_vizinha], especie_vizinha});
            }
        }
    }

    // verificamos se nao existe caminho
    if (menor_distancia[indice_destino] == INT_MAX) {
        return { {}, -1 };
    }

    // constroimos o caminho a partir dos pais
    std::vector<std::string> caminho;
    for (int especie_atual = indice_destino; especie_atual != -1; especie_atual = especie_pai[especie_atual]) {
        caminho.push_back(_adjacencias[especie_atual].nome());
    }
    std::reverse(caminho.begin(), caminho.end());

    return { caminho, menor_distancia[indice_destino] };
}

// realizamos a busca em profundidade no grafo
void Grafo::DFS(int i, std::vector<bool>& visitado, std::vector<int>& pilha, bool empilhar)
{
    visitado[i] = true;
    for (const auto& aresta : _adjacencias[i].arestas())
    {
        int destino = obter_indice_vertice(aresta.nome());
        if (destino != -1 && !visitado[destino])
        {
            DFS(destino, visitado, pilha, empilhar);
        }
    }
    if (empilhar)
    {
        pilha.push_back(i); 
    }
}

// geramos o grafo transposto
Grafo Grafo::gerar_transposto()
{
    Grafo grafo_transposto;

    // copiamos os vertices
    grafo_transposto._adjacencias = _adjacencias;
    // limpamos as arestas do grafo transposto
    for (auto& vertice : grafo_transposto._adjacencias)
    {
        vertice.limpa_arestas(); 
    }

    // invertimos as arestas
    for (int v = 0; v < _adjacencias.size(); ++v)
    {
        for (const auto& aresta : _adjacencias[v].arestas())
        {
            int destino = obter_indice_vertice(aresta.nome());
            if (destino != -1)
            {
                Aresta aresta_invertida(_adjacencias[v].nome(), aresta.peso());
                grafo_transposto._adjacencias[destino].adiciona_aresta(aresta_invertida);
            }
        }
    }
    return grafo_transposto;
}

// contamos o numero de componentes fortemente conexos no grafo
int Grafo::conta_componentes_fortemente_conexos()
{
    int n = _adjacencias.size();
    if (n == 0)
    {
        std::cout << "Grafo vazio, nenhum componente fortemente conexo encontrado.\n";
        return 0;
    }

    std::vector<bool> visitado(n, false);
    std::vector<int> pilha; 

    // realizamos DFS para preencher a pilha
    for (int i = 0; i < n; ++i)
    {
        if (!visitado[i])
        {
            DFS(i, visitado, pilha, true);
        }
    }

    // geramos o grafo transposto
    Grafo grafo_transposto = gerar_transposto();

    // reiniciamos o vetor de visitados
    std::fill(visitado.begin(), visitado.end(), false);
    int n_cfc = 0;

    // realizamos DFS no grafo transposto seguindo a ordem da pilha
    while (!pilha.empty())
    {
        int v = pilha.back(); 
        pilha.pop_back(); 

        if (!visitado[v])
        {
            grafo_transposto.DFS(v, visitado, pilha, false);
            ++n_cfc;
        }
    }

    // imprimimos o resultado
    if (n_cfc == 1)
    {
        std::cout << "O grafo é fortemente conexo.\n";
    }
    else
    {
        std::cout << "Não, o grafo não e fortemente conexo e possui " << n_cfc << " componentes.\n";
    }

    return n_cfc;
}
