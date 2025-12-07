#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;

#define INPUT_FILE_PATH "data/input.dat"
#define OUTPUT_FILE_PATH "data/output.dat"

// 1. ESTRUTURAS DE DADOS
struct HuffmanNode {
    string palavra;
    int frequencia;
    HuffmanNode *esquerda, *direita;

    HuffmanNode(string p, int f) : palavra(p), frequencia(f), esquerda(nullptr), direita(nullptr) {}
    HuffmanNode(int f, HuffmanNode* esq, HuffmanNode* dir) : palavra(""), frequencia(f), esquerda(esq), direita(dir) {}
    ~HuffmanNode() {}
};

struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequencia > b->frequencia;
    }
};

typedef map<string, string> HuffmanCodeMap;

// 2. FUNÇÕES AUXILIARES DE ÁRVORE E ALGORITMO

void liberarArvore(HuffmanNode* raiz) {
    if (raiz) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        delete raiz;
    }
}

HuffmanNode* construirArvoreHuffman(const string& texto) {
    map<string, int> frequencias;
    stringstream ss(texto);
    string palavra;

    while (ss >> palavra) {
        frequencias[palavra]++;
    }

    if (frequencias.empty()) return nullptr;

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> min_heap;
    for (const auto& par : frequencias) {
        min_heap.push(new HuffmanNode(par.first, par.second));
    }

    while (min_heap.size() > 1) {
        HuffmanNode* esq = min_heap.top(); min_heap.pop();
        HuffmanNode* dir = min_heap.top(); min_heap.pop();

        int nova_frequencia = esq->frequencia + dir->frequencia;
        HuffmanNode* novo_no = new HuffmanNode(nova_frequencia, esq, dir);
        min_heap.push(novo_no);
    }
    return min_heap.top();
}

void gerarCodigosHuffman(HuffmanNode* raiz, string codigo, HuffmanCodeMap& huffman_codes) {
    if (!raiz) return;

    if (raiz->esquerda == nullptr && raiz->direita == nullptr) {
        if (codigo.empty()) {
             huffman_codes[raiz->palavra] = "0";
        } else {
             huffman_codes[raiz->palavra] = codigo;
        }
        return;
    }

    gerarCodigosHuffman(raiz->esquerda, codigo + "0", huffman_codes);
    gerarCodigosHuffman(raiz->direita, codigo + "1", huffman_codes);
}

string comprimirTexto(const string& texto, const HuffmanCodeMap& huffman_codes) {
    string texto_comprimido = "";
    stringstream ss(texto);
    string palavra;

    while (ss >> palavra) {
        if (huffman_codes.count(palavra)) {
            texto_comprimido += huffman_codes.at(palavra);
        }
    }
    return texto_comprimido;
}

void serializarArvore(HuffmanNode* raiz, ofstream& output) {
    if (!raiz) {
        output << "# ";
        return;
    }
    // L=Folha, I=Interno, #: Nulo
    if (!raiz->esquerda && !raiz->direita) {
        output << "L " << raiz->palavra << " " << raiz->frequencia << " ";
    } else {
        output << "I " << raiz->frequencia << " ";
    }
    serializarArvore(raiz->esquerda, output);
    serializarArvore(raiz->direita, output);
}

// 3. FUNÇÃO PRINCIPAL DE PROCESSAMENTO

void processarTexto(const string& texto_original, int indice_texto, ofstream& output) {
    // Limpar espaços extras no início/fim
    string texto_limpo = texto_original;
    texto_limpo.erase(texto_limpo.find_last_not_of(" \n\r\t") + 1);
    texto_limpo.erase(0, texto_limpo.find_first_not_of(" \n\r\t"));

    if (texto_limpo.empty()) {
        output << "--- FIM TEXTO " << indice_texto << " (VAZIO) ---" << endl;
        return;
    }

    // Construção e Codificação
    HuffmanNode* raiz = construirArvoreHuffman(texto_limpo);
    if (!raiz) return;
    HuffmanCodeMap huffman_codes;
    gerarCodigosHuffman(raiz, "", huffman_codes);
    string texto_comprimido = comprimirTexto(texto_limpo, huffman_codes);
    
    // ESCRITA NO ARQUIVO DE SAÍDA (output.dat) - APENAS DADOS DE DECODIFICAÇÃO
    output << "===========================================" << endl;
    output << "PROCESSO DO TEXTO " << indice_texto << endl;
    output << "===========================================" << endl;
    output << "TEXTO ORIGINAL: " << texto_limpo << endl << endl; 

    // A) Estrutura da árvore de Huffman
    output << "A) ESTRUTURA DA ARVORE DE HUFFMAN (Serializacao Pre-ordem):" << endl;
    serializarArvore(raiz, output);
    output << "\n" << endl;

    // B) Conjunto dos códigos gerados
    output << "B) CONJUNTO DOS CODIGOS GERADOS:" << endl;
    for (const auto& par : huffman_codes) {
        output << "  '" << par.first << "' : " << par.second << endl;
    }
    output << endl;

    // C) Texto comprimido correspondente
    output << "C) TEXTO COMPRIMIDO (Binario - A ser decodificado):" << endl;
    output << texto_comprimido << endl;
    output << endl;

    output << "--- FIM PROCESSO TEXTO " << indice_texto << " ---\n" << endl;

    liberarArvore(raiz);
}

// 4. FUNÇÃO MAIN

int main() {
    // Garantir que o diretório 'data' exista
    #if defined(_WIN32)
        system("mkdir data > nul 2> nul");
    #else
        system("mkdir -p data");
    #endif

    ifstream input_leitura(INPUT_FILE_PATH);
    bool input_existe = input_leitura.is_open();
    
    ofstream output(OUTPUT_FILE_PATH);
    if (!output.is_open()) {
        cerr << "ERRO: Nao foi possivel criar ou abrir o arquivo " << OUTPUT_FILE_PATH << endl;
        return 1;
    }

    // Se o input.dat não existe, criá-lo com as frases de exemplo. Aqui é onde se colocam os textos
    if (!input_existe) {
        ofstream input_criacao(INPUT_FILE_PATH);
        if (input_criacao.is_open()) {
            input_criacao << "O computador executa instrucoes em alta velocidade e processa dados com precisao." << endl;
            input_criacao << endl;
            input_criacao << "A memoria armazena informacoes que sao acessadas rapidamente pela CPU." << endl;
            input_criacao << endl;
            input_criacao << "Os sistemas operacionais controlam os recursos e coordenam as tarefas do processador." << endl;
            input_criacao.close();
            
            // Reabrir para leitura
            input_leitura.open(INPUT_FILE_PATH); 
            
            cout << "Arquivo " << INPUT_FILE_PATH << " nao encontrado. Criado com frases de exemplo." << endl;
            // Se a reabertura falhar, não se pode continuar
            if (!input_leitura.is_open()) {
                cerr << "ERRO: Nao foi possivel reabrir o arquivo " << INPUT_FILE_PATH << " para processamento." << endl;
                output.close();
                return 1;
            }

        } else {
             cerr << "ERRO: Nao foi possivel criar o arquivo " << INPUT_FILE_PATH << endl;
             output.close();
             return 1;
        }
    }

    // Processar o conteúdo do input.dat
    string linha;
    string texto_atual;
    int indice_texto = 1;

    output << "--- INICIO PROCESSAMENTO DOS TEXTOS ---\n" << endl;

    while (getline(input_leitura, linha)) {
        if (linha.empty() || linha == "\r") {
            if (!texto_atual.empty()) {
                processarTexto(texto_atual, indice_texto++, output);
                texto_atual.clear();
            }
        } else {
            texto_atual += linha + " ";
        }
    }
    
    // Processar o último texto
    if (!texto_atual.empty()) {
        processarTexto(texto_atual, indice_texto, output);
    }
    
    input_leitura.close();
    output.close();

    cout << "Processo concluido com sucesso!" << endl;
    cout << "Os dados de decodificacao estao em " << OUTPUT_FILE_PATH << endl;

    return 0;
}