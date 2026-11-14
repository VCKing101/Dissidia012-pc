extern "C" {
typedef struct Channel { char data[0x100]; } Channel;
typedef struct Mixer { int id; int field_04; int field_08; int field_0C; Channel channels[4]; float level; unsigned char busy; } Mixer;
extern void func_088E7088(Channel *c, unsigned char flag);

void func_088E6E4C(Mixer *m, int id, unsigned char flag)
{
    m->id = id;
    for (int i = 0; i < 4; i++) {
        func_088E7088(&m->channels[i], flag);
    }
    m->busy = 0;
    m->level = 0.0f;
}
}
