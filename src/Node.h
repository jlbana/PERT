#ifndef _H_NODE
#define _H_NODE

#include <stdio.h>
#include "DynamicArray.h"

/*** A node can easily point to other nodes
     using the dynamic-array implementation.
     By searching for a specific node using
     a string Identifier, insertion is made
     simpler with a hierarchical model.
***/

struct _Node {
    char*   identifier;
    u32     earlyDate;
    u32     latestDate;
    u32     nparent;
    struct _Node* parent;
    DynamicArray* links;
};

typedef struct _Node Node;

Node* createNode  (char*, u32);
Node* createChild (Node*, char*, u32);
Node* searchNode  (Node*, const char*);
void  linkChild   (Node*, Node*);
void  destroyTree (Node*);
void  updateParent(Node*, Node*);
void  traverseTree(Node*);

#define INCPARENT(n)        n->nparent++
#define DECPARENT(n)        n->nparent--
#define INSERTCHILD(p, n)   appendArray(p->links, n)
#define SETPARENT(n, p)     n->parent = p

#endif
