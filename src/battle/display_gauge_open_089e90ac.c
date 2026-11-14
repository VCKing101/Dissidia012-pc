/* Opens one fighter display gauge: loads its frame and bar models for the
 * side's colour and starts the bar from the side's current value. */
typedef struct GaugeModel GaugeModel;

typedef struct GaugeView {
    unsigned char open;
    unsigned char pad_01[3];
    float value;
    unsigned char pad_08[8];
    GaugeModel *frame;
    GaugeModel *bar;
    unsigned int side;
    void *source;
    void *context;
} GaugeView;

extern void func_089E9024(GaugeView *gauge);
extern unsigned int func_089E95E0(unsigned int side);
extern GaugeModel *func_088DAA40(void *source, int name, int flags);
extern int D_089E9578[2];
extern int D_089E9580[2];
extern void func_088D9F00(GaugeModel *model, float frame);
extern float func_089E97FC(unsigned int side);

int func_089E90AC(GaugeView *gauge, unsigned int side, void *source, void *context)
{
    unsigned int colour;

    func_089E9024(gauge);
    if (side >= 2 || source == 0 || context == 0) {
        return -1;
    }
    colour = func_089E95E0(side);
    if (colour >= 2) {
        return -2;
    }
    gauge->frame = func_088DAA40(source, D_089E9578[colour], 0);
    if (gauge->frame == 0) {
        return -3;
    }
    gauge->bar = func_088DAA40(source, D_089E9580[colour], 0);
    if (gauge->bar == 0) {
        return -4;
    }
    func_088D9F00(gauge->bar, 0.0f);
    gauge->value = func_089E97FC(side);
    gauge->side = side;
    gauge->source = source;
    gauge->context = context;
    gauge->open = 1;
    return 0;
}
