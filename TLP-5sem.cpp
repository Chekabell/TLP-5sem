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
// E -> -E | + (E, E) | * (E, E) | S | I | C | E, E

// S -> (I, E)
// E -> -E | + (T) | * (T) | S | I | C
// T -> E,T | E

// S -> I(E)
// E -> -E | + (T) | * (T) | S | I | #R
// T -> E, T | E
// I -> aI | bI | cI ... | zI | _I | a | b | c | ... | z | _
// R -> 0D | 1D | 2D ... | 9D | 0 | 1 | 2 | ... | 9

/*
 * (I , E)
 * (I , S)
 * (I , (I, E))
 * (A , (A, M))
 * (a , (b, I))
 * (a , (b, A))
 * (a , (b, c))
 */
int main()
{
    auto translator = new Translator();
    try
    {
        translator->Start();
    }
    catch(const char* e)
    {
        std::cout << e << std::endl;
    }
    delete translator;
}