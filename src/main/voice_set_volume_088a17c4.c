typedef struct Slot { int field_00; int field_04; int field_08; int field_0C; } Slot;
typedef struct Channel { char data_00[0x7C]; Slot slots[3]; } Channel;
extern float func_088A1C38(Slot *slot);
extern void func_08990AC0(int voice, int volume, float pitch);

void func_088A17C4(Channel *ch, int voice, int index, float pitch, float level)
{
    float volume = func_088A1C38(&ch->slots[index]);
    func_08990AC0(voice, (int)(level * volume / 100.0f * 4096.0f), pitch);
}
