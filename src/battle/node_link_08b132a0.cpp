/* Appends a node to the list at D_089D8360 (head +0x194, tail +0x198), pushes it on D_089D8364's chain and resets its state. */
extern "C" {
struct LNode { LNode *next; char pad_04[8]; char chain[8]; int saved; char pad_18[0x3C0]; int d3d8; int d3dc; unsigned char d3e0; };
struct LList {
    char pad_00[0x194];
    LNode *head;
    LNode *tail;
    static void Append(LList *list, LNode *node)
    {
        if (list->tail == 0) {
            list->tail = node;
            list->head = node;
            node->next = 0;
        } else {
            list->tail->next = node;
            node->next = 0;
            list->tail = node;
        }
    }
};
struct LHub0 { char pad_00[0x1660]; void *top; };
extern LList *D_089D8360;
extern LHub0 *D_089D8364;
extern void func_08B136D4(void);
void func_08B132A0(LNode *node)
{
    LList *list = D_089D8360;
    LHub0 *hub = D_089D8364;
    if (list != 0) {
        LList::Append(list, node);
    }
    if (hub != 0) {
        *(void **)((char *)node + 0x14) = hub->top;
        hub->top = node->chain - 0 + 0;
        *(void **)((char *)hub + 0x10EC) = (void *)func_08B136D4;
        *(void **)((char *)hub + 0x10F0) = node;
    }
    node->d3dc = 1;
    node->d3e0 = 0;
    node->d3d8 = 0;
}
}
