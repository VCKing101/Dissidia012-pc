/* Updates pad state `index` of the versus screen. */
typedef struct VersusState {
    char pad_0000[0x20C0];
    int pads[2];
} VersusState;

extern VersusState D_09AFB8A0;
extern void func_088FAEF8(int *pad);

void func_088FAE70(int index)
{
    int pads = (int)D_09AFB8A0.pads;

    func_088FAEF8((int *)(index * 4 + pads));
}
