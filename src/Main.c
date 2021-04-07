#include <stdio.h>
#include <stdlib.h>

#include "Pert.h"

/*** A stupid-simple PERT chart implementation,
     which took approximately 4-days of coding.
     Console-only, does not include graphics.
***/

int main()
{
    File* file = openFile("PERT.txt");
    parseFile(file);
    traverseTree(file->root);
    closeFile(file);
}
