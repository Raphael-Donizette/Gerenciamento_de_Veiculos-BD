/*
 * Nome: Raphael Donizette da Silva Alves
 * Data: 05 de maio de 2026
 * Disciplina: AEDs 1 - Prática
 * Objetivo: realizar operações sobre um vetor de registros carregado a partir da base de dados de veiculos.
 * carregado a partir de uma base de dados de veículos.
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>


using namespace std;

// Função para limpar a tela do terminal
void limpa_tela() {
    #ifdef _WIN32
        system("cls");  // Se for Windows, usa cls
    #else
        system("clear"); // Se for Linux ou Mac, usa clear
    #endif
}

// Definição da estrutura para representar um veículo (Registro)
typedef struct
{
    int ano, km, portas;
    float potencia, valor;
    string modelo, marca, tipo, combustivel, cambio, direcao, cor, placa;
} veiculo;

// Função para inserir veiculos na base de dados (RAM)
bool insere(veiculo bd[], int *qtde, veiculo aux);

// Função para remover veiculos na base de dados (RAM)
bool remove(veiculo bd[], int *qtde, string placa_procurada);

// Função que reescreve o arquivo limpo com os dados atualizados da RAM
void atualizarArquivo(string nome_arquivo, veiculo bd[], int qtde)
{
    ofstream arquivo;

    // Abre o arquivo no modo padrão (sobrescreve o antigo para atualizar posições e remoções)
    arquivo.open(nome_arquivo);

    if (arquivo.is_open())
    {
        for (int i = 0; i < qtde; i++)
        {
            arquivo << bd[i].modelo << " " << bd[i].marca << " " << bd[i].tipo << " "
                    << bd[i].ano << " " << bd[i].km << " " << bd[i].potencia << " "
                    << bd[i].combustivel << " " << bd[i].cambio << " " << bd[i].direcao << " "
                    << bd[i].cor << " " << bd[i].portas << " " << bd[i].placa << " " << bd[i].valor << endl;
        }

        // Escreve a tag de parada na última linha do arquivo
        arquivo << "FIM" << endl;
        arquivo.close();
    }
}

const int tam = 50; // Tamanho máximo do vetor

// Função para exibir o menu e retornar a opção escolhida
int menu()
{
    int op;
    cout << "\n==============================================" << endl;
    cout << "     SISTEMA DE GERENCIAMENTO DE VEÍCULOS            " << endl;
    cout << "==============================================" << endl;
    cout << "1 - Calcular média dos preços." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "2 - Listar placas de veículos acima da média." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "3 - Buscar veículo pela placa." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "4 - Buscar veículos pelo câmbio." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "5 - Buscar por combustível e potência do motor." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "6 - Buscar veículos por faixa de valores." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "7 - Listar veículos." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "8 - Gerar relátorio da base de dados." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "9 - Adicionar um novo veículo." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "10 - Remover um veículo." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "0 - Sair." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Escolha uma opção: ";
    cin >> op;
    cout << "----------------------------------------------" << endl;
    return op;
}

void listar(veiculo bd[], int qtde)
{
    if (qtde == 0)
    {
        cout << "Nenhum veículo cadastrado na memória." << endl;
        return;
    }
    for (int i = 0; i < qtde; i++)
    {
        cout << bd[i].modelo << " " << bd[i].marca << " " << bd[i].tipo << " "
             << bd[i].ano << " " << bd[i].km << " " << bd[i].potencia << " "
             << bd[i].combustivel << " " << bd[i].cambio << " " << bd[i].direcao << " "
             << bd[i].cor << " " << bd[i].portas << " " << bd[i].placa << " "
             << bd[i].valor << endl;
    }
}

int main()
{
    system("chcp 65001 > nul");
    veiculo carro[tam]; // Vetor de registros (Alocação local na RAM)
    int qtde = 0, opcao;
    float soma_precos = 0, media = 0;
    string nome_arquivo = "BD_veiculos_2.txt";

    // 1ª ETAPA: Leitura dos dados do arquivo para o vetor
    ifstream entrada(nome_arquivo);
    if (!entrada)
    {
        cout << "Erro: Arquivo " << nome_arquivo << " nao encontrado!" << endl;
        return 1;
    }

    // Carregamento do vetor até encontrar "FIM" ou atingir o limite
    while (qtde < tam && entrada >> carro[qtde].modelo && carro[qtde].modelo != "FIM")
    {
        entrada >> carro[qtde].marca >> carro[qtde].tipo >> carro[qtde].ano >> carro[qtde].km >> carro[qtde].potencia >> carro[qtde].combustivel >> carro[qtde].cambio >> carro[qtde].direcao >> carro[qtde].cor >> carro[qtde].portas >> carro[qtde].placa >> carro[qtde].valor;

        soma_precos += carro[qtde].valor; // Acumula valor para a média
        qtde++;
    }
    entrada.close();

    // Cálculo prévio da média (necessário para a opção 1 e 2)
    if (qtde > 0)
    {
        media = soma_precos / qtde;
    }

    // 2ª ETAPA: Menu de operações (Switch Case)
    do
    {
        opcao = menu();

        switch (opcao)
        {
        case 1:
            // Recalcula a média antes de exibir para garantir valores reais pós inserção/remoção
            if (qtde > 0)
            {
                soma_precos = 0;
                for (int i = 0; i < qtde; i++)
                    soma_precos += carro[i].valor;
                media = soma_precos / qtde;
            }
            else
                media = 0;

            cout << "\n>> Média de preço dos veículos: R$ " << media << endl;
            break;

        case 2:
            cout << "\n>> Placas de veículos com valor acima da média:" << endl;
            for (int i = 0; i < qtde; i++)
            {
                if (carro[i].valor > media)
                {
                    cout << "- Placa: " << carro[i].placa << " (R$ " << carro[i].valor << ")" << endl;
                }
            }
            break;

        case 3:
        {
            string busca_placa;
            bool achou = false;
            cout << "*Digite a placa para buscar: ";
            cin >> busca_placa;
            cout << endl;
            // Algoritmo de busca sequencial no vetor
            for (int i = 0; i < qtde; i++)
            {
                if (carro[i].placa == busca_placa)
                {
                    cout << ">> Veiculo encontrado: " << carro[i].modelo << " (" << carro[i].marca << ")" << endl;
                    cout << "Ano: " << carro[i].ano << " | Preço: R$ " << carro[i].valor << endl;
                    achou = true;
                    break;
                }
            }
            if (!achou)
                cout << "Veículo com placa: " << busca_placa << " - não encontrado." << endl;
            break;
        }
        case 4:
        {
            string busca_cambio;
            bool achou = false;
            cout << "Câmbios disponíveis:" << endl;
            cout << "Manual | Automático" << endl;
            cout << "----------------------------------------------" << endl;
            cout << "*Digite o câmbio desejado: ";
            cin >> busca_cambio;
            cout << "\n--- Veículos com câmbio: " << busca_cambio << " ---" << endl;
            cout << endl;

            for (int i = 0; i < qtde; i++)
            {
                if (carro[i].cambio == busca_cambio)
                {
                    cout << "- " << carro[i].modelo << " (" << carro[i].marca << ") " << endl;
                    cout << "  Ano: " << carro[i].ano << " | Preço: R$ " << carro[i].valor << endl;
                    cout << endl;
                    achou = true;
                }
            }
            if (!achou)
            {
                cout << "Nenhum veículo com câmbio |" << busca_cambio << "| foi encontrado." << endl;
            }
            break;
        }
        case 5:
        {
            string busca_combustivel;
            float busca_potencia;
            bool achou = false;
            cout << ">> Combustíveis disponíveis:" << endl;
            cout << "Diesel | Gasolina | Flex" << endl;
            cout << "----------------------------------------------" << endl;
            cout << "*Digite o combustível desejado: ";
            cin >> busca_combustivel;
            cout << endl;
            cout << ">> Potencias disponíveis:" << endl;
            cout << "1.0 | 1.3 | 1.4 | 1.5 | 1.6 | 1.8 | 2.0 | 2.3 | 2.4" << endl;
            cout << "----------------------------------------------" << endl;
            cout << "*Digite a potencia desejada: ";
            cin >> busca_potencia;
            cout << endl;
            cout << "--- Veículos com combustível: " << busca_combustivel << " e Potencia: " << busca_potencia << " ---" << endl;
            cout << endl;

            for (int i = 0; i < qtde; i++)
            {
                if (carro[i].combustivel == busca_combustivel && carro[i].potencia == busca_potencia)
                {
                    cout << "- " << carro[i].modelo << " (" << carro[i].marca << ") " << endl;
                    cout << "  Ano: " << carro[i].ano << " | Preço: R$ " << carro[i].valor << endl;
                    cout << endl;
                    achou = true;
                }
            }
            if (!achou)
            {
                cout << "Nenhum veículo com combustível: " << busca_combustivel << " e potencia: " << busca_potencia << " foi encontrado." << endl;
                cout << endl;
            }
            break;
        }
        case 6:
        {
            float busca_faixaDePreco;
            bool achou = false;
            cout << "*Digite a faixa de preço desejada: ";
            cin >> busca_faixaDePreco;
            cout << endl;
            cout << "\n--- Veículos com preço igual ou menor que: " << busca_faixaDePreco << " ---" << endl;
            cout << endl;

            for (int i = 0; i < qtde; i++)
            {
                if (carro[i].valor <= busca_faixaDePreco)
                {
                    cout << "- " << carro[i].modelo << " (" << carro[i].marca << ") " << endl;
                    cout << "  Ano: " << carro[i].ano << " | Preço: R$ " << carro[i].valor << endl;
                    cout << endl;
                    achou = true;
                }
            }
            if (!achou)
            {
                cout << "Nenhum veículo com preço abaixou ou igual: " << busca_faixaDePreco << " foi encontrado." << endl;
            }
            break;
        }
        case 7:
        {
            listar(carro, qtde);
            break;
        }
        case 8:
        {
            if (qtde == 0)
            {
                cout << "Erro: Nao há veiculos cadastrados na memória para gerar o relatório." << endl;
                break;
            }

            // 1. Variáveis para Categorias de Tipo
            int qtd_suv = 0, qtd_sedan = 0, qtd_hatch = 0, qtd_pick_up = 0, qtd_passeio = 0, qtd_van = 0, qtd_outros_tipos = 0;

            // 2. Variáveis para Combustível
            int qtd_flex = 0, qtd_gasolina = 0, qtd_diesel = 0, qtd_outros_comb = 0;

            // 3. Variáveis para o Mais Barato motor 1.6
            float preco_mais_barato_16 = 9999999.0;
            string placa_mais_barato_16 = "Nao encontrado";

            // 4. Variáveis para o Mais Caro Flex Automático
            float preco_mais_caro_flex_auto = -1.0;
            string placa_mais_caro_flex_auto = "Nao encontrado";

            // 5. Variáveis para Ano >= 2020
            int qtd_novos = 0;
            float soma_km_novos = 0;

            // Parte 1: Varredura do Vetor
            for (int i = 0; i < qtde; i++)
            {
                // 1. Contagem por Tipo
                if (carro[i].tipo == "SUV" || carro[i].tipo == "suv")
                    qtd_suv++;
                else if (carro[i].tipo == "Sedã" || carro[i].tipo == "sedã" || carro[i].tipo == "Sedan" || carro[i].tipo == "sedan")
                    qtd_sedan++;
                else if (carro[i].tipo == "Hatch" || carro[i].tipo == "hatch")
                    qtd_hatch++;
                else if (carro[i].tipo == "Pick-up" || carro[i].tipo == "pick-up")
                    qtd_pick_up++;
                else if (carro[i].tipo == "Passeio" || carro[i].tipo == "passeio")
                    qtd_passeio++;
                else if (carro[i].tipo == "Van" || carro[i].tipo == "van")
                    qtd_van++;
                else
                    qtd_outros_tipos++;

                // 2. Contagem por Combustível
                if (carro[i].combustivel == "Flex" || carro[i].combustivel == "flex")
                    qtd_flex++;
                else if (carro[i].combustivel == "Gasolina" || carro[i].combustivel == "gasolina")
                    qtd_gasolina++;
                else if (carro[i].combustivel == "Diesel" || carro[i].combustivel == "diesel")
                    qtd_diesel++;
                else
                    qtd_outros_comb++;

                // 3. Filtro do motor 1.6 Mais Barato
                if (carro[i].potencia == 1.6f)
                {
                    if (carro[i].valor < preco_mais_barato_16)
                    {
                        preco_mais_barato_16 = carro[i].valor;
                        placa_mais_barato_16 = carro[i].placa;
                    }
                }

                // 4. Filtro do Flex Automático Mais Caro
                if ((carro[i].combustivel == "Flex" || carro[i].combustivel == "flex") &&
                    (carro[i].cambio == "Automatico" || carro[i].cambio == "automatico" || carro[i].cambio == "Automático" || carro[i].cambio == "automático"))
                {
                    if (carro[i].valor > preco_mais_caro_flex_auto)
                    {
                        preco_mais_caro_flex_auto = carro[i].valor;
                        placa_mais_caro_flex_auto = carro[i].placa;
                    }
                }

                // 5. Filtro de idade
                if (carro[i].ano >= 2020)
                {
                    qtd_novos++;
                    soma_km_novos += carro[i].km;
                }
            }

            // Parte 2: Exibição dos Dados
            cout << "\n==============================================" << endl;
            cout << "          RELATÓRIO DA BASE DE DADOS            " << endl;
            cout << "==============================================" << endl;

            cout << "\n> 1. PORCENTAGEM POR CATEGORIA DE TIPO:" << endl;
            cout << "  - SUV: " << ((float)qtd_suv / qtde) * 100 << "%" << endl;
            cout << "  - Sedã: " << ((float)qtd_sedan / qtde) * 100 << "%" << endl;
            cout << "  - Hatch: " << ((float)qtd_hatch / qtde) * 100 << "%" << endl;
            cout << "  - Pick-up: " << ((float)qtd_pick_up / qtde) * 100 << "%" << endl;
            cout << "  - Passeio: " << ((float)qtd_passeio / qtde) * 100 << "%" << endl;
            cout << "  - Van: " << ((float)qtd_van / qtde) * 100 << "%" << endl;
            cout << "  - Outros: " << ((float)qtd_outros_tipos / qtde) * 100 << "%" << endl << endl;
            cout << "==============================================" << endl;
            cout << "\n> 2. PORCENTAGEM POR TIPO DE COMBUSTIVEL:" << endl;
            cout << "  - Flex: " << ((float)qtd_flex / qtde) * 100 << "%" << endl;
            cout << "  - Gasolina: " << ((float)qtd_gasolina / qtde) * 100 << "%" << endl;
            cout << "  - Diesel: " << ((float)qtd_diesel / qtde) * 100 << "%" << endl;
            cout << "  - Outros: " << ((float)qtd_outros_comb / qtde) * 100 << "%" << endl << endl;
            cout << "==============================================" << endl;
            cout << "\n> 3. VEÍCULO 1.6 MAIS BARATO:" << endl;
            if (placa_mais_barato_16 != "Nao encontrado")
            {
                cout << "  - Placa: " << placa_mais_barato_16 << " | Valor: R$ " << preco_mais_barato_16 << endl;

                float taxa_mensal = 0.014f; // 1.4% ao mês
                int meses = 60;
                float fator = pow(1 + taxa_mensal, meses);
                float prestacao = preco_mais_barato_16 * (taxa_mensal * fator) / (fator - 1);

                cout << "  - Prestação estimada (60 meses a 1.4% a.m.): R$ " << prestacao << endl << endl;
            }
            else
            {
                cout << "  - Nenhum veículo com motor 1.6 encontrado." << endl << endl;
            }
            cout << "==============================================" << endl;
            cout << "\n> 4. VEÍCULO FLEX AUTOMÁTICO MAIS CARO:" << endl;
            if (placa_mais_caro_flex_auto != "Não encontrado.")
            {
                cout << "  - Placa: " << placa_mais_caro_flex_auto << " | Valor: R$ " << preco_mais_caro_flex_auto << endl;

                float estimativa_seguro = preco_mais_caro_flex_auto * 0.05f; // 5% do valor
                cout << "  - Valor estimado do seguro anual (5% do valor): R$ " << estimativa_seguro << endl << endl;
            }
            else
            {
                cout << "  - Nenhum veículo Flex Automático encontrado." << endl << endl;
            }
            cout << "==============================================" << endl;
            cout << "\n> 5. VEÍCULOS COM ATÉ 5 ANOS (2020 OU POSTERIOR):" << endl;
            cout << "  - Quantidade encontrada: " << qtd_novos << endl;
            if (qtd_novos > 0)
            {
                cout << "  - Média de quilometragem: " << (soma_km_novos / qtd_novos) << " km" << endl << endl;
            }
            else
            {
                cout << "  - Nenhum veículo fabricado de 2020 em diante." << endl << endl;
            }
            cout << "=============================================" << endl;

            break;
        }
        case 9:
        {
            veiculo aux;
            if (qtde < tam)
            {
                cout << "Insira o modelo do veículo: ";
                cin >> aux.modelo;
                cout << "Insira a marca do veículo: ";
                cin >> aux.marca;
                cout << "Insira o tipo do veículo: ";
                cin >> aux.tipo;
                cout << "Insira o ano do veículo: ";
                cin >> aux.ano;
                cout << "Insira a quantidade de quilometros rodados: ";
                cin >> aux.km;
                cout << "Insira a potência do motor: ";
                cin >> aux.potencia;
                cout << "Insira o tipo de combustível do veículo: ";
                cin >> aux.combustivel;
                cout << "Insira o tipo de câmbio do veículo: ";
                cin >> aux.cambio;
                cout << "Insira o tipo de direção do veículo: ";
                cin >> aux.direcao;
                cout << "Insira a cor do veículo: ";
                cin >> aux.cor;
                cout << "Insira o número de portas do veículo: ";
                cin >> aux.portas;
                cout << "Insira a placa do veículo: ";
                cin >> aux.placa;
                cout << "Insira o valor do veículo: ";
                cin >> aux.valor;

                if (insere(carro, &qtde, aux))
                {
                    cout << "Veículo adicionado com sucesso na memória!" << endl;
                    // Atualiza o arquivo físico reescrevendo com o novo veículo posicionado antes do FIM
                    atualizarArquivo(nome_arquivo, carro, qtde);
                    cout << "Dados sincronizados no arquivo!" << endl;
                }
            }
            else
            {
                cout << "Erro: Garagem cheia!" << endl;
            }
            break;
        }
        case 10:
        {
            string remocao;
            cout << "Digite a placa do veículo para remover: ";
            cin >> remocao;

            if (remove(carro, &qtde, remocao))
            {
                cout << "Veículo removido com sucesso da memória!" << endl;
                // Reescreve o arquivo físico
                atualizarArquivo(nome_arquivo, carro, qtde);
                cout << "Arquivo físico atualizado com sucesso!" << endl;
            }
            else
            {
                cout << "Erro: Veículo não encontrado." << endl;
            }
            break;
        }
        case 0:
        {
            cout << "Encerrando programa..." << endl;
            cout << "\nPrograma encerrado com sucesso." << endl;
            break;
        }
        default:
            cout << "Opção inválida!" << endl;
        }
        if (opcao != 0)
        {
            cout << "\n---------------------------------------------" << endl;
            cout << "Pressione ENTER para retornar ao menu principal.";

            cin.ignore(); // Limpa o buffer de entradas anteriores
            cin.get();    // Aguarda o usuário pressionar ENTER

            limpa_tela(); // Chamando a função para limpar a tela do terminal
        }

    } while (opcao != 0);

    return 0;
}

bool insere(veiculo bd[], int *qtde, veiculo aux)
{
    if (*qtde < tam)
    {
        bd[*qtde] = aux;
        (*qtde)++;
        return true;
    }
    return false;
}

bool remove(veiculo bd[], int *qtde, string placa_procurada)
{
    int indiceEncontrado = -1;

    for (int i = 0; i < *qtde; i++)
    {
        if (bd[i].placa == placa_procurada)
        {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1)
    {
        return false;
    }

    // Deslocamento dos elementos adjacentes para cobrir o espaço vazio
    for (int i = indiceEncontrado; i < (*qtde) - 1; i++)
    {
        bd[i] = bd[i + 1];
    }
    (*qtde)--;
    return true;
}