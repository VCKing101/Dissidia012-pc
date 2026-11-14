/* Opens a score tracker: checks every argument and keeps them. */
struct ScoreTracker {
    unsigned char open;
    unsigned char pad_01[0x57];
    unsigned int side;
    void *source;
    void *models;
    int name;
    int frame;
    void *names;
    void *bones;
};

extern void func_08A21578(struct ScoreTracker *tracker);

int func_08A21660(struct ScoreTracker *tracker, unsigned int side, void *source, void *models, int name,
                  int frame, void *names, void *bones)
{
    func_08A21578(tracker);
    if (side >= 2 || source == 0 || models == 0 || name < 0 || frame < 0 || names == 0 || bones == 0) {
        return -1;
    }
    tracker->side = side;
    tracker->source = source;
    tracker->models = models;
    tracker->name = name;
    tracker->frame = frame;
    tracker->names = names;
    tracker->bones = bones;
    tracker->open = 1;
    return 0;
}
