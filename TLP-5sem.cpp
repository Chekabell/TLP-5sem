#include <iostream>

#include "Translator.h"

// S -> (I, E)
// E -> -E | + (T) | * (T) | S | M
// T -> E, T | E
// M -> I | C
// I -> AI | A
// C -> #R
// R -> DR | D
// A -> a | b | c | ... | z
// D -> 0 | 1 | 2 | ... | 9

// S -> (I, E)
// E -> -E | + (T) | * (T) | S | I | #R
// T -> E,T | E
// I -> aI | bI | cI ... |zI | a | b | c | ... | z
// R -> 0D | 1D | 2D ... |9D | 0 | 1 | 2 | ... | 9

int main()
{
    auto translator = new Translator();
    try
    {
        translator->StartParse();
        std::cout << std::endl;
        translator->StartOptimize();
    }
    catch(const char* e)
    {
        std::cout << e << std::endl;
    }
    delete translator;
}