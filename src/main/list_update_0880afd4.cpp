/* Calls the update function for every node of the list. */
struct Node0880AFD4 {
    char pad_00[0x78];
    Node0880AFD4 *next;
};

extern "C" Node0880AFD4 *D_089D4770;
extern "C" void func_0880A9E8(Node0880AFD4 *node);

struct List0880AFD4 {
    static Node0880AFD4 *after(Node0880AFD4 *node)
    {
        return node == 0 ? D_089D4770 : node->next;
    }
};

extern "C" {

void func_0880AFD4(void)
{
    Node0880AFD4 *node = 0;
    while ((node = List0880AFD4::after(node)) != 0) {
        func_0880A9E8(node);
    }
}

}
