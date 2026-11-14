/* Queues the scripted command sequence for this battle event: three
 * blocks of commands, once the event's resource is ready (-1 closed, -2 no
 * resource, -3 not loaded). */
typedef struct ScriptInfo {
    unsigned char pad_00[4];
    void *resource;
} ScriptInfo;

typedef struct ScriptEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
} ScriptEvent;

extern ScriptInfo *func_08AB3264(void *owner);
extern int func_08AB344C(void *resource);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);

int func_08A67F8C(ScriptEvent *self)
{
    ScriptInfo *info;
    if (!self->open) {
        return -1;
    }
    info = func_08AB3264(self->owner);
    if (info->resource == 0) {
        return -2;
    }
    if (func_08AB344C(info->resource) == 0) {
        return -3;
    }
    func_08A65A84(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x25);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x2A);
    func_08A65B50(self->queue, 0x27);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 1);
    func_08A65C08(self->queue, 5);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    return 0;
}
