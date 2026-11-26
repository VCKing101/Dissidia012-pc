/* Save Replay: a 60,000-byte save object for FIELD +0x7CC, the replay object copied into its buffer from file
   offset 0xE8, the detail text's six words, and the save started. See docs/research/replay-module.md section 4. */
#include "main/replay_types.h"
extern "C" void func_089A34C0(float *dst, const float *src);
extern "C" void func_089A34E0(float *v);
/* The saved part of the replay object as C++ sees it: assigned member by member (the vectors through the
   library's copy), which is how the game copies it into the save buffer. Same layout as D012Replay. */
struct Vec3 {
    float v[3];
    Vec3() { func_089A34E0(v); }
    Vec3 &operator=(const Vec3 &o) { func_089A34C0(v, o.v); return *this; }
};
struct Camera {
    Vec3 pos_a;
    Vec3 pos_b;
    int frame;
    short unk_1C;
    short unk_1E;
    unsigned char unk_20;
    unsigned char unk_21[3];
};
struct Setup { int words[0x22E4 / 4]; };
struct ReplayData {
    unsigned int magic;
    Setup setup;
    unsigned char records[2][0x3F48];
    int record_count[2];
    Camera camera[200];
    int unk_BDA0;
    int camera_count;
    int unk_BDA8;
    int frame_total;
    unsigned char unk_BDB0;
    int player_side;
};
struct SaveObject {
    char p0[0x2BA8 + 0xE8];
    ReplayData data;                /* the buffer from file offset 0xE8 */
};
struct Field {
    char p0[0x54];
    int side;
    char p1[0x7CC - 0x58];
    SaveObject *save;
    char p2[0x800 - 0x7D0];
    int stage;
};
struct DetailArgs { int player[2]; int other[2]; int stage; int seconds; };
extern "C" Field *D_089D6A10;
extern "C" int D_089D7260;
extern "C" void func_08889060(Field *field);
extern "C" SaveObject *func_088909C8(int size);
extern "C" void func_088B0AF8(SaveObject *save);
extern "C" int func_088908F0(void);
extern "C" void func_088AF020(int manager, int heap);
extern "C" D012Replay *func_08889C30(Field *field);
extern "C" void func_08889C38(Field *field, int side, int *out);
extern "C" int func_0889716C(int stage);
extern "C" int func_088D5258(int stage);
extern "C" void func_088B0BA8(DetailArgs *args);

extern "C" void func_0888AD50(void)
{
    SaveObject *save;
    D012Replay *replay;
    Field *field;
    int heap;
    DetailArgs args;
    func_08889060(D_089D6A10);
    D_089D6A10->save = func_088909C8(60000);
    func_088B0AF8(D_089D6A10->save);
    heap = func_088908F0();
    func_088AF020(D_089D7260, heap);
    save = D_089D6A10->save;
    replay = func_08889C30(D_089D6A10);
    save->data = *(ReplayData *)replay;
    field = D_089D6A10;
    func_08889C38(field, field->side, args.player);
    func_08889C38(field, 1 - field->side, args.other);
    args.stage = func_088D5258(func_0889716C(field->stage));
    args.seconds = save->data.frame_total / 30;
    func_088B0BA8(&args);
}
