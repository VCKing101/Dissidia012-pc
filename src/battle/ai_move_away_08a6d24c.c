/* AI move order away from the event's target point: steps a fixed distance
 * from the fighter's position along the direction away from the point and sends
 * the fighter there (-1 no point, -2 no fighter; 0 when the point is the
 * origin or no direction remains). */
typedef struct MoveVec4 {
    float x, y, z, w;
} MoveVec4;

typedef struct MoveFighter {
    unsigned char pad_000[0x100];
    MoveVec4 pos;
} MoveFighter;

typedef struct MoveInfo {
    unsigned char pad_00[4];
    MoveFighter *fighter;
} MoveInfo;

typedef struct MoveAi {
    unsigned char pad_00[0x64];
    void *owner;
    unsigned char pad_68[4];
    void *orders;
} MoveAi;

extern MoveVec4 D_09AF84E0;
extern float D_08A7ABF8;

extern int func_08A6AF6C(MoveAi *self, MoveVec4 *point);
extern int func_089A3B70(MoveVec4 *v);
extern MoveVec4 *func_089A3B30(MoveVec4 *out, const MoveVec4 *a, const MoveVec4 *b);
extern float func_089A3BF8(MoveVec4 *v);
extern MoveVec4 *func_089A3BC8(MoveVec4 *out, const MoveVec4 *in);
extern MoveInfo *func_08AB3264(void *owner);
extern MoveVec4 *func_089A3A64(MoveVec4 *out, const MoveVec4 *in, float scale);
extern MoveVec4 *func_089A38C4(MoveVec4 *out, const MoveVec4 *a, const MoveVec4 *b);
extern void func_08A877F8(void *orders, MoveVec4 *point, int kind, int flags, float a, float b);

int func_08A6D24C(MoveAi *self)
{
    MoveVec4 point;
    MoveVec4 dir;
    MoveFighter *fighter;
    if (func_08A6AF6C(self, &point) != 0) {
        return -1;
    }
    if (func_089A3B70(&point) == 1) {
        return 0;
    }
    func_089A3B30(&dir, &point, &D_09AF84E0);
    if (!(func_089A3BF8(&dir) == 0.0f)) {
        MoveVec4 *pos;
        func_089A3BC8(&dir, &dir);
        fighter = func_08AB3264(self->owner)->fighter;
        if (fighter == 0) {
            return -2;
        }
        pos = &fighter->pos;
        func_089A3A64(&dir, &dir, D_08A7ABF8);
        func_089A38C4(&dir, &dir, pos);
        func_08A877F8(self->orders, &dir, 7, 0, 0.0f, 0.0f);
    }
    return 0;
}
