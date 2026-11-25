/* The flat direction from the fighter to the event's stage object
 * (normalised, or the default when they meet); -1 no fighter, -2 no stage
 * object. */
typedef struct DirVec {
    float x, y, z, w;
} DirVec;

typedef struct DirInfo {
    unsigned char pad_00[4];
    struct DirBody *fighter;
} DirInfo;

typedef struct DirBody {
    unsigned char pad_00[0x80];
    DirVec pos;
} DirBody;

typedef struct DirEvent {
    unsigned char pad_00[0x64];
    void *owner;
} DirEvent;

extern DirInfo *func_08AB3264(void *owner);
extern DirBody *func_08A973A8(DirInfo *info);
extern void func_089A38FC(DirVec *out, DirVec *to, DirVec *from);
extern float func_089A3BF8(DirVec *v);
extern void func_089A3890(DirVec *out);
extern void func_089A3BC8(DirVec *out, DirVec *v);

int func_08A6AF6C(DirEvent *self, DirVec *out)
{
    DirBody *fighter;
    DirBody *stage;
    DirVec delta;
    float zero;
    DirVec *from;
    DirVec *to;
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -1;
    }
    stage = func_08A973A8(func_08AB3264(self->owner));
    if (stage == 0) {
        return -2;
    }
    from = &fighter->pos;
    to = &stage->pos;
    func_089A38FC(&delta, to, from);
    zero = 0.0f;
    delta.y = zero;
    if (func_089A3BF8(&delta) == zero) {
        func_089A3890(out);
    } else {
        func_089A3BC8(out, &delta);
    }
    return 0;
}
