#include "Stack.h"

Stack* createStack()
{
    /*** Allocate an empty stack on the  heap.
         Composed of an item-counter and head.
         // Stack       \\ !
         | NItems = 0    |
         | Head   = NULL |
    ***/
    Stack* s = malloc(sizeof(*s));
    s->head  = 0;
    s->nitems= 0;
    return s;
}

enum bool emptyStack(Stack* stack)
{
    /*** Returns true when the item-counter
         equals zero (i.e: head -> NULL)
    ***/
    return !stack->nitems;
}

void pushElement(Stack* stack, void* element)
{
    /*** Add an element to the stack, encapsulated
         in a container.
         // Stack    \\    // Container    \\ !
         |  NItems++  |  / | Element        |
         |  Head      | *  | Old Stack-head |
    ***/
    Container* c = malloc(sizeof(*c));
    c->data      = element;
    c->next      = stack->head;
    stack->head  = c;
    stack->nitems++;
}

void* popElement(Stack* stack)
{
    /*** Extract the last-inserted element in the
         stack (a LIFO structure).
         // Head \\ -> // Next \\ -> ..
            ^
            | > Extract first element and delete
            *)> the holding container.
    ***/
    if (emptyStack(stack))
        return 0;

    Container* c = stack->head;
    void* element= c->data;
    stack->head  = c->next;
    free(c);
    stack->nitems--;

    return element;
}

void destroyStack(Stack* stack)
{
    while (! emptyStack(stack))
        popElement(stack);
    free(stack);
}
