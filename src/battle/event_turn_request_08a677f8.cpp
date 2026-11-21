/* Queues the fighter's turn-order request (kind 5, any actor when the
 * first try fails), records the order slot, then starts the turn script
 * or the follow-up (-1 closed, -2 no fighter, -3/-4 the script refused;
 * no slot clears the queue). */
struct TurnRequest {
    int actor;
    int kind;
    int side;
    int mode;
    int count;
    int a;
    int b;
    int c;
    int d;
    int e;
    unsigned char f0, f1, f2, f3;
    int target;
    void *result;

    TurnRequest()
        : actor(-1), kind(2), side(2), mode(2), count(0), a(0), b(0), c(0), d(2), e(2), f0(0), f1(0), f2(0), f3(0),
          target(-1), result(0)
    {
    }
};

struct TurnFighter {
    unsigned char pad_000[0x180];
    short motion_id;
};

struct TurnInfo {
    unsigned char pad_00[4];
    TurnFighter *fighter;
    unsigned char pad_08[0x18];
    int order;
};

struct TurnEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
};

extern "C" {

extern TurnInfo *func_08AB3264(void *owner);
extern void *func_08AB328C(void *owner);
extern int func_08A7B5E0(void *scene);
extern int func_08A7C1DC(void *scene, TurnRequest *request);
extern int func_08A73864(TurnEvent *self, int a, int b, int c, int d, int e);
extern int func_08A67988(TurnEvent *self);
extern void func_08A65A84(void *queue);

int func_08A677F8(TurnEvent *self)
{
    if (!self->open) {
        return -1;
    }
    TurnFighter *fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    TurnRequest request;
    int result[4];
    int one;
    request.actor = 5;
    request.kind = 3;
    one = 1;
    request.count = one;
    request.c = 3;
    request.result = result;
    void *scene = func_08AB328C(self->owner);
    int order = func_08A7C1DC(scene, &request);
    if (order < 0) {
        request.actor = func_08A7B5E0(scene);
        request.count = 0;
        order = func_08A7C1DC(scene, &request);
    }
    if (order >= 0) {
        func_08AB3264(self->owner)->order = order;
        if (request.count == one || fighter->motion_id == 0x1388) {
            if (func_08A73864(self, 1, 0, 0, 0, 0) != 0) {
                return -3;
            }
        } else if (func_08A67988(self) != 0) {
            return -4;
        }
    } else {
        func_08A65A84(self->queue);
    }
    return 0;
}

}
