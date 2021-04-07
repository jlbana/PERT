#ifndef _H_STACK
#define _H_STACK

#include <stdlib.h>
#include "Types.h"

/*** The Stack serves as a natural solution for
     determining the latestDate in the tree, we
     begin from the Last node, and go backwards
     until we hit the Root node.
     // Stack \\    // Container \\    ...
     | NItems  | !  |  Element    |  / ...
     | Head    | *  |  Next       | *  ...
***/

struct _Container {
    void*   data;
    struct _Container*  next;
};

struct _Stack {
    u32     nitems;
    struct _Container*  head;
};

typedef struct _Container Container;
typedef struct _Stack Stack;

Stack*  createStack   (void);
void    pushElement   (Stack*, void*);
void*   popElement    (Stack*);
void    destroyStack  (Stack*);

enum bool emptyStack  (Stack*);

#endif
