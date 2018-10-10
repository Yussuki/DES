#include <iostream>

#define mask_0000111111111111111111 268435455

uint32_t rotaciona( uint32_t chave, unsigned nDesl);
uint64_t nEsimoBit( uint64_t chave, int n);
uint64_t permutaInicial(uint64_t chave);

int deslocamentos[16] = {
    1,1,1,1,1
};

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

const int PC1[56] = {
   57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
   10,  2, 59, 51, 43, 35, 27,
   19, 11,  3, 60, 52, 44, 36,
   63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
   14,  6, 61, 53, 45, 37, 29,
   21, 13,  5, 28, 20, 12,  4
};

int main()
{

    uint64_t msg, chave;

    //Recebendo Msg
    std::cin >> std::hex >> msg;

    //Recebendo Chave
    std::cin >> std::hex >> chave;

    //Permutação Inicial
    msg=permutaInicial(msg);

    std::cout << std::hex << msg << '\n';
/*
    //16 Rounds
    for (int i = 0; i < 16; i++) {
        chave = rotaciona(chave,deslocamentos[i]);
        std::cout << chave << '\t';
    }
*/
    //32 - bit swap

    //Permutação inversa

    return 0;
}

uint32_t rotaciona( uint32_t chave, unsigned nDesl){
    for(int i=0; i<nDesl;i++){
        chave = chave << 1;
        chave += (chave >> 28)&1;
        //Mascara para descartar os 4 primeiros bits
        chave = chave & mask_0000111111111111111111;
    }
    return chave;

}

uint64_t nEsimoBit( uint64_t chave, int n)
{
    return (chave >> (n)) & 1;
}

uint64_t permutaInicial(uint64_t chave)
{
    uint64_t nova = 0;
    for (int i = 63; i >= 0; i--) {
        nova += nEsimoBit(chave, (pi[i]-1)) << i;
    }
    return  nova;
}
