/* Sends event 3 to every node of the list, then clears the busy flag. */
struct Node088083A8;

extern "C" Node088083A8 *D_089D2800;
extern "C" unsigned char D_08B63280;

struct Node088083A8 {
    char pad_00[0x6C];
    Node088083A8 *next;
    virtual void handle(int event);

    static Node088083A8 *after(Node088083A8 *node)
    {
        return node == 0 ? D_089D2800 : node->next;
    }
};

extern "C" {

void func_088083A8(void)
{
    Node088083A8 *node = D_089D2800;
    while (node != 0) {
        Node088083A8 *next = Node088083A8::after(node);
        if (node != 0) {
            node->handle(3);
        }
        node = next;
    }
    D_08B63280 = 0;
}

}
