/* Queues a turn-order request for the scene's current actor with the
 * given value (-1 when the event is closed). */
struct OrderRequest {
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
    int value;

    OrderRequest()
        : actor(-1), kind(2), side(2), mode(2), count(0), a(0), b(0), c(0), d(2), e(2), f0(0), f1(0), f2(0), f3(0),
          target(-1), value(0)
    {
    }
};

struct OrderEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
};

extern "C" {

extern void *func_08AB328C(void *owner);
extern int func_08A7B5E0(void *scene);
extern void func_08A6BFB4(OrderEvent *self, OrderRequest *request);
extern int func_08A7C1DC(void *scene, OrderRequest *request);

int func_08A6666C(OrderEvent *self, int value)
{
    if (!self->open) {
        return -1;
    }
    OrderRequest request;
    void *scene = func_08AB328C(self->owner);
    request.actor = func_08A7B5E0(scene);
    request.kind = 3;
    request.mode = 1;
    request.count = 1;
    request.c = 3;
    request.value = value;
    func_08A6BFB4(self, &request);
    return func_08A7C1DC(scene, &request);
}

}
