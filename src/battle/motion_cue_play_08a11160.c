/* Plays a motion on a freshly created cue model. */
typedef struct CueModel {
    unsigned char pad_00[0xE];
    short playing;
} CueModel;

typedef struct MotionCue {
    unsigned char open;
    unsigned char pad_01[3];
    void *source;
    CueModel *model;
    unsigned char closing;
} MotionCue;

extern void func_088DA060(void *model);
extern CueModel *func_088DAA68(void *source);
extern void func_088D9F5C(CueModel *model, int motion, int loop);

int func_08A11160(MotionCue *cue, int motion, int playing)
{
    if (!cue->open) {
        return -1;
    }
    if (cue->model != 0) {
        func_088DA060(cue->model);
        cue->model = 0;
    }
    cue->model = func_088DAA68(cue->source);
    if (cue->model == 0) {
        return -2;
    }
    func_088D9F5C(cue->model, motion, 1);
    cue->model->playing = playing;
    cue->closing = 0;
    return 0;
}
