//
// Created by Luka on 14-Aug-2024.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::Node* MemoryAllocator::freeMemory = nullptr;
MemoryAllocator::Node* MemoryAllocator::usedMemory = nullptr;

void MemoryAllocator::init_memory() {
    Node *node = (Node*) HEAP_START_ADDR;
    node->size = (size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR - sizeof(Node);
    node->next = nullptr;

    freeMemory = node;
    usedMemory = nullptr;
}

void *MemoryAllocator::allocate(size_t size) {
    size = size * MEM_BLOCK_SIZE;
    Node *node, *prev;
    node = firstFit(&prev, size);
    if (!node) return node;
    node = nodeSplit(prev, node, size);
    declareUsed(node);
    return (void*) ((size_t) node + sizeof(Node));
}

int MemoryAllocator::deallocate(void *address) {
    void* nodeAddress = (uint64*) address - sizeof(Node);
    Node *node, *prev;
    node = findNode(&prev, nodeAddress);
    if (!node) return 1;

    if (prev) prev->next = node->next;
    else usedMemory = node->next;
    node->next = nullptr;

    declareFree(node);
    return 0;
}

MemoryAllocator::Node *MemoryAllocator::firstFit(MemoryAllocator::Node **previous, size_t size) {
    Node *curr = freeMemory, *prev = nullptr;
    while (curr != nullptr && curr->size < size) {
        prev = curr;
        curr = curr->next;
    }
    *previous = prev;
    return curr;
}

MemoryAllocator::Node *MemoryAllocator::nodeSplit(MemoryAllocator::Node *prev, MemoryAllocator::Node *node, size_t size) {
    Node *newNode;
    if (node->size - size >= sizeof(Node)) {
        newNode = (Node*)((uint64*) node + size + sizeof(Node));
        newNode->next = node->next;
        newNode->size = node->size - size - sizeof(Node);

        node->size = size;
        node->next = nullptr;
    }
    else newNode = node->next;

    if (prev) prev->next = newNode;
    else freeMemory = newNode;

    return node;
}

void MemoryAllocator::declareUsed(MemoryAllocator::Node *node) {
    Node *curr = usedMemory, *prev = nullptr;
    while (curr != nullptr && node > curr) {
        prev = curr;
        curr = curr->next;
    }

    node->next = curr;
    if (prev) prev->next = node;
    else usedMemory = node;
}

MemoryAllocator::Node *MemoryAllocator::findNode(MemoryAllocator::Node **previous, void *nodeAddress) {
    Node *node = usedMemory, *prev = nullptr;
    while (node && node != nodeAddress) {
        prev = node;
        node = node->next;
    }
    *previous = prev;
    return node;
}

void MemoryAllocator::compact(MemoryAllocator::Node *node, MemoryAllocator::Node *next) {
    if ((uint64*) node + node->size + sizeof(Node) == (uint64*) next) {
        node->size += next->size + sizeof(Node);
        node->next = next->next;
    }
}

void MemoryAllocator::declareFree(MemoryAllocator::Node *node) {
    Node *curr = freeMemory, *prev = nullptr;
    while (curr != nullptr && node > curr) {
        prev = curr;
        curr = curr->next;
    }

    node->next = curr;
    if (prev) prev->next = node;
    else freeMemory = node;

    if (curr) compact(node, curr);
    if (prev) compact(prev, node);
}