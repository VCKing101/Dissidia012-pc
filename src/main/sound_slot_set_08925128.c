typedef struct SoundRec { char pad_00[0xC]; signed char channel; signed char state; short id; } SoundRec;
typedef struct SoundSlot { int field_00; SoundRec *rec; } SoundSlot;
extern void func_089A34C0(SoundRec *rec, int name);
void func_08925128(SoundSlot *slot, int channel, int name, int state, int id)
{
    slot->rec->channel = channel;
    func_089A34C0(slot->rec, name);
    slot->rec->state = state;
    slot->rec->id = id;
}
