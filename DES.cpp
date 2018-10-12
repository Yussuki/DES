/*
    Universidade Federal de São Carlos - Campus Sorocaba
    Implementação DES
    Disciplina: Instrodução à Criptografia
    Desenvolvido por:
        Anderson Pinheiro Garrote   RA 743505
        Thiago Yussuki Uehara       RA 745???
*/
#include <iostream>

//  Mascaras
#define mask_0000111111111111111111 268435455

//  Funções
uint32_t rotaciona( uint32_t chave, unsigned nDesl);
uint64_t nEsimoBit( uint64_t v, int n);
uint64_t permutaInicial(uint64_t chave);
uint64_t bitSwap(uint64_t swap);
uint64_t escPermut1( uint64_t chave);
uint64_t escPermut2( uint64_t chave);
uint64_t expE( uint32_t msg);

//  Tabelas
int deslocamentos[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

unsigned pi[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

unsigned PC1[56] = {
   57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
   10,  2, 59, 51, 43, 35, 27,
   19, 11,  3, 60, 52, 44, 36,
   63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
   14,  6, 61, 53, 45, 37, 29,
   21, 13,  5, 28, 20, 12,  4
};

unsigned PC2[48]={
    14, 17, 11, 24, 1, 5, 3, 28,
    15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

unsigned E[48]={
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

//  Função Principal
int main()
{

    uint64_t msg, chave;
    uint32_t msgL, msgR, chaveL, chaveR;

    //Recebendo Msg
    std::cin >> std::hex >> msg;
    //Recebendo Chave
    std::cin >> std::hex >> chave;
    std::cout << "Mensagem Original: "<< std::hex << msg << '\n';
    std::cout << "Chave Original: "<< std::hex << chave << '\n';
    std::cout << "===========================" << '\n';

    //Permutação Inicial (Msg)
    msg=permutaInicial(msg);
    //Divisão em 2 blocos 32 bits
    msgL = msg >> 32;
    msgR = (msg << 64-32) >> 64-32;
    std::cout << "Permutacao Inicial: "<< std::hex << msg << '\n';
    std::cout << "===========================" << '\n';

    //Escalonamento chave 64 -> 56 bits (PC1)
    chave = escPermut1(chave);
    //Divisão em 2 blocos 28 bits
    chaveL = chave >> 28;
    chaveR = (chave << 64-28) >> 64-28;
    std::cout << "PC1 - Chave: "<< std::hex << chave << '\n';
    //std::cout << "PC1 - ChaveL: "<< std::hex << chaveL << '\t';
    //std::cout << "PC1 - ChaveR: "<< std::hex << chaveR << '\n';
    std::cout << "===========================" << '\n';



    //16 Rounds
    for (int i = 0; i < 16; i++) {
        //Inicio
        std::cout << "[ROUND " << i+1 <<"]" << '\n';

        //Chave do ROUND
        std::cout << "Chave:\n";
        //Deslocamento
        chaveL = rotaciona(chaveL,deslocamentos[i]);
        chaveR = rotaciona(chaveR,deslocamentos[i]);
        chave = ((uint64_t)chaveL << 28) + chaveR;
        std::cout << "\t" << "Desloc: " << chave << '\n';
        //Escolha Permutada 2
        chave=escPermut2(chave);
        std::cout << "\t" << "PC2: " << chave << '\n';

        //Chave do ROUND
        std::cout << "Mensagem:\n";
        std::cout << "\t" << "Orig: " << msg << '\n';

        //Expansão E
        msg=expE(msgR);
        std::cout << "\t" << "Expan: " << msg << '\n';
        
        std::cout << "\t" << "Add Key: " << msg << '\n';

        std::cout << "\t" << "S-Box: " << msg << '\n';

        std::cout << "\t" << "Permuta: " << msg << '\n';

        std::cout << "\t" << "Add Left: " << msg << '\n';

        std::cout << "\t" << "Final: " << msg << '\n';

        std::cout << "===========================" << '\n';
    }

    //32 - bit swap
    msg = bitSwap(msg);
    std::cout << "32-bit Swap: " << std::hex << msg << '\n';

    //Permutação inversa

    return 0;
}

uint32_t rotaciona( uint32_t chave, unsigned nDesl)
{
    for(int i=0; i<nDesl;i++){
        chave = chave << 1;
        chave += (chave >> 28)&1;
        //Mascara para descartar os 4 primeiros bits
        chave = chave & mask_0000111111111111111111;
    }
    return chave;

}

uint64_t nEsimoBit( uint64_t v, int n)
{
    return (v >> (n)) & 1;
}

uint64_t permutaInicial(uint64_t chave)
{
    uint64_t nova = 0;
    for (int i = 63; i >= 0; i--) {
        nova += nEsimoBit(chave, (pi[i]-1)) << i;
    }
    return nova;
}

uint64_t bitSwap(uint64_t swap)
{
    return (swap >> 32) + (swap << 32);
}


uint64_t escPermut1( uint64_t chave){
    uint64_t nova = 0;
    for (int i = 55; i >= 0; i--) {
        nova += nEsimoBit(chave, (64-PC1[i])) << 55-i;
    }
    return nova;
}

uint64_t escPermut2( uint64_t chave){
    uint64_t nova = 0;
    for (int i = 47; i >= 0; i--) {
        nova += nEsimoBit(chave, (56-PC2[i])) << 47-i;
    }
    return nova;
}

uint64_t expE( uint32_t msg){
    uint64_t nova = 0;
    for (int i = 47; i >= 0; i--) {
        nova += nEsimoBit(msg, (32-E[i])) << 47-i;
    }
    return nova;
}
