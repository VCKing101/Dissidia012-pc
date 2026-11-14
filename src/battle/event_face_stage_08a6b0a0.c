/* Turns an idle fighter to face the event's stage object (-1 no fighter,
 * -2 no direction; 0 when the fighter cannot turn or no turn is needed). */
typedef struct FaceVec {
    float x, y, z, w;
} FaceVec;

typedef struct FaceAction {
    int id;
} FaceAction;

typedef struct FaceFighter {
    unsigned char pad_000[0x3A4];
    FaceAction *action;
} FaceFighter;

typedef struct FaceInfo {
    unsigned char pad_00[4];
    FaceFighter *fighter;
} FaceInfo;

typedef struct FaceEvent {
    unsigned char pad_00[0x64];
    void *owner;
} FaceEvent;

extern FaceInfo *func_08AB3264(void *owner);
extern int func_08A83EB8(FaceFighter *fighter);
extern int func_08A6AF6C(FaceEvent *self, FaceVec *out);
extern int func_089A3B70(FaceVec *v);
extern void func_08835C7C(FaceFighter *fighter, FaceVec *dir);

int func_08A6B0A0(FaceEvent *self)
{
    FaceFighter *fighter;
    FaceVec dir;
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -1;
    }
    if (func_08A83EB8(fighter) == 0) {
        return 0;
    }
    if (func_08A6AF6C(self, &dir) != 0) {
        return -2;
    }
    if (func_089A3B70(&dir) == 0) {
        if (fighter->action->id == 0) {
            FaceVec turn = dir;
            func_08835C7C(fighter, &turn);
        }
    }
    return 0;
}
