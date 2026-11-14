/* Effect list: twenty keyed effect slots stepped together. */
class ListEffect {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void stop();
    virtual void step(float scale, float time);
    virtual void pause();
    virtual void slot_040();
    virtual void select();
    virtual void resume();
    virtual void show();
    virtual void hide();
    virtual void set_mode(int mode);
    virtual int is_finished();
};

struct ListEntry {
    int key;
    ListEffect *effect;
};

struct FrameClock {
    unsigned char pad_00[0x18];
    float frame_scale;
};

struct EffectList {
    unsigned char open;
    unsigned char pad_01[3];
    void *owner;
    ListEntry entries[20];
    int waiting;
};

extern "C" {

extern FrameClock *D_089D69E0;
extern float func_08A22D38(void);
extern int func_08891944(void *owner);
extern void func_08891920(void *owner, int value);

int func_08A039AC(EffectList *list)
{
    if (!list->open) {
        return -1;
    }
    for (int i = 0; i < 20; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->effect != 0) {
            entry->effect->stop();
        }
    }
    return 0;
}

int func_08A03A24(EffectList *list)
{
    if (!list->open) {
        return -1;
    }
    float scale = D_089D69E0->frame_scale;
    float time = func_08A22D38();
    for (int i = 0; i < 20; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->effect != 0) {
            entry->effect->step(scale, time);
        }
    }
    if (func_08891944(list->owner) == 1 && list->waiting == 1) {
        int finished = 1;
        for (int i = 0; i < 20; i++) {
            ListEntry *entry = &list->entries[i];
            if (entry->effect != 0) {
                if (entry->effect->is_finished() == 0) {
                    finished = 0;
                }
            }
        }
        if (finished == 1) {
            func_08891920(list->owner, 0);
        }
    }
    return 0;
}

int func_08A03B60(EffectList *list)
{
    if (!list->open) {
        return -1;
    }
    for (int i = 0; i < 20; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->effect != 0) {
            entry->effect->pause();
        }
    }
    return 0;
}

}
