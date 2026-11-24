/* Walks the list at 0x5C (next at 0x20) for a node whose info word at 0x10 equals the key with the low 18 bits cleared; returns the node or 0. */
typedef struct Node0894349C { char pad[0x18]; struct { char pad[0x10]; unsigned key; } *info; char pad2[4]; struct Node0894349C *next; } Node0894349C;
typedef struct { char pad[0x5C]; Node0894349C *head; } Owner0894349C;
Node0894349C *func_0894349C(Owner0894349C *o, unsigned key) {
    unsigned mask = 0xFFFC0000;
    unsigned k = key & mask;
    Node0894349C *n;
    for (n = o->head; n; n = n->next) {
        if (n->info->key == k) { return n; }
    }
    return 0;
}
