// Pedro Fuziwara Filho - 13676840

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

    // inicializa variáveis antes do switch
    std::vector<Registro> registros;
    Grafo grafo;

    // declara variáveis fora do switch
    std::streampos posicao;
    int tamanho;

    // executa funcionalidade desejada
    switch (n)
    {
        case 10:
        {
            grafo = Grafo(arquivo);
            grafo.printa_grafo(); // Certifique-se de que este método não está sendo chamado no case 13
            break;
        }
        case 11:
        {
            grafo = Grafo(arquivo);
            int numero_presas;
            std::cin >> numero_presas;
            for (int i = 0; i < numero_presas; i++)
            {
                std::string presa;
                std::getline(std::cin, presa, '"'); // descarta o primeiro ' "' de todo string
                std::getline(std::cin, presa, '"');
                grafo.printa_cacadores(presa);
                std::cout << std::endl;
            }
            break;
        }
        case 12:
        {
            // lê registros e constrói o grafo
            posicao = arquivo.tellg();
            arquivo.seekg(0, std::ios::end);
            tamanho = arquivo.tellg();
            arquivo.seekg(posicao, std::ios::beg);
            for (int offset = 1600; offset < tamanho; offset += 160)
            {
                Registro reg(arquivo, offset);
                if (reg.is_valido()) { // Adicionado verificação
                    registros.push_back(reg);
                } else {
                    // Opcional: Log de registros inválidos
                    std::cerr << "Registro inválido no offset " << offset << std::endl;
                }
            }
            grafo = Grafo(registros);
            // Remove the redundant edge insertion loop
            /*
            for (int origem = 0; origem < grafo.tamanho(); origem++)
            {
                for (int destino = 0; destino < grafo.tamanho(); destino++)
                    if (registros[origem].alimento() == registros[destino].nome())
                    {
                        grafo.insere_aresta(Aresta(registros[destino]), origem, destino);
                        break;
                    }
            }
            */
            // chama o método para contar ciclos simples
            int num_ciclos = grafo.conta_ciclos_simples();
            std::cout << "Quantidade de ciclos: " << num_ciclos << "\n";
            break;
        }
        case 13:
        {
            grafo = Grafo(arquivo);

            // Verifica se o grafo é fortemente conexo e conta o número de CFCs
            grafo.conta_componentes_fortemente_conexos();

            break;
        }
        case 14:
        {
            grafo = Grafo(arquivo); // Constrói o grafo a partir do arquivo

            int num_pares;
            std::cin >> num_pares;

            for (int i = 0; i < num_pares; i++) {
                std::string especie_origem, especie_destino;

                // Ler as espécies entre aspas
                std::cin.ignore(); // Ignora o espaço antes da primeira aspas
                std::getline(std::cin, especie_origem, '"'); // Descarta até a primeira aspas
                std::getline(std::cin, especie_origem, '"');
                std::cin.ignore(); // Ignora o espaço antes da segunda aspas
                std::getline(std::cin, especie_destino, '"'); // Descarta até a próxima aspas
                std::getline(std::cin, especie_destino, '"');

                // Debug messages
                std::cout << "Buscando caminho entre: " << especie_origem << " e " << especie_destino << std::endl;

                auto resultado = grafo.menor_caminho(especie_origem, especie_destino);

                if (resultado.second != -1) {
                    std::cout << especie_origem << " " << especie_destino << ": " << resultado.second << "\n";
                } else {
                    std::cout << especie_origem << " " << especie_destino << ": CAMINHO INEXISTENTE\n";
                }
            }
            break;
        }
    }

    // fecha o arquivo
    arquivo.close();

    return 0;
}