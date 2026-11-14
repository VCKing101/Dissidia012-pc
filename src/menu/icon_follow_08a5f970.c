/* Moves icon `to` onto node `node` of icon `from` when both exist. */
typedef struct FollowMatrix {
    float m[16];
} FollowMatrix;

typedef struct FollowPanel {
    char pad_00[0x3C];
    void *field_3C[8];
} FollowPanel;

extern void func_088DA0A8(void *model, int node, FollowMatrix *matrix, int *extra);
extern void func_088D9B84(void *icon, FollowMatrix *matrix);
extern void func_088D9AF4(void *icon, int *extra);

void func_08A5F970(FollowPanel *self, int from, int to, int node)
{
    FollowMatrix matrix;
    int extra;
    if (self->field_3C[from] == 0) {
        return;
    }
    if (self->field_3C[to] == 0) {
        return;
    }
    func_088DA0A8(self->field_3C[from], node, &matrix, &extra);
    func_088D9B84(self->field_3C[to], &matrix);
    func_088D9AF4(self->field_3C[to], &extra);
}
