#include <iostream>
#include "Huffman.h"

int main()
{
    int line_size = 400; //max quantity of symbols in input line
    char* str = new char[line_size]; //array for input_line
    int size; //real size of input line
    size = 0;
    char symbol;

    cout << "Enter a line:" << endl;
    do //reading the input line
    {
        cin >> noskipws >> symbol; //to read with spaces
        str[size] = symbol;
        size++;
    } while ((symbol != '.') && ((size + 1) < line_size));

    str[size] = '\n';
    double coefficient = 1.0 * sizeof(char) * size * 8; //volume of the line

    Huffman ToCode(str);
    int size_coded; //size of the coded line
    bool* code = ToCode.Code(size); //coded line
    char* decode = ToCode.Decode(size_coded); //decoded line

    cout << "Coded line: ";//output of the coded line
    for (size_t i = 0; i < size; i++)
    {
        cout << code[i];
    }
    cout << endl << endl;

    coefficient = coefficient / (1 * size); //calculate compression ratio
    cout << "Coefficient of compression: " << coefficient << endl << endl;

    cout << "Decoded line: ";
    for (size_t i = 0; i < size_coded; i++)
    {
        cout << decode[i];
    }
    cout << endl;
}