/* Refreshes the stage's item models: rebuilds their view from the camera,
   then shows each item that is not locked, hidden or shown by its flag. */
typedef struct ModelPart {
    int a;
    int b;
    unsigned int flags;
} ModelPart;

typedef struct StageModel {
    char pad_000[0x108];
    ModelPart *parts;
} StageModel;

typedef struct StageItem {
    char pad_00[0x44];
    int hidden;
    char pad_48[4];
    int part;
    char pad_50[0x50 - 0x50];
} StageItem;

typedef struct Stage {
    char pad_000[0x74];
    StageModel *model;
    char pad_078[0x6B8 - 0x78];
    StageItem *items;
    int count;
} Stage;

typedef struct ViewData {
    char data[0x50];
} ViewData;

extern Stage *D_089D6AC8;
extern char *D_089D5EC0;

extern void func_08866BA8(char *camera);
extern void func_089A25C0(void);
extern void func_0885E860(void *matrix);
extern void func_0889E7A0(ViewData *view, void *eye, void *matrix);
extern void func_08894B64(ViewData *view, StageItem *items, int flag, int count);
extern void func_0886BB44(StageModel *model, int part, int show, int a, int b);

void func_088935CC(void)
{
    Stage *stage = D_089D6AC8;

    if (stage->model != 0) {
        StageItem *items;
        int count;

        func_08866BA8(D_089D5EC0);
        func_089A25C0();
        items = stage->items;
        count = stage->count;
        if (count > 0) {
            ViewData view;
            char *eye = D_089D5EC0 + 0x7E0;
            char *matrix = D_089D5EC0 + 0x720;
            int i;

            func_0885E860(matrix);
            func_0889E7A0(&view, eye, matrix);
            func_08894B64(&view, items, 1, count);
            for (i = 0; i < count; i++) {
                StageItem *item = &items[i];
                if (!(unsigned char)((stage->model->parts[item->part].flags & 0x20000) != 0)) {
                    if (item->hidden != 0) {
                        func_0886BB44(stage->model, item->part, 0, 0, 0);
                    } else {
                        func_0886BB44(stage->model, item->part, 1, 0, 0);
                    }
                }
            }
        }
    }
}
