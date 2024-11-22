// Pedro Fuziwara Filho - 13676840
// Murilo Vinicius da Silva - 14600030

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
            grafo.printa_grafo();
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
            grafo = Grafo(arquivo); 
            // chama o método para contar ciclos simples
            int num_ciclos = grafo.conta_ciclos_simples();
            std::cout << "Quantidade de ciclos: " << num_ciclos << "\n";
            break;
        }
        case 13:
        {
            grafo = Grafo(arquivo); 

            grafo.conta_componentes_fortemente_conexos();

            break;
        }
        case 14:
        {
            grafo = Grafo(arquivo); 

            int num_pares;
            std::cin >> num_pares;

            for (int i = 0; i < num_pares; i++) {
                std::string predador, alimento;

                // ignora espaços vazios e descarta aspas
                std::cin.ignore();
                std::getline(std::cin, predador, '"'); 
                std::getline(std::cin, predador, '"');
                std::cin.ignore();
                std::getline(std::cin, alimento, '"');
                std::getline(std::cin, alimento, '"');

                // chama o método menor_caminho para encontrar o menor caminho entre predador e alimento
                auto resultado = grafo.menor_caminho(predador, alimento);

                // verifica se o caminho existe
                if (resultado.second != -1) {
                    // imprime o predador, alimento e a distância do menor caminho
                    std::cout << predador << " " << alimento << ": " << resultado.second << "\n";
                } else {
                    std::cout << predador << " " << alimento << ": CAMINHO INEXISTENTE\n";
                }
            }
            break;
        }
    }

    // fecha o arquivo
    arquivo.close();

    return 0;
}