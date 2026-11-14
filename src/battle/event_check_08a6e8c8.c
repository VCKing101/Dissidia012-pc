/* Marks the event (and the caller's flag) when the event's resource is in
 * use (-1 bad arguments, -2 no resource). */
typedef struct CheckInfo {
    unsigned char pad_00[4];
    void *resource;
} CheckInfo;

typedef struct CheckEvent {
    unsigned char open;
    unsigned char marked;
    unsigned char pad_02[0x62];
    void *owner;
} CheckEvent;

extern CheckInfo *func_08AB3264(void *owner);
extern int func_088374B0(void *resource);

int func_08A6E8C8(CheckEvent *self, void *key, unsigned char *flag, void *context)
{
    CheckInfo *info;
    if (key == 0 || flag == 0 || context == 0) {
        return -1;
    }
    info = func_08AB3264(self->owner);
    if (info->resource == 0) {
        return -2;
    }
    if (func_088374B0(info->resource) == 1) {
        self->marked = 1;
        *flag = 1;
    }
    return 0;
}
