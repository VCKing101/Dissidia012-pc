/* Reloads the bound timer's model and, when it has one, its mark, then
 * attaches them. */
typedef struct BoundTimerData {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    void *mark;
    unsigned char pad_0c[4];
    void *source;
    int name;
    void *mark_source;
    int mark_name;
} BoundTimerData;

extern void func_088DA060(void *model);
extern void *func_088DAA40(void *source, int name, int flags);
extern int func_08A10C88(BoundTimerData *timer);

int func_08A10DB0(BoundTimerData *timer)
{
    if (!timer->open) {
        return -1;
    }
    if (timer->mark != 0) {
        func_088DA060(timer->mark);
        timer->mark = 0;
    }
    if (timer->model != 0) {
        func_088DA060(timer->model);
        timer->model = 0;
    }
    timer->model = func_088DAA40(timer->source, timer->name, 0);
    if (timer->model == 0) {
        return -2;
    }
    if (timer->mark_source != 0 && timer->mark_name >= 0) {
        timer->mark = func_088DAA40(timer->mark_source, timer->mark_name, 0);
        if (timer->mark == 0) {
            if (timer->model != 0) {
                func_088DA060(timer->model);
                timer->model = 0;
            }
            return -3;
        }
    }
    func_08A10C88(timer);
    return 0;
}
