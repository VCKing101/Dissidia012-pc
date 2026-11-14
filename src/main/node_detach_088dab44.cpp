/* Clears the attached flag (bit 0 at 0x14) and hands the node back to the manager. */
struct DetachNode {
    char pad_00[0x14];
    unsigned char flags;
};

extern "C" {
extern void *D_089D7830;
extern void func_088CF314(void *manager, DetachNode *node, int mode);
}

struct Detach {
    static void release(DetachNode *node)
    {
        int zero = 0;
        func_088CF314(D_089D7830, node, zero);
    }
};

extern "C" {
void func_088DAB44(DetachNode *node)
{
    node->flags &= ~1;
    Detach::release(node);
}
}
