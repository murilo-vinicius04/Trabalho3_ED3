#include "registro.hpp"
#include "grafo.hpp"

int main()
{
    // primeiro recebemos as informacoes que precisamos
    int n;
    std::string nome;
    std::cin >> n >> nome;

    // busca o arquivo
    std::ifstream arquivo;
    arquivo.open(nome);
    if (!arquivo.is_open())
    {
        printf("Falha no processamento do arquivo\n");
        return 1;
    }

    // executa funcionalidade desejada
    switch (n)
    {
    case 10:
        // lemos registros e ordenamos a partir do nome
        std::streampos posicao = arquivo.tellg();
        arquivo.seekg(0, std::ios::end);
        int tamanho = arquivo.tellg();
        arquivo.seekg(posicao, std::ios::beg);
        std::vector<Registro> registros;
        for (int offset = 1600, i = 0; offset < tamanho; offset += 160, i++)
        {
            registros.push_back(Registro(arquivo, offset));
        }
        // criamos um grafo com os registros ordenados, no momento ele so possui os vertice
        // sem nenhuma aresta
        Grafo grafo(registros);
        // vamos inserir as arestas
        for (int origem = 0; origem < grafo.tamanho(); origem++)
        {
            // para cada alimento, precisamos buscar registro para conseguir sua populacao
            for (int destino = 0; destino < grafo.tamanho(); destino++)
                if (registros[origem].alimento() == registros[destino].nome())
                {
                    grafo.insere_aresta(registros, origem, destino);
                    break;
                }
        }

        grafo.printa_grafo();
        break;
    }

    // fecha o arquivo
    arquivo.close();

    return 0;
}
