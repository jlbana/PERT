#include "Node.h"

Node* searchNode(Node* node, const char* id)
{
    /*** Search for a node with a specific identifier
         by recursively traversing the tree;
         / Node  \    / Node  \  / Node  \
         | Id1   |    | Id2   |  | Id3   |    ...
         | ...   |  / | ...   |  | ...   |  /
         | Links | *  | Links |  | Links | *
    ***/
    if (! strcmp(node->identifier, id))
        return node;

    DynamicArray* links = node->links;
    Node*         link;
    u32           i     = 0;

    while((link  = getAt(links, i++)))
    {
        link    = searchNode(link, id);
        if(link)  return link;
    }
    return NULL;
}

Node* createNode(char* id, u32 initial)
{
    /*** Creates an isolated node, with the links
         growing dynamically,  each node can grow
         and shrink as desired, providing a degree
         of flexibility.
         / Node   \    <Child node 1>
         | Id     |    <Child node 2>
         | ...    |  {      ...
         | Links  | /  <Child node n>
    ***/
    Node* node      = malloc(sizeof(*node));
    node->identifier= strdup(id);
    node->earlyDate = initial;
    node->latestDate= -1;
    node->nparent   = 0;
    node->links     = createArray(16);
    return node;
}

Node* createChild(Node* parent, char* id, u32 start)
{
    /*** Creates a child node for the parent with
         a specific identifier.
         / Parent \
         | PId    |
         | ...    |
         | Links  | )> Forward links to children.
    ***/
    Node* node   = NULL;
    if (! parent)
        return NULL;
    node         = INSERTCHILD(parent, createNode(id, start));
    SETPARENT(node, parent);
    INCPARENT(node);
    return node;
}

void linkChild(Node* parent, Node* child)
{
    /*** Link a child to a parent node, by incrementing
         nparents which serves as a reference-count,and
         inserting the child into the parent's links.
    ***/
    INSERTCHILD(parent, child);
    INCPARENT(child);
}

void destroyTree(Node* node)
{
    /*** Terminate all nodes, deletes node only when the
         reference-count drops down to zero.
    ***/
    Node*   child;
    u32     i = 0;

    while((child = getAt(node->links, i++)))
        destroyTree(child);
    destroyArray(node->links);
    node->links = NULL;

    if (DECPARENT(node) != 1)
        return;
    free(node->identifier);
    free(node);
}

void updateParent(Node* node, Node* parent)
{
    /*** Change the current node's original parent,
         which is presented with a either a strong
         or dashed-line connection.
               Dashed       The nature of the conn
         [ * ]  - -  [ * ]  -ection helps draw the
              \     /       graph properly.
               \   /
               [ * ]
               Solid
    ***/
    if (! parent)
        return;
    SETPARENT(node, parent);
    node->earlyDate += parent->earlyDate;
}

Node* identifyTail(Node* root)
{
    /*** Exploits the fact that all nodes point
         to the tail, to identify it, we verify
         its child-array size, for the tail has
         none.
         // Node \\ -> // Tail \\ -> NULL
    ***/
    if (! getSize(root->links))
    {
        root->latestDate = root->earlyDate;
        return root;
    }
    return identifyTail(getAt(root->links, 0));
}

void traverseTree(Node* node)
{
    Node*   child,
        *   neighbour;
    u32     i = 0;

    enum bool bparent;

    // If Child->Parent is Not Node
    // Dashed-edge between Child->Parent and Node
    // Else
    // Solid-edge  between Child and Node

    while((child = getAt(node->links, i++)))
    {
        bparent   = child->parent != node;
        neighbour = bparent ? child->parent : child;

        printf("(%s, %d, %d) %s (%s, %d, %d)\n",
               node->identifier, node->earlyDate, node->latestDate,
               bparent ? "(Dashed-edge)" : "(Solid-edge)",
               neighbour->identifier, neighbour->earlyDate, neighbour->latestDate);
        traverseTree(child);
    }
}
