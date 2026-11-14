/* The fighter's current target: the camera's target while the fighter is
 * controlled by it, the fighter's own otherwise. */
typedef struct TargetActor {
    unsigned char pad_000[0x39C];
    void *target;
} TargetActor;

extern void *D_089D4DA0;
extern int func_08838074(TargetActor *actor);
extern void *func_0884E58C(void *camera, int index);

void *func_08A5C24C(TargetActor *actor)
{
    if (func_08838074(actor) != 0) {
        return func_0884E58C(D_089D4DA0, 0);
    }
    return actor->target;
}
