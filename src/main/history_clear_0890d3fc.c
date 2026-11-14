extern void func_08935828(void *dst, int value, int size);

typedef struct History {
    int field_00;
    int slots[4];               /* 0x04 */
    int pos;                    /* 0x14 */
    int field_18;
    int field_1C;
    signed char count;          /* 0x20 */
    signed char field_21;
} History;

void func_0890D3FC(History *history)
{
    func_08935828(history->slots, 0, sizeof(history->slots));
    history->field_00 = 0;
    history->pos = 0;
    history->field_18 = 0;
    history->field_1C = 0;
    history->count = 0;
    history->field_21 = 0;
}
