typedef struct SoundRec { char pad_00[0xC]; signed char channel; } SoundRec;
typedef struct SoundSlot { int field_00; SoundRec *rec; } SoundSlot;
extern int D_089D6AC8;
extern void func_088939AC(int mixer, int channel, int volume);
extern int func_08893A18(int mixer);
extern int func_088985F0(void *table, int channel);
extern void *func_08899300(void);
extern int func_089257BC(SoundSlot *slot, int volume, int max);
void func_08925704(SoundSlot *slot)
{
    int channel = func_088985F0(func_08899300(), slot->rec->channel);
    int volume = func_089257BC(slot, func_08893A18(D_089D6AC8), 100);
    func_088939AC(D_089D6AC8, channel, volume);
}
