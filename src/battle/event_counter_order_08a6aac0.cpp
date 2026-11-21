/* Queues the counter turn-order request (actor kind 5), or a plain one when
 * that fails, records the order slot and starts the matching script; no
 * slot clears the queue (-1 closed). */
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
extern void func_08A7B468(void *scene);
extern float func_08A6677C(TurnEvent *self);
extern void func_08A66CA8(TurnEvent *self);
extern void *func_08AB327C(void *owner);
extern void func_08A67A28(TurnEvent *self, int arg);
extern void func_08A67614(TurnEvent *self);
extern void func_08A65A84(void *queue);

int func_08A6AAC0(TurnEvent *self)
{
    if (!self->open) {
        return -1;
    }
    func_08AB327C(self->owner);
    void *scene = func_08AB328C(self->owner);
    TurnRequest request;
    int result[4];
    request.actor = 5;
    request.c = 3;
    request.f0 = 1;
    request.f2 = 1;
    request.result = result;
    int order = func_08A7C1DC(scene, &request);
    if (order >= 0) {
        func_08AB3264(self->owner)->order = order;
        func_08A67A28(self, 0);
    } else {
        request.count = 0;
        request.c = 0;
        order = func_08A7C1DC(scene, &request);
        if (order >= 0) {
            func_08AB3264(self->owner)->order = order;
            func_08A67614(self);
        } else {
            func_08A65A84(self->queue);
        }
    }
    return 0;
}

}
