#include <iostream>
#include <string>

using namespace std;

bool plaintext[64];

bool LR[64];
bool L[32], L_pred[32];
bool R[32], R_pred[32];
bool ER[48];
bool S[32];


bool key[64];

bool CD[56];
bool K[48];


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

int IP[64] = { 58, 50, 42, 34, 26, 18, 10,  2,
               60, 52, 44, 36, 28, 20, 12,  4,
               62, 54, 46, 38, 30, 22, 14,  6,
               64, 56, 48, 40, 32, 24, 16,  8,
               57, 49, 41, 33, 25, 17,  9,  1,
               59, 51, 43, 35, 27, 19, 11,  3,
               61, 53, 45, 37, 29, 21, 13,  5,
               63, 55, 47, 39, 31, 23, 15,  7 };

int E[48] = { 32,  1,  2,  3,  4,  5,
               4,  5,  6,  7,  8,  9,
               8,  9, 10, 11, 12, 13,
              12, 13, 14, 15, 16, 17,
              16, 17, 18, 19, 20, 21,
              20, 21, 22, 23, 24, 25,
              24, 25, 26, 27, 28, 29,
              28, 29, 30, 31, 32,  1 };

int S1[4][16] = { { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
                  {  0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
                  {  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
                  { 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},};

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

void createC0D0(bool CD[]) {
    for (int i = 0; i < 56; i++)
        CD[i] = key[PC1[i] - 1];
}

void shiftCD(bool CD[], int i) {
    bool b1, b2;
    if (i == 1 || i == 2 || i == 9 || i == 16) {
        b1 = CD[0];
        for (int i = 0; i < 27; i++)
            CD[i] = CD[i + 1];
        CD[27] = b1;

        b1 = CD[28];
        for (int i = 28; i < 55; i++)
            CD[i] = CD[i + 1];
        CD[55] = b1;
    }
    else {
        b1 = CD[0];
        b2 = CD[1];
        for (int i = 0; i < 26; i++)
            CD[i] = CD[i + 2];
        CD[26] = b1; 
        CD[27] = b2;

        b1 = CD[28];
        b2 = CD[29];
        for (int i = 28; i < 54; i++)
            CD[i] = CD[i + 2];
        CD[54] = b1;
        CD[55] = b2;
    }
}

void createK_i(bool K[], bool CD[]) {
    for (int i = 0; i < 48; i++)
        K[i] = CD[PC2[i] - 1];
}

void copyFromTo(bool from[], bool to[], int length) {
    for (int i = 0; i < length; i++)
        to[i] = from[i];
}

void DESencode(bool plaintext[], bool key[], bool cryptotext[]) {
    createC0D0(CD);

    for (int i = 0; i < 64; i++)
        LR[i] = plaintext[IP[i] - 1];

    for (int i = 0; i < 32; i++)
        L[i] = LR[i];
    for (int i = 32; i < 64; i++)
        R[i - 32] = LR[i];
    //L_i-1 R_i-1
    copyFromTo(R, R_pred, 32);
    copyFromTo(L, L_pred, 32);

    //L_i
    copyFromTo(R_pred, L, 32);

    shiftCD(CD, 1);

    createK_i(K, CD);

    for (int i = 0; i < 48; i++)
        ER[i] = R_pred[E[i] - 1] ^ K[i];


}

int main()
{
    string keyB = "011000010001011110111010100001100110010100100111";
    string keyH = "133457799bbcdff1";
    string plainH = "0123456789abcdef";

    convertStringHexToBinary(keyH, key);
    convertStringHexToBinary(plainH, plaintext);

    DESencode(plaintext, key, cryptotext);

    cout << keyB << endl;
    for (int i = 0; i < 48; i++)
        cout << ER[i];
}