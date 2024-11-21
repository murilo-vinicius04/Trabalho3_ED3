// Pedro Fuziwara Filho - 13676840

#include "grafo.hpp"
#include <climits>

Aresta::Aresta(Registro origem)
{
    _peso = origem.populacao();
    _nome = origem.alimento();
}

Aresta::Aresta(std::string nome, int peso)
{
    _nome = nome;
    _peso = peso;
}

void Aresta::printa()
{
    std::cout << _nome << " " << _peso << std::endl;
}

Aresta& Aresta::operator=(const Aresta& aresta)
{
    if (this == &aresta) return *this;
    _peso = aresta._peso;
    _nome = aresta._nome;
    return *this;
}

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

Vertice::Vertice(const Vertice& vertice)
    : _nome(vertice._nome), _especie(vertice._especie), _dieta(vertice._dieta),
    _tipo(vertice._tipo), _habitat(vertice._habitat), _grau_entrada(vertice._grau_entrada),
    _grau_saida(vertice._grau_saida), _grau(vertice._grau), _arestas(vertice._arestas) {}

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

void Vertice::printa_vertice()
{
    std::cout << _nome << ", " << _especie << ", " << _habitat
              << ", " << _dieta << ", " << _tipo << ", "
              << _grau_entrada << ", " << _grau_saida 
              << ", " << _grau << std::endl;
}

void Vertice::ordena_arestas()
{
    struct
    {
        bool operator()(Aresta a, Aresta b){return a.nome() < b.nome();}
    } compara_aresta;
    _arestas.sort(compara_aresta);
}

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

void Vertice::poe_no_fim(const Aresta& aresta)
{
    _arestas.push_back(aresta);
}

bool Vertice::caca(std::string presa)
{
    for (Aresta aresta : _arestas)
        if (aresta.nome() == presa)
            return true;
    return false;
}

void Vertice::limpa_arestas()
{
    _arestas.clear();
}

void Vertice::adiciona_aresta(const Aresta& aresta)
{
    _arestas.push_back(aresta);
}

Grafo::Grafo(std::ifstream& arquivo)
{
    // lemos registros e ordenamos a partir do nome
    arquivo.seekg(0, std::ios::end);
    int tamanho = arquivo.tellg();
    std::vector<Registro> registros;
    for (int offset = 1600; offset < tamanho; offset += 160)
    {
        Registro registro(arquivo, offset);
        registros.push_back(registro);
    }

    // precisamos deixar em ordem alfabetica
    struct
    {
        bool operator()(Registro a, Registro b){return a.nome() < b.nome();}
    } compara_nome;    
    std::sort(registros.begin(), registros.end(), compara_nome);

    for (const auto& registro : registros)
    {
        if (_adjacencias.empty() || _adjacencias.back().nome() != registro.nome())
        {
            _adjacencias.push_back(Vertice(registro));
        }
    }

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

Grafo& Grafo::operator=(const Grafo& grafo)
{
    if (this == &grafo) return *this;
    _adjacencias = grafo._adjacencias;
    return *this;
}

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

void Grafo::insere_aresta(Aresta aresta, int origem, int destino)
{
    if (origem < 0 || origem >= _adjacencias.size() || destino < 0 || destino >= _adjacencias.size())
        return;

    for (const auto& atual : _adjacencias[origem].arestas())
        if (atual.nome() == aresta.nome())
            return;

    _adjacencias[origem].insertion_sort(aresta);
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
            std::cout << vertice.nome() << " " << vertice.especie() << " " << vertice.habitat()
                      << " " << vertice.dieta() << " " << vertice.tipo() << " "
                      << vertice.grau_entrada() << " " << vertice.grau_saida() << " " << vertice.grau()
                      << " " << aresta.nome() << " " << aresta.peso() << "\n";
        }
    }
}

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

void Grafo::printa_cacadores(std::string presa)
{
    // buscamos cacadores
    std::vector<std::string> cacadores;
    for (int i = 0; i < this->tamanho(); i++)
        if (_adjacencias[i].caca(presa))
            cacadores.push_back(_adjacencias[i].nome());
    // checa se vazio
    if (cacadores.empty())
    {
        std::cout << "Registro Inexistente." << std::endl;
        return;
    }
    std::sort(cacadores.begin(), cacadores.end());
    // printamos cacadores
    std::cout << presa << ": ";
    for (long unsigned int i = 0; i < cacadores.size(); i++)
    {
       if (i != cacadores.size() - 1)
          std::cout << cacadores[i] << ", ";
       else
          std::cout << cacadores[i] << std::endl; 
    }
}

void busca_ciclos(Grafo& grafo, int v, int start, std::vector<bool>& visitado, int& contador, std::vector<int>& caminho)
{
    visitado[v] = true;
    caminho.push_back(v);

    for (auto& aresta : grafo.adjacencias()[v].arestas())
    {
        int destino = grafo.obter_indice_vertice(aresta.nome());
        if (destino == -1) { // Corrigido para verificar se destino é inválido
            // Registro não encontrado, pular esta aresta
            continue;
        }
        if (!visitado[destino])
        {
            busca_ciclos(grafo, destino, start, visitado, contador, caminho);
        }
        else if (destino == start)
        {
            contador++;
        }
    }

    visitado[v] = false;
    caminho.pop_back();
}

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

int Grafo::obter_indice_vertice(const std::string& nome_completo)
{
    for (size_t i = 0; i < _adjacencias.size(); ++i)
    {
        if (_adjacencias[i].nome() == nome_completo || _adjacencias[i].especie() == nome_completo)
            return i;
    }
    return -1; 
}

std::pair<std::vector<std::string>, int> Grafo::menor_caminho(const std::string& origem_nome, const std::string& destino_nome)
{
    int origem = obter_indice_vertice(origem_nome);
    int destino = obter_indice_vertice(destino_nome);

    if (origem == -1 || destino == -1) {
        return { {}, -1 };
    }

    std::vector<int> dist(_adjacencias.size(), INT_MAX);
    std::vector<int> prev(_adjacencias.size(), -1);
    dist[origem] = 0;

    std::vector<std::pair<int, int>> pq; 
    pq.push_back({0, origem});

    while (!pq.empty()) {
        auto min_it = std::min_element(pq.begin(), pq.end());
        int u = min_it->second;
        int d = min_it->first;
        pq.erase(min_it);

        if (u == destino) break; // Otimização: pare se alcançar o destino

        if (d > dist[u]) continue; // Já encontramos um caminho melhor

        for (const auto& aresta : _adjacencias[u].arestas()) {
            int v = obter_indice_vertice(aresta.nome());
            if (v == -1) continue; // Vértice não encontrado

            int peso = aresta.peso(); // População do predador
            if (peso == -1) continue; // Ignorar arestas inválidas

            if (dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                prev[v] = u;
                pq.push_back({dist[v], v});
            }
        }
    }

    if (dist[destino] == INT_MAX) {
        return { {}, -1 };
    }

    std::vector<std::string> caminho;
    for (int at = destino; at != -1; at = prev[at]) {
        caminho.push_back(_adjacencias[at].nome());
    }
    std::reverse(caminho.begin(), caminho.end());

    return { caminho, dist[destino] };
}

void Grafo::DFS(int v, std::vector<bool>& visitado, std::vector<int>& pilha)
{
    visitado[v] = true;
    for (const auto& aresta : _adjacencias[v].arestas())
    {
        int destino = obter_indice_vertice(aresta.nome());
        if (destino != -1 && !visitado[destino])
        {
            DFS(destino, visitado, pilha);
        }
    }
    pilha.push_back(v); 
}

void Grafo::DFSUtil(int v, std::vector<bool>& visitado)
{
    visitado[v] = true;
    for (const auto& aresta : _adjacencias[v].arestas())
    {
        int destino = obter_indice_vertice(aresta.nome());
        if (destino != -1 && !visitado[destino])
        {
            DFSUtil(destino, visitado);
        }
    }
}

Grafo Grafo::obter_transposto()
{
    Grafo grafo_transposto;

    // Copiar vértices
    grafo_transposto._adjacencias = _adjacencias;
    for (auto& vertice : grafo_transposto._adjacencias)
    {
        vertice.limpa_arestas(); 
    }

    for (size_t v = 0; v < _adjacencias.size(); ++v)
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

    for (int i = 0; i < n; ++i)
    {
        if (!visitado[i])
        {
            DFS(i, visitado, pilha);
        }
    }

    Grafo grafo_transposto = obter_transposto();

    std::fill(visitado.begin(), visitado.end(), false);
    int n_cfc = 0;

    while (!pilha.empty())
    {
        int v = pilha.back(); 
        pilha.pop_back(); 

        if (!visitado[v])
        {
            grafo_transposto.DFSUtil(v, visitado);
            ++n_cfc;
        }
    }

    if (n_cfc == 1)
    {
        std::cout << "O grafo é fortemente conexo.\n";
    }
    else
    {
        std::cout << "O grafo não é fortemente conexo.\n";
        std::cout << "Número de componentes fortemente conexos: " << n_cfc << "\n";
    }

    return n_cfc;
}