typedef struct SoundRec { char pad_00[0xC]; signed char channel; } SoundRec;
typedef struct SoundSlot { int field_00; SoundRec *rec; } SoundSlot;
extern int D_089D6AC8;
extern void func_088939CC(int mixer, int channel);
extern int func_088985F0(void *table, int channel);
extern void *func_08899300(void);
void func_08925778(SoundSlot *slot)
{
    int channel = func_088985F0(func_08899300(), slot->rec->channel);
    func_088939CC(D_089D6AC8, channel);
}
