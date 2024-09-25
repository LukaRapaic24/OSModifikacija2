#ifndef LUKA_RAPAIC_OS_PROJEKAT_MEMORYALLOCATOR_HPP
#define LUKA_RAPAIC_OS_PROJEKAT_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
public:
    static void init_memory();

    static void* allocate(size_t size);
    static int deallocate(void* address);

private:
    typedef struct Node {
        size_t size;
        struct Node *next;
    } Node;
    typedef struct List {
        Node *head;
    } List;
    static Node* freeMemory;
    static Node* usedMemory;

    static Node* firstFit(Node **previous, size_t size);
    static Node* nodeSplit(Node *prev, Node *node, size_t size);
    static void declareUsed(Node *node);
    static Node* findNode(Node **previous, void *nodeAddress);
    static void compact(Node *node, Node *next);
    static void declareFree(Node *node);
};

#endif //LUKA_RAPAIC_OS_PROJEKAT_MEMORYALLOCATOR_HPP
