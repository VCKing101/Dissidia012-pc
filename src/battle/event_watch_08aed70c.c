typedef struct EventOwner {
    char pad_0000[0x1108];
    void *target;
} EventOwner;

typedef struct BattleEvent {
    EventOwner *owner;
} BattleEvent;

typedef struct EventWatch {
    char pad_00[0x8];
    int pending;
} EventWatch;

extern void func_08AED9AC(EventWatch *self, BattleEvent *event);
extern int func_08837990(void *target);

void func_08AED70C(EventWatch *self, BattleEvent *event)
{
    void *target = event->owner->target;
    func_08AED9AC(self, event);
    if (target != 0 && func_08837990(target) == 1) {
        self->pending = 0;
    }
}
