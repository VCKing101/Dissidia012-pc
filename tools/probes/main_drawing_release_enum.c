/* Nonmatching candidate: excluded from all source manifests and progress. */
enum MainDrawingReleaseMode {ReleaseModeZero = 0};
typedef struct MainDrawingReleasePrefix {
    unsigned char unknown_0000[0x14];
    unsigned char field_0014;
} MainDrawingReleasePrefix;

extern void *D_089D7830;
extern void func_088CF314(void *, void *, enum MainDrawingReleaseMode);

void func_088DAB44(void *object) {
    MainDrawingReleasePrefix *self = object;
    self->field_0014 &= ~1;
    func_088CF314(D_089D7830, self, ReleaseModeZero);
}
