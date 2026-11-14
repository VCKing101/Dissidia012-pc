/* Course gauge: shows the time digits for the current position, then the
   icon's motion 7 (mode 3) when +0x4C is set, else mode 4. */
struct Gauge29 {
    char pad_00[0x8];
    void *icon;       /* +0x08 */
    void *models[6];  /* +0x0C, used last to first */
    void *digits[6];  /* +0x24, used last to first */
    int mode;         /* +0x3C */
    int end;          /* +0x40 */
    int pos;          /* +0x44 */
    int speed;        /* +0x48 */
    unsigned char field_4C;
};

extern "C" {
extern const int D_089EC2F8[];
void func_089EB50C(Gauge29 *self, int *motions, int pos, const int *table);
void func_088D9F5C(void *model, int motion, bool arg);

void func_089EB400(Gauge29 *self)
{
    void *digits[6] = {self->digits[5], self->digits[4], self->digits[3],
                       self->digits[2], self->digits[1], self->digits[0]};
    int motions[6];
    func_089EB50C(self, motions, self->pos, D_089EC2F8);
    for (int i = 0; i < 6; i++) {
        func_088D9F5C(digits[i], motions[i], true);
    }
    if (self->field_4C == 1) {
        func_088D9F5C(self->icon, 7, true);
        *(short *)((char *)self->icon + 0xE) = 8;
        self->mode = 3;
    } else {
        self->mode = 4;
    }
}
}
