/* Loads the pair mark's owner and mark models on first use, registers the
 * mark with its source and attaches it. */
typedef struct PairMarkData {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    void *mark;
    unsigned char pad_0c[8];
    void *source;
    void *mark_source;
    int mark_name;
} PairMarkData;

extern void *func_088DAA40(void *source, int name, int flags);
extern void func_088DA060(void *model);
extern void func_088DAB7C(void *source, void *model);
extern int func_08A01F94(PairMarkData *pair);

int func_08A020C8(PairMarkData *pair)
{
    if (!pair->open) {
        return -1;
    }
    if (pair->model == 0) {
        pair->model = func_088DAA40(pair->source, 0x15, 0);
        if (pair->model == 0) {
            return -2;
        }
    }
    if (pair->mark == 0) {
        pair->mark = func_088DAA40(pair->mark_source, pair->mark_name, 0);
        if (pair->mark == 0) {
            if (pair->model != 0) {
                func_088DA060(pair->model);
                pair->model = 0;
            }
            return -3;
        }
    }
    func_088DAB7C(pair->mark_source, pair->mark);
    func_08A01F94(pair);
    return 0;
}
