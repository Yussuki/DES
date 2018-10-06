#include <iostream>

void rotaciona( unsigned long *chave, unsigned nDesl);

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

  unsigned long msg, chave;

  //Recebendo Msg
  std::cin >> msg;

   //Recebendo Chave
  std::cin >> chave;
  std::cout << chave;
  //Permutação Inicial

  //16 Rounds

  //32 - bit swap

  //Permutação inversa

  return 0;
}

void rotaciona( unsigned long *chave, unsigned nDesl){
    for(int i=0; i<nDesl;i++){
        *chave = *chave << 1;
        *chave += *chave >> 28;
        //Mascara para descartar os 4 primeiros bits
        *chave = *chave & 268435455;
    }

}

unsigned long long nEsimoBit(unsigned long long chave, int n)
{
    return (chave >> (n-1)) & 1;
}
