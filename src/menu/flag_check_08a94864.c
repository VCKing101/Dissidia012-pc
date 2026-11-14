/* -1 when not ready, else whether flag 1 (kind 0) or flag 2 (kind 1) is set. */
typedef struct CheckFlagsObj {
    char pad_000[0x294];
    unsigned int field_294;
} CheckFlagsObj;

extern int func_08A951CC(void *self, CheckFlagsObj *object);

int func_08A94864(void *self, CheckFlagsObj *object, int kind)
{
    if (func_08A951CC(self, object) == 0) {
        return -1;
    }
    if (kind == 0) {
        if ((object->field_294 & 1) != 0) {
            return 1;
        }
    } else if (kind == 1 && (object->field_294 & 2) != 0) {
        return 1;
    }
    return 0;
}
