/* Picks a sound id for the current slot (or 0x22 when none) and starts it when it differs from the last one played. */
typedef struct St { char pad0[0x564]; int a564; int a568; char pad1[0x58C - 0x56C]; int mode; char pad2[0x71C - 0x590]; int *list; char pad3[0x1DEC - 0x720]; unsigned short last; char pad4[0x1DF4 - 0x1DEE]; int slot; } St;
typedef struct Own { char pad[0x1EE4]; St *st; } Own;
extern const unsigned short D_08AA8840[];
extern const unsigned short D_08AA884C[];
extern int D_089D69E8;
extern void func_0888550C(int, int, int, int);
void func_08AA4174(Own *self) {
    St *st = self->st;
    int id = st->slot;
    if (id == 0) {
        id = 0x22;
    } else {
        id = st->list[id - 1];
        if (st->mode != 1) {
            id = D_08AA8840[id];
        } else {
            id = D_08AA884C[id];
        }
    }
    if (id != 0xFFFF) {
        if (st->last != id) {
            func_0888550C(D_089D69E8, st->a564, st->a568, id);
            self->st->last = id;
        }
    }
}
