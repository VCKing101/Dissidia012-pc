/* State callback: an airborne fighter still moving enters state 0x10
 * (keeping its horizontal speed); otherwise starts the slide motions
 * (0x96 with the rate, 0x97) unless already sliding, then runs the slide. */
typedef struct SlideMove {
    float speed;
} SlideMove;

typedef struct SlideVec {
    float x, y, z, w;
} SlideVec;

typedef struct SlideActor {
    unsigned char pad_000[0x130];
    unsigned char motion[0x50];
    short motion_id;
    unsigned char pad_182[0x226];
    SlideMove *move;
    unsigned char pad_3ac[0x1A4];
    SlideVec velocity;
} SlideActor;

extern float D_08A59340;
extern int func_0883739C(SlideActor *actor);
extern float func_089A3BF8(SlideVec *v);
extern void func_08834C10(SlideActor *actor, int state, int arg);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern void func_0882FB90(void *motion, int id, float x, float y);
extern void func_08A59154(void *ctx, SlideActor *actor);

void func_08A58F54(void *ctx, SlideActor *actor)
{
    if (func_0883739C(actor) != 0 && !(func_089A3BF8(&actor->velocity) <= 0.0f)) {
        SlideMove *move = actor->move;
        float speed = move->speed;
        func_08834C10(actor, 0x10, 0);
        move->speed = speed;
    } else {
        if (actor->motion_id != 0x97 && actor->motion_id != 0x96) {
            float zero = 0.0f;
            void *motion = actor->motion;
            func_0882FA10(motion, 0x96, D_08A59340, zero, 0);
            func_0882FB90(motion, 0x97, zero, zero);
        }
        func_08A59154(ctx, actor);
    }
}
