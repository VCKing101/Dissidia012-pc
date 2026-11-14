typedef struct SoundRec {
    char pad_00[0xC]; signed char channel; char pad_0D[7];
    float step; float pos; int time; int frames; char pad_24[4]; unsigned char active;
} SoundRec;
typedef struct SoundSlot { int field_00; SoundRec *rec; } SoundSlot;
extern void *func_08899300(void);
extern void func_088983C8(void *table, int channel, int flag);
extern void func_08925AF8(SoundSlot *slot);
void func_08925A58(SoundSlot *slot, int frames)
{
    void *table = func_08899300();
    int channel = slot->rec->channel;
    int on = 1;
    func_088983C8(table, channel, 1);
    slot->rec->active = on;
    slot->rec->time = 0;
    slot->rec->frames = frames;
    slot->rec->pos = 1.0f;
    slot->rec->step = -1.0f / slot->rec->frames;
    func_08925AF8(slot);
}
