#include "battle/agent27_types.h"

extern void func_088D9B84(NodeList *list, int a);
extern void func_088D9AF4(NodeList *list, int b);
extern void func_088DA0A8(NodeList *list, int index, float *matrix, int *extra);
extern void func_0887B778(ListNode *node, float *matrix);
extern void func_0887B828(ListNode *node, int *extra);

int func_08B2C120(ModelSlot *slot, int a, int b)
{
    float matrix[16];
    int extra;

    if (b == 0) {
        return -1;
    }
    if (slot->list != 0) {
        func_088D9B84(slot->list, a);
        func_088D9AF4(slot->list, b);
        if (slot->node != 0) {
            func_088DA0A8(slot->list, 0, matrix, &extra);
            func_0887B778(slot->node, matrix);
            func_0887B828(slot->node, &extra);
        }
    }
    return 0;
}
