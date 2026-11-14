struct Slot { int field_00; int field_04; int field_08; unsigned char enabled; char pad[3];
    static void fill(int *ids, int a, int b, int c) { ids[0] = a; ids[1] = b; ids[2] = c; }
    static void enable(Slot *s, unsigned char on) { s->enabled = on; } };
struct Mixer { char data_00[0x14]; char master[0x7C - 0x14]; Slot slots[3]; };
struct Config { char data_00[0x18]; float volume; };
extern "C" {
extern Config *D_089D69E0;
void func_08990B90(void);
int func_088F47E0(int id);
void func_088A1B68(Slot *slot, float volume);
void func_088A07E4(void *master, float volume);
void func_088A2324(void);
void func_0889FB9C(void);
void func_088A0098(void);

void func_088A19B0(Mixer *m)
{
    int ids[4];
    float volume = D_089D69E0->volume;
    func_08990B90();
    Slot::fill(ids, 0xC, 0xD, 0xE);
    for (int i = 0; i < 3; i++) {
        Slot *slot = &m->slots[i];
        Slot::enable(slot, func_088F47E0(ids[i]) != 0);
        func_088A1B68(slot, volume);
    }
    func_088A07E4(m->master, volume);
    func_088A2324();
    func_0889FB9C();
    func_088A0098();
}
}
