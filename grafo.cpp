// Pedro Fuziwara Filho - 13676840

#include "grafo.hpp"
#include <queue>
#include <climits>
#include <stack>

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

Vertice::Vertice(Vertice&& vertice) noexcept
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
    // Leitura e criação dos registros
    arquivo.seekg(0, std::ios::end);
    int tamanho = arquivo.tellg();
    std::vector<Registro> registros;
    for (int offset = 1600; offset < tamanho; offset += 160)
    {
        Registro registro(arquivo, offset);
        registros.push_back(registro);
    }

    // Ordenação dos registros
    std::sort(registros.begin(), registros.end(), [](const Registro& a, const Registro& b){
        return a.nome() < b.nome();
    });

    // Criação dos vértices únicos a partir dos registros
    for (const auto& registro : registros)
    {
        if (_adjacencias.empty() || _adjacencias.back().nome() != registro.nome())
        {
            _adjacencias.push_back(Vertice(registro));
        }
    }

    // Inicialização do mapeamento antes de inserir as arestas
    for (size_t i = 0; i < _adjacencias.size(); ++i)
    {
        nome_para_indice[_adjacencias[i].nome()] = i;
    }

    // Adicionar vértices para todas as espécies mencionadas como alimento, incluindo "sunlight"
    for (const auto& registro : registros)
    {
        Aresta aresta(registro);
        if (aresta.peso() != -1) // Removido o teste aresta.nome() != "sunlight"
        {
            if (nome_para_indice.find(aresta.nome()) == nome_para_indice.end())
            {
                // Criar um novo vértice para esta espécie com valores padrão
                Vertice novo_vertice(aresta.nome());
                _adjacencias.push_back(novo_vertice);
                nome_para_indice[aresta.nome()] = _adjacencias.size() - 1;
            }
        }
    }

    // Atualizar o mapeamento nome_para_indice após adicionar novos vértices
    for (size_t i = 0; i < _adjacencias.size(); ++i)
    {
        nome_para_indice[_adjacencias[i].nome()] = i;
    }

    // Inserção das arestas
    for (const auto& registro : registros)
    {
        Aresta aresta(registro);
        int origem = obter_indice_vertice(registro.nome());
        if (origem == -1)
            continue; // Vértice de origem não encontrado

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

Grafo::Grafo(std::vector<Registro> registros)
{
    // Filtrar registros válidos
    std::vector<Registro> registros_validos;
    for (const auto& reg : registros) {
        if (reg.is_valido()) {
            registros_validos.push_back(reg);
        }
    }

    // precisamos deixar em ordem alfabetica
    struct
    {
        bool operator()(Registro a, Registro b){return a.nome() < b.nome();}
    } compara_nome;    
    std::sort(registros_validos.begin(), registros_validos.end(), compara_nome);
    // criamos um grafo com os registros ordenados, no momento ele so possui os vertice
    // sem nenhuma aresta
    for (auto registro = registros_validos.begin(); registro != registros_validos.end(); registro++)
    {
        if (registro == registros_validos.begin())
            _adjacencias.push_back(Vertice(*registro));
        else if (_adjacencias.back().nome() != registro->nome())
            _adjacencias.push_back(Vertice(*registro));
    }
    // vamos inserir as arestas
    for (long unsigned int i = 0; i < registros_validos.size(); i++)
    {
        Aresta aresta(registros_validos[i]);
        int origem;
        // elemento repetido
        for (origem = 0; origem < this->tamanho(); origem++)
            if (_adjacencias[origem].nome() == registros_validos[i].nome())
                break;
        // faz fotossintese
        if (aresta.nome() == "sunlight")
            this->insere_sunlight(aresta, origem);
        // aresta existe
        else if (aresta.peso() != -1)
        {
            // para cada alimento, precisamos buscar destino na lista encadeada
            int destino = this->obter_indice_vertice(aresta.nome());
            if (destino != -1) { // Adicionado verificação
                this->insere_aresta(aresta, origem, destino);
            }
            // Se destino == -1, a aresta não será inserida
        }
    }
    // Inicializar nome_para_indice
    for (size_t i = 0; i < _adjacencias.size(); ++i)
    {
        nome_para_indice[_adjacencias[i].nome()] = i;
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
    if (destino == -1)
    {
        // Opcional: Tratar erro caso "sunlight" não exista, mas agora deve existir
        return;
    }

    for (const auto& atual : _adjacencias[origem].arestas())
        if (atual.nome() == aresta.nome())
            return;

    _adjacencias[origem].insertion_sort(aresta);
    _adjacencias[origem].incrementa_saida();
    _adjacencias[origem].incrementa_grau();

    // Incrementar o grau de entrada e grau total do vértice "sunlight"
    _adjacencias[destino].incrementa_entrada();
    _adjacencias[destino].incrementa_grau();
}

void Grafo::insere_aresta(Aresta aresta, int origem, int destino)
{
    // Verificar se os índices são válidos
    if (origem < 0 || origem >= _adjacencias.size() || destino < 0 || destino >= _adjacencias.size())
        return;

    // Verificar se a aresta já existe
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

int Grafo::obter_indice_vertice(const std::string& nome)
{
    auto it = nome_para_indice.find(nome);
    if (it != nome_para_indice.end())
        return it->second;
    else
        return -1; // Caso não encontrado
}

std::pair<std::vector<std::string>, int> Grafo::menor_caminho(const std::string& origem_nome, const std::string& destino_nome)
{
    int origem = obter_indice_vertice(origem_nome);
    int destino = obter_indice_vertice(destino_nome);

    if (origem == -1 || destino == -1) {
        std::cout << "Espécie não encontrada no grafo.\n";
        return { {}, -1 };
    }

    std::vector<int> dist(_adjacencias.size(), INT_MAX);
    std::vector<int> prev(_adjacencias.size(), -1);
    dist[origem] = 0;

    using pii = std::pair<int, int>; // (distância, índice do vértice)
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;
    pq.push({0, origem});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

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
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[destino] == INT_MAX) {
        std::cout << "Não há caminho entre as espécies.\n";
        return { {}, -1 };
    }

    // Reconstruir o caminho
    std::vector<std::string> caminho;
    for (int at = destino; at != -1; at = prev[at]) {
        caminho.push_back(_adjacencias[at].nome());
    }
    std::reverse(caminho.begin(), caminho.end());

    return { caminho, dist[destino] };
}

void Grafo::DFS(int v, std::vector<bool>& visitado, std::stack<int>& pilha)
{
    visitado[v] = true;
    for (const auto& aresta : _adjacencias[v].arestas())
    {
        auto it = nome_para_indice.find(aresta.nome());
        if (it != nome_para_indice.end())
        {
            int destino = it->second;
            if (!visitado[destino])
            {
                DFS(destino, visitado, pilha);
            }
        }
    }
    pilha.push(v);
}

void Grafo::DFSUtil(int v, std::vector<bool>& visitado)
{
    visitado[v] = true;
    for (const auto& aresta : _adjacencias[v].arestas())
    {
        auto it = nome_para_indice.find(aresta.nome());
        if (it != nome_para_indice.end())
        {
            int destino = it->second;
            if (!visitado[destino])
            {
                DFSUtil(destino, visitado);
            }
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
        vertice.limpa_arestas(); // Limpa as arestas
    }

    // Copiar o mapeamento
    grafo_transposto.nome_para_indice = nome_para_indice;

    // Adicionar as arestas invertidas
    for (size_t v = 0; v < _adjacencias.size(); ++v)
    {
        for (const auto& aresta : _adjacencias[v].arestas())
        {
            auto it = nome_para_indice.find(aresta.nome());
            if (it != nome_para_indice.end())
            {
                int destino = it->second;
                // Inverter a aresta
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
    std::stack<int> pilha;

    // Passo 1: Preencher a pilha com a ordem de término dos vértices
    for (int i = 0; i < n; ++i)
    {
        if (!visitado[i])
        {
            DFS(i, visitado, pilha);
        }
    }

    // Passo 2: Obter o grafo transposto
    Grafo grafo_transposto = obter_transposto();

    // Passo 3: Realizar DFS no grafo transposto na ordem da pilha
    std::fill(visitado.begin(), visitado.end(), false);
    int num_cfc = 0;

    while (!pilha.empty())
    {
        int v = pilha.top();
        pilha.pop();

        if (!visitado[v])
        {
            grafo_transposto.DFSUtil(v, visitado);
            ++num_cfc;
        }
    }

    // Verificar se o grafo é fortemente conexo
    if (num_cfc == 1)
    {
        std::cout << "O grafo é fortemente conexo.\n";
    }
    else
    {
        std::cout << "O grafo não é fortemente conexo.\n";
        std::cout << "Número de componentes fortemente conexos: " << num_cfc << "\n";
    }

    return num_cfc;
}