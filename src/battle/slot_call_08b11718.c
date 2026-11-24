/* Calls func_08A642B4 with the object held at +0x5C4 of the indexed battle slot; 0 when empty. */
typedef struct SlotObj { char pad_00[0x5C4]; void *held; } SlotObj;
typedef struct SlotRoot { char pad_00[0x38]; SlotObj *slot[2]; } SlotRoot;
extern SlotRoot *D_089D6A10;
extern int func_08A642B4(void *held);
int func_08B11718(int index)
{
    void *held = D_089D6A10->slot[index]->held;
    if (held != 0) {
        return func_08A642B4(held);
    }
    return 0;
}
