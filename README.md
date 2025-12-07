# 📚 Implementação do Código de Huffman em C++

Este repositório contém a implementação do Algoritmo de Huffman para compressão de texto sem perdas (lossless), desenvolvida em C++ para consolidar o conhecimento sobre estruturas em árvore e compressão de dados. O objetivo é desenvolver um programa que realize a compressão de pequenos trechos de texto utilizando o código de Huffman como método de codificação estatística.

O algoritmo constrói uma árvore binária ponderada, associando códigos binários menores aos símbolos (palavras) mais frequentes e códigos maiores aos menos frequentes, promovendo a redução do tamanho total da representação.

---

## 1. 📂 Estrutura do Projeto

O projeto é organizado para funcionar via linha de comando (`g++` no Linux/Windows) e interage com o diretório `data/`:

| Arquivo/Pasta | Descrição |
| :--- | :--- |
| **`main.cpp`** | O código-fonte principal que implementa todas as etapas do algoritmo de Huffman. |
| **`data/`** | Diretório que armazena os arquivos de entrada e saída. |

---

## 2. 📄 Entrada (`input.dat`) e Saída (`output.dat`)

### Arquivo de Entrada (`input.dat`)

* Deve conter pequenos textos de exemplo.
* Cada trecho de texto **deve estar separado por uma linha em branco** para ser processado individualmente pelo programa.

### Conteúdo do Arquivo de Saída (`output.dat`)

Para cada texto processado, o `output.dat` contém as informações estritamente necessárias para permitir a decodificação completa do texto original:

* **TEXTO ORIGINAL:** O trecho de texto lido e processado.
* **A) ESTRUTURA DA ÁRVORE DE HUFFMAN:** A estrutura serializada da árvore (em pré-ordem), que serve como o mapa da decodificação.
* **B) CONJUNTO DOS CÓDIGOS GERADOS:** A chave de decodificação: lista todas as palavras e seus códigos binários correspondentes.
* **C) TEXTO COMPRIMIDO (Binário):** A sequência de `0`s e `1`s resultante da compressão do texto original.
