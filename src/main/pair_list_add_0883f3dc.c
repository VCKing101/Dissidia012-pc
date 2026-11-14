typedef struct IdPair0883F3DC { short id; short value; } IdPair0883F3DC;
typedef struct Rec0883F3DC { char pad_00[4]; short a; short b; } Rec0883F3DC;
typedef struct Owner0883F3DC { char pad_000[0x5F0]; IdPair0883F3DC pairs[16]; int count; } Owner0883F3DC;
extern Rec0883F3DC *func_088BFEEC(int id);
extern IdPair0883F3DC *func_0883F4AC(IdPair0883F3DC *begin, IdPair0883F3DC *end, IdPair0883F3DC *key);
void func_0883F3DC(Owner0883F3DC *o, int id, unsigned char alt)
{
    Rec0883F3DC *r = func_088BFEEC(id);
    if (r != 0) {
        int value = alt == 1 ? r->a : r->b;
        if (value != -1 && o->count < 16) {
            IdPair0883F3DC key;
            key.id = id;
            key.value = value;
            if (func_0883F4AC(o->pairs, &o->pairs[o->count], &key) == &o->pairs[o->count]) {
                o->pairs[o->count] = key;
                o->count++;
            }
        }
    }
}
