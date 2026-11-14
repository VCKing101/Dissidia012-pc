extern "C" {
typedef struct TrackOwner { char pad_000[0x570]; char state[4]; } TrackOwner;
typedef struct Track {
    signed char active;
    char pad_01[3];
    TrackOwner *owner;
    static bool isActive(Track *t) { return t->active != 0; }
} Track;
typedef struct GameState { char pad_00[0x70]; int ready; } GameState;
extern unsigned char func_08822D80(void *state);
extern GameState *func_0881BAA8(void);
extern void func_08821518(Track *t);
extern void func_088217F8(Track *t);
extern void func_088216E0(Track *t);
int func_08821278(Track *t)
{
    if (!Track::isActive(t)) {
        return -1;
    }
    if (func_08822D80(t->owner->state) == 1) {
        return 0;
    }
    if (func_0881BAA8()->ready == 0) {
        return -2;
    }
    func_08821518(t);
    func_088217F8(t);
    func_088216E0(t);
    return 0;
}
}
