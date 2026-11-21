/* Fills the script queue with one of four random patterns (-1 closed, -2
 * when the pattern could not be queued); the patterns are member functions
 * called through pointer-to-member entries. */
typedef struct FillEntry {
    short delta;
    short pad;
    int (*fn)(void *self);
} FillEntry;

typedef struct FillStep {
    short delta;
    short index;
    int target;
} FillStep;

typedef struct FillTable {
    FillStep steps[4];
} FillTable;

typedef struct FillEvent {
    unsigned char open;
} FillEvent;

extern const FillTable D_08A7ACF8;
extern void *D_08AA5828;

extern void *func_08935524(void *dst, const void *src, unsigned int size);
extern int func_08AA5748(void *random);

int func_08A67CBC(FillEvent *self)
{
    FillStep steps[4];
    int count;
    FillStep *step;
    char *object;
    int (*fn)(void *self);
    if (!self->open) {
        return -1;
    }
    func_08935524(steps, &D_08A7ACF8, sizeof(steps));
    count = 4;
    step = &steps[func_08AA5748(D_08AA5828) % count];
    object = (char *)self + step->delta;
    if (step->index == 0) {
        fn = (int (*)(void *))step->target;
    } else {
        FillEntry *entry = *(FillEntry **)(object + step->target) + step->index;
        object += entry->delta;
        fn = entry->fn;
    }
    if (fn(object) != 0) {
        return -2;
    }
    return 0;
}
