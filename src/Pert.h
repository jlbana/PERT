#ifndef _H_PERT
#define _H_PERT

#include "Node.h"
#include "Stack.h"

struct _File {
    FILE* handle;
    u32   currentLine;
    Node* root;
    Node* tail;
    Stack* stack;
};

typedef struct _File File;

File*     openFile (const char*);
void      closeFile(File*);
enum bool parseFile(File*);

#define   MAX(p, q)     (p->earlyDate < q->earlyDate ? q : p)
#define   CAST(t, d)    (t) d
#define   MIN(p, q)     (p < q ? p : q)

#endif
