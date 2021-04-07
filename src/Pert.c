#include "Pert.h"

File* openFile(const char* filename)
{
    File* file   = malloc(sizeof(*file));
    file->handle = fopen(filename,  "r");
    if (! file->handle)
    {
        perror("fopen");
        exit(0);
    }
    file->root   = createNode("root", 0);
    file->currentLine   = 0;
    file->tail   = NULL;
    file->stack  = createStack();

    file->root->nparent     = 1;
    file->root->latestDate  = 0;
    return file;
}

void closeFile(File* file)
{
    /*** Destroy a file and the equivalent tree, before
         freeing the holding memory area.          ***/
    destroyStack(file->stack);
    destroyTree(file->root);
    fclose(file->handle);
    free(file);
}

void determineLatest(Stack* stack)
{
    /*** Traverse the tree backwards, and calculate the latestDate
         properly based on the information available:
         | Noden     | -> | Duration | -> | Parent #1 | ->
         | Parent #2 | -> | Noden    | -> ...
    ***/
    u32     duration, i;
    Node*   node,
        *   pnode,
        *   mnode;

    while (! emptyStack(stack))
    {
        node     = CAST(Node*,  popElement(stack));
        duration = CAST(u32,    popElement(stack));
        mnode    = node->parent;
        i        = node->nparent;
        mnode->latestDate = MIN(mnode->latestDate, node->latestDate - duration);

        while (i--)
        {
            pnode = popElement(stack);
            if (pnode == mnode)
                continue;
            pnode->latestDate= MIN(pnode->latestDate, mnode->latestDate);
        }
    }
}

enum bool parseFile(File* file)
{
    /*** Parse a PERT file with the following line-syntax:
                    <Node ID,Duration,Parents>
         The tree is expanded accordingly and links nodes
         together, the result is a proper dissection of a
         PERT table into a rooted-tree.

         | A,1,_   | A is a child of the root node.
         | B,4,A   | A points to B as one of its children.
         | C,8,B   | A->B->C becomes available.
         | D,1,A C | D is pointed to by both A and C.
    ***/
    char    id[64], parents[64],
        *   parent;
    u32     duration;
    Node*   node,
        *   pnode,
        *   mnode;

    if   (! file->handle)
        return false;

    while(! feof(file->handle))
    {
        fscanf(file->handle,
               "%64[^,],%d,%64[^\n]\n",
               id, &duration, parents);
        node     = NULL;
        mnode    = file->root;
        parent   = strtok(parents, " ");
        do {
            if (! strcmp(parent, "_"))
                parent  = "root";

            pnode = searchNode(file->root, parent);
            pushElement(file->stack, pnode);

            if (! node)
            {
                node = createChild(pnode, id, duration);
            }
            else
                linkChild(pnode, node);

            mnode = MAX(pnode, mnode);
        } while((parent = strtok(NULL, " ")));

        pushElement(file->stack, CAST(void*, duration));
        pushElement(file->stack, CAST(void*, node));
        updateParent(node, mnode);
    }

    file->tail              = node;
    node->latestDate        = node->earlyDate;
    determineLatest(file->stack);
    return true;
}
