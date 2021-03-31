#include <iostream>
#include <string>

using namespace std;

bool plaintext[64];
bool key[64];
bool cryptotext[64];

int PC1[56] = { 57, 49, 41, 33, 25, 17,  9,
                 1, 58, 50, 42, 34, 26, 18,
                10,  2, 59, 51, 43, 35, 27,
                19, 11,  3, 60, 52, 44, 36,
                63, 55, 47, 39, 31, 23, 15,
                 7, 62, 54, 46, 38, 30, 22,
                14,  6, 61, 53, 45, 37, 29,
                21, 13,  5, 28, 20, 12,  4 };

int PC2[48] = { 14, 17, 11, 24,  1,  5,
                 3, 28, 15,  6, 21, 10,
                23, 19, 12,  4, 26,  8,
                16,  7, 27, 20, 13,  2,
                41, 52, 31, 37, 47, 55,
                30, 40, 51, 45, 33, 48,
                44, 49, 39, 56, 34, 53,
                46, 42, 50, 36, 29, 32 };

bool CD[56];
bool k[48];

void convertNumberToBinary(int number, int index, bool code[]) {
    code[index * 4 + 3] = number % 2;
    number >>= 1;
    code[index * 4 + 2] = number % 2;
    number >>= 1;
    code[index * 4 + 1] = number % 2;
    number >>= 1;
    code[index * 4] = number % 2;
}

void convertStringHexToBinary(string s, bool code[]) {
    if (s.size() != 16)
        return;
    for (int i = 0; i < s.size(); i++)
        if (!(s[i] >= '0' && s[i] <= '9') && !(s[i] >= 'a' && s[i] <= 'f'))
            return;
    for (int i = 0; i < s.size(); i++)
        if (s[i] >= '0' && s[i] <= '9')
            convertNumberToBinary(s[i] - '0', i, code);
        else
            convertNumberToBinary(s[i] - 'a' + 10, i, code);
}

void DESencode(bool plaintext[], bool key[], bool cryptotext[]) {
    //C0 D0
    for (int i = 0; i < 56; i++)
        CD[i] = key[PC1[i] - 1];

    /*for (int i = 0; i < 56; i++)
        cout << CD[i];
    cout << endl;*/

    //C1 D1
    bool b1;

    b1 = CD[0];
    for (int i = 0; i < 27; i++)
        CD[i] = CD[i + 1];
    CD[27] = b1;
    b1 = CD[28];
    for (int i = 28; i < 55; i++)
        CD[i] = CD[i + 1];
    CD[55] = b1;

    /*for (int i = 0; i < 56; i++)
        cout << CD[i];
    cout << endl;*/

    //K1
    for (int i = 0; i < 48; i++)
        k[i] = CD[PC2[i] - 1];
}

int main()
{
    string keyB = "00011011OOOOGO1011101111111111000111OOOOO1110010";
    string keyH = "133457799bbcdff1";

    convertStringHexToBinary(keyH, key);

    bool c[20];

    DESencode(plaintext, key, c);

    cout << keyB << endl;
    for (int i = 0; i < 48; i++)
        cout << k[i];
}