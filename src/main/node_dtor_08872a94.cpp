/* Deleting destructor of a scene node (vtable at 0x34) and its base. */
extern "C" {
extern char D_089C96E8[];
extern char D_089C96B0[];
void func_08992F04(void *p);
}

struct NodeBase08872A94 {
    char pad_00[0x34];
    void *vtable;

    static void destroy(NodeBase08872A94 *b)
    {
        if (b != 0) {
            b->vtable = D_089C96B0;
        }
    }
};

struct Node08872A94 : NodeBase08872A94 {
    static void destroy(Node08872A94 *n, unsigned int flags)
    {
        if (n != 0) {
            n->vtable = D_089C96E8;
            NodeBase08872A94::destroy(n);
            if (flags & 1) {
                func_08992F04(n);
            }
        }
    }
};

extern "C" {

void func_08872A94(Node08872A94 *n, unsigned int flags)
{
    Node08872A94::destroy(n, flags);
}

}
