typedef struct Rect088759FC {
    int unk_00;
    float x;                           /* +0x04 */
    float y;                           /* +0x08 */
    float w;                           /* +0x0C */
    float h;                           /* +0x10 */
    unsigned char mode;                /* +0x14 */
} Rect088759FC;

extern int func_08875978(Rect088759FC *self);

int func_088759FC(Rect088759FC *self, unsigned char mode, float x, float y, float w, float h)
{
    if (func_08875978(self) == 0) {
        return -1;
    }
    self->x = x;
    self->y = y;
    self->w = w;
    self->h = h;
    self->mode = mode;
    return 0;
}
