#ifndef MAIN_AGENT2_BLOCK_HEAP_H
#define MAIN_AGENT2_BLOCK_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Block allocator managed by 0x088672F4..0x08867800. Blocks are a doubly
 * linked list of 0x14-byte headers carved from one region. */
typedef struct Agent2Block {
    int field_0000;
    int field_0004;
    int field_0008;
    struct Agent2Block *field_000C;
    struct Agent2Block *field_0010;
} Agent2Block;

typedef struct Agent2BlockHeap {
    int field_0000;
    int field_0004;
    int field_0008;
    Agent2Block *field_000C;
    int field_0010;
} Agent2BlockHeap;

#ifdef __cplusplus
}
#endif
#endif
