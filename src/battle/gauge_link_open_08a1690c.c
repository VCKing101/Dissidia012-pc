/* Opens a gauge link between two sides: checks the arguments and starts it
 * in its first state. */
struct GaugeLink {
    unsigned char open;
    unsigned char pad_01[3];
    int state;
    unsigned char pad_08[8];
    unsigned int from;
    unsigned int to;
    void *models;
    unsigned char pad_1c[4];
    void *names;
    int first;
    int last;
};

extern void func_08A1681C(struct GaugeLink *link);

int func_08A1690C(struct GaugeLink *link, unsigned int from, unsigned int to, void *models, void *names,
                  int first, int last)
{
    func_08A1681C(link);
    if (from >= 2 || to >= 2 || models == 0 || names == 0 || first < 0 || last < 0) {
        return -1;
    }
    link->from = from;
    link->to = to;
    link->models = models;
    link->names = names;
    link->first = first;
    link->last = last;
    link->open = 1;
    link->state = 1;
    return 0;
}
