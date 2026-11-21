/* Checks a fighter's slot (row < 7, column < 5) against the scene: 1 when
 * only the second test holds, or both hold and the scene allows it; 0 when
 * only the first holds or the scene refuses; -1 otherwise. */
typedef struct SlotInfo {
    unsigned char pad_00[0x30];
    void *busy;
} SlotInfo;

typedef struct SlotEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
} SlotEvent;

extern SlotInfo *func_08AB3264(void *owner);
extern int func_08A6DD08(SlotEvent *self, unsigned int column, unsigned int row);
extern int func_08A6DFD4(SlotEvent *self, unsigned int column, unsigned int row);
extern void *func_08AB327C(void *owner);
extern int func_08AAEF94(void *scene);

int func_08A668A0(SlotEvent *self, unsigned int row, unsigned int column)
{
    int first;
    int second;
    if (!self->open) {
        return -1;
    }
    if (row >= 7 || column >= 5) {
        return -1;
    }
    if (func_08AB3264(self->owner)->busy != 0) {
        return -1;
    }
    first = func_08A6DD08(self, column, row);
    second = func_08A6DFD4(self, column, row);
    if (first == 0) {
        if (second == 0) {
            return -1;
        }
        return 1;
    }
    if (second == 0) {
        return 0;
    }
    if (func_08AAEF94(func_08AB327C(self->owner)) == 0) {
        return 0;
    }
    return 1;
}
