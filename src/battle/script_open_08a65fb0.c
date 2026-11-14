/* Opens a battle script: resets it, opens its two timers and its command
 * list, and keeps the owner, the queue and the two handlers (-1 missing
 * argument, -2/-3 timer failed, -4 list failed). */
typedef struct ScriptTimer {
    unsigned char data[0x10];
} ScriptTimer;

typedef struct BattleScript {
    unsigned char open;
    unsigned char pad_01[3];
    ScriptTimer timer_a;
    ScriptTimer timer_b;
    unsigned char list[0x2C];
    int step;
    unsigned char pad_54[0x10];
    void *owner;
    void *queue;
    void *on_start;
    void *on_end;
} BattleScript;

extern void func_08A65F70(BattleScript *self);
extern int func_08A7E7FC(ScriptTimer *timer);
extern int func_08A9431C(void *list);

int func_08A65FB0(BattleScript *self, void *owner, void *queue, void *on_start, void *on_end)
{
    func_08A65F70(self);
    if (owner == 0 || queue == 0 || on_start == 0 || on_end == 0) {
        return -1;
    }
    if (func_08A7E7FC(&self->timer_a) != 0) {
        return -2;
    }
    if (func_08A7E7FC(&self->timer_b) != 0) {
        return -3;
    }
    if (func_08A9431C(self->list) != 0) {
        return -4;
    }
    self->owner = owner;
    self->queue = queue;
    self->on_start = on_start;
    self->on_end = on_end;
    self->step = 0;
    self->open = 1;
    return 0;
}
