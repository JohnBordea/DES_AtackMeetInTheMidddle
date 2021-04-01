#include <iostream>
#include <string>

using namespace std;

bool plaintext[64];

bool LR[64];
bool L[32], L_pred[32];
bool R[32], R_pred[32];
bool ER[48];
bool S_box[32];


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

int IPinvers[64] = { 40,  8, 48, 16, 56, 24, 64, 32,
                     39,  7, 47, 15, 55, 23, 63, 31,
                     38,  6, 46, 14, 54, 22, 62, 30,
                     37,  5, 45, 13, 53, 21, 61, 29,
                     36,  4, 44, 12, 52, 20, 60, 28,
                     35,  3, 43, 11, 51, 19, 59, 27,
                     34,  2, 42, 10, 50, 18, 58, 26,
                     33,  1, 41,  9, 49, 17, 57, 25 };

int E[48] = { 32,  1,  2,  3,  4,  5,
               4,  5,  6,  7,  8,  9,
               8,  9, 10, 11, 12, 13,
              12, 13, 14, 15, 16, 17,
              16, 17, 18, 19, 20, 21,
              20, 21, 22, 23, 24, 25,
              24, 25, 26, 27, 28, 29,
              28, 29, 30, 31, 32,  1 };

int S[8][4][16] = { { { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
                      {  0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
                      {  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
                      { 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13} },

                    { { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
                      {  3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
                      {  0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
                      { 13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9} },

                    { { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
                      { 13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
                      { 13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
                      {  1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12} },


                    { {  7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
                      { 13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
                      { 10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
                      {  3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14} },

                    { {  2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
                      { 14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
                      {  4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
                      { 11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3} },

                    { { 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
                      { 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
                      {  9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
                      {  4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13} },

                    { {  4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
                      { 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
                      {  1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
                      {  6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12} },

                    { { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
                      {  1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
                      {  7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
                      {  2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11} } };

int P[32] = { 16,  7, 20, 21,
              29, 12, 28, 17,
               1, 15, 23, 26,
               5, 18, 31, 10,
               2,  8, 24, 14,
              32, 27,  3,  9,
              19, 13, 30,  6,
              22, 11,  4, 25 };

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

void shiftLeftCD(bool CD[], int i) {
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

void shiftRightCD(bool CD[], int i) {
    bool b1, b2;
    if (i == 1 || i == 2 || i == 9 || i == 16) {
        b1 = CD[27];
        for (int i = 27; i > 0; i--)
            CD[i] = CD[i - 1];
        CD[0] = b1;

        b1 = CD[55];
        for (int i = 55; i > 28; i--)
            CD[i] = CD[i - 1];
        CD[28] = b1;
    }
    else {
        b1 = CD[26];
        b2 = CD[27];
        for (int i = 27; i > 1; i--)
            CD[i] = CD[i - 2];
        CD[0] = b1;
        CD[1] = b1;

        b1 = CD[54];
        b2 = CD[55];
        for (int i = 55; i > 29; i--)
            CD[i] = CD[i - 2];
        CD[28] = b1;
        CD[29] = b2;
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

void encodeDES(bool plaintext[], bool key[], bool cryptotext[]) {
    createC0D0(CD);

    for (int i = 0; i < 64; i++)
        LR[i] = plaintext[IP[i] - 1];

    for (int i = 0; i < 32; i++)
        L[i] = LR[i];
    for (int i = 32; i < 64; i++)
        R[i - 32] = LR[i];

    for (int i = 1; i <= 16; i++) {
        //L_i-1 R_i-1
        copyFromTo(R, R_pred, 32);
        copyFromTo(L, L_pred, 32);

        //L_i
        copyFromTo(R_pred, L, 32);

        shiftLeftCD(CD, i);

        createK_i(K, CD);

        for (int j = 0; j < 48; j++)
            ER[j] = R_pred[E[j] - 1] ^ K[j];

        for (int j = 0; j < 8; j++)
            convertNumberToBinary(S[j][((ER[j * 6] << 1) + ER[j * 6 + 5])][(((((ER[j * 6 + 1] << 1) + ER[j * 6 + 2]) << 1) + ER[j * 6 + 3]) << 1) + ER[j * 6 + 4]], j, S_box);

        for (int j = 0; j < 32; j++)
            R[j] = S_box[P[j] - 1] ^ L_pred[j];
    }
    
    for (int i = 0; i < 32; i++)
        LR[i] = R[i];
    for (int i = 32; i < 64; i++)
        LR[i] = L[i - 32];

    for (int i = 0; i < 64; i++)
        cryptotext[i] = LR[IPinvers[i] - 1];
}

void decodeDES(bool cryptotext[], bool key[], bool plaintext[]) {
    for (int i = 0; i < 64; i++)
        LR[i] = cryptotext[IP[i] - 1];

    for (int i = 0; i < 32; i++)
        L[i] = LR[i];
    for (int i = 32; i < 64; i++)
        R[i - 32] = LR[i];

    for (int i = 16; i >= 1; i--) {
        //L_i-1 R_i-1
        copyFromTo(R, R_pred, 32);
        copyFromTo(L, L_pred, 32);

        //L_i
        copyFromTo(R_pred, L, 32);

        createC0D0(CD);
        for (int j = 1; j <= i; j++)
            shiftLeftCD(CD, j);

        createK_i(K, CD);

        for (int i = 0; i < 48; i++)
            ER[i] = R_pred[E[i] - 1] ^ K[i];

        for (int i = 0; i < 8; i++)
            convertNumberToBinary(S[i][((ER[i * 6] << 1) + ER[i * 6 + 5])][(((((ER[i * 6 + 1] << 1) + ER[i * 6 + 2]) << 1) + ER[i * 6 + 3]) << 1) + ER[i * 6 + 4]], i, S_box);

        for (int i = 0; i < 32; i++)
            R[i] = S_box[P[i] - 1] ^ L_pred[i];
    }

    for (int i = 0; i < 32; i++)
        LR[i] = R[i];
    for (int i = 32; i < 64; i++)
        LR[i] = L[i - 32];

    for (int i = 0; i < 64; i++)
        plaintext[i] = LR[IPinvers[i] - 1];
}

string bitstringToString(bool bit[]) {
    string text;
    int nr = 0;
    for (int i = 0; i < 16; i++) {
        nr = 0;
        for (int j = 0; j < 4; j++) {
            nr <<= 1;
            nr += bit[i * 4 + j];
        }
        text += "-";
        if (nr >= 0 && nr <= 9)
            text[i] = '0' + nr;
        else
            text[i] = 'A' + nr - 10;
    }

    return text;
}

int main()
{
    string keyB = "85E813540FOAB405";
    string keyH = "133457799bbcdff1";
    string plainH = "0123456789abcdef";

    convertStringHexToBinary(keyH, key);
    convertStringHexToBinary(plainH, plaintext);

    encodeDES(plaintext, key, cryptotext);

    cout << keyB << endl << bitstringToString(cryptotext) << endl;

    decodeDES(cryptotext, key, plaintext);

    cout << plainH << endl << bitstringToString(plaintext);
    
}