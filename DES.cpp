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
uint32_t Sbox(uint64_t msg);
uint32_t Perm(uint32_t msg);
uint64_t permutaInicialInversa(uint64_t msg);
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

unsigned S[8][64] = {
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,

    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,

    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
   14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};

unsigned P[32]={
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

//  Função Principal
int main()
{

    uint64_t msg, chave;
    uint32_t msgL, msgR, chaveL, chaveR;

    //INICIO
    std::cout << "\t\t [INICIO]"<<'\n';

    //Recebendo Msg
    std::cin >> std::hex >> msg;
    std::cout << "Mensagem Original:  "<< std::hex << msg << '\n';
    //Permutação Inicial (Msg)
    msg=permutaInicial(msg);
    //Divisão em 2 blocos 32 bits
    msgL = msg >> 32;
    msgR = (msg << 64-32) >> 64-32;
    std::cout << "Permutacao Inicial: "<< std::hex << msg << '\n';

    //Recebendo Chave
    std::cin >> std::hex >> chave;
    std::cout << "Chave Original:\t\t"<< std::hex << chave << '\n';
    //Escalonamento chave 64 -> 56 bits (PC1)
    chave = escPermut1(chave);
    //Divisão em 2 blocos 28 bits
    chaveL = chave >> 28;
    chaveR = (chave << 64-28) >> 64-28;
    std::cout << "PC1:\t\t\t\t"<< std::hex << chave << '\n';
    std::cout << "===========================" << '\n';

    //16 Rounds
    for (int i = 0; i < 16; i++) {
        //Inicio
        std::cout << "\t\t [ROUND " << std::dec << i+1 <<"]" << '\n';

        //Converter saída para hexa
        std::cout << std::hex;

        //Chave do ROUND
        std::cout << "> Chave:\n";
        //Deslocamento
        chaveL = rotaciona(chaveL,deslocamentos[i]);
        chaveR = rotaciona(chaveR,deslocamentos[i]);
        chave = ((uint64_t)chaveL << 28) + chaveR;
        std::cout << "\t" << "Desloc:\t\t " << chave << '\n';
        //Escolha Permutada 2
        chave=escPermut2(chave);
        std::cout << "\t" << "PC2:\t\t " << chave << '\n';

        //Chave do ROUND
        std::cout << "> Mensagem:\n";
        std::cout << "\t" << "Orig:\t\t " << msg << '\n';

        //Expansão E
        msg=expE(msgR);
        std::cout << "\t" << "Expan:\t\t " << msg << '\n';
        //Adição XOR
        msg=msg^chave;
        std::cout << "\t" << "Add Key:\t " << msg << '\n';
        //S-box
        uint32_t msgAux=Sbox(msg);
        std::cout << "\t" << "S-Box:\t\t " << msgAux << '\n';
        //Permutação
        msgAux=Perm(msgAux);
        std::cout << "\t" << "Permutacao:\t " << msgAux << '\n';
        //Adição msgL
        msgAux=msgL^msgAux;
        std::cout << "\t" << "Add Left:\t " << msgAux << '\n';
        //Atribuição msgR em msgL
        msgL = msgR;
        msgR = msgAux;
        //Recontrução da msg
        msg = msgL;
        msg = (msg << 32) + msgR;
        std::cout << "\t" << "Final:\t\t " << msg << '\n';

        std::cout << "===========================" << '\n';
    }

    //FINAL
    std::cout << "\t\t [FINAL]"<<'\n';

    //32 - bit swap
    msg = bitSwap(msg);
    std::cout << "32-bit Swap:\t\t " << std::hex << msg << '\n';

    //Permutação inversa
    msg=permutaInicialInversa(msg);
    std::cout << "Permutacao Inversa:\t " << std::hex << msg << '\n';
    std::cout << "===========================" << '\n';

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

uint32_t Sbox(uint64_t msg){


    //std::cout << "\t" << "Antes:\t\t " << msg <<  '\n';
    int auxiliar;
    uint32_t nova = 0;
    //std::cout << "\tAntes: " << msg << "\n";
    for(int i = 8; i >= 0; i--)
    {
        uint64_t linha = 0, coluna = 0;

        //pegando primeiro e ultimo bit dos 6 bits e definindo a linha da sBox
        linha = (nEsimoBit(msg, (47 - i*6)) << 1) + nEsimoBit(msg, (47 - (i*6 + 5)));

        //pegando os 4 do meio e definindo a coluna
        coluna = (nEsimoBit(msg, (47 - (i*6 + 1))) << 3) + (nEsimoBit(msg, (47 - (i*6 + 2))) << 2) + (nEsimoBit(msg, (47 - (i*6 + 3))) << 1) + nEsimoBit(msg, (47 - (i*6 + 4)));
        //std::cout << "S[" << linha*16 + coluna << "]" << '\n';
        nova += ((S[i][linha*16 + coluna]) << (28 - 4*i));
    //    std::cout << "Linha: " << linha << "\n";
    //    std::cout << "Coluna: " << coluna << "\n";
    //    std::cout << "S-Box[" << i <<  "] = " << ((S[i][linha*16 + coluna]) << (28 - 4*i)) << "\n";
    }
    return nova;
}

uint32_t Perm(uint32_t msg){
    uint32_t nova = 0;
    for (int i = 31; i >= 0; i--) {
        nova += nEsimoBit(msg, (32-P[i])) << 31-i;
    }
    return nova;
}

uint64_t permutaInicialInversa(uint64_t msg){
    uint64_t nova = 0;
    for (int i = 63; i >= 0; i--) {
        nova += nEsimoBit(msg, i) << (pi[i]-1);
    }
    return nova;
}
