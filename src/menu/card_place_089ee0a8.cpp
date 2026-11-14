/* Result card: places both card models on frame `frame` of `source`, then
   its three texts on the frames of the card models. */
struct Card29 {
    void *models[2];
    void *texts[3];
};

extern "C" {
void func_088DA0A8(void *model, int index, float *matrix, unsigned int *color);
void func_088D9B84(void *model, const float *matrix);
void func_088D9B48(void *model, unsigned int color);
void func_0887B778(void *text, const float *matrix);
void func_0887B820(void *text, unsigned int color);

void func_089EE0A8(Card29 *self, void *source, int frame)
{
    float matrix[16];
    unsigned int color;

    func_088DA0A8(source, frame, matrix, &color);
    for (int i = 0; i < 2; i++) {
        func_088D9B84(self->models[i], matrix);
        func_088D9B48(self->models[i], color);
    }
    void *text = self->texts[0];
    if (text != 0) {
        func_088DA0A8(self->models[0], 0, matrix, &color);
        unsigned int c = color;
        func_0887B778(text, matrix);
        func_0887B820(text, c);
    }
    text = self->texts[1];
    if (text != 0) {
        func_088DA0A8(self->models[0], 1, matrix, &color);
        unsigned int c = color;
        func_0887B778(text, matrix);
        func_0887B820(text, c);
    }
    text = self->texts[2];
    if (text != 0) {
        func_088DA0A8(self->models[1], 0, matrix, &color);
        unsigned int c = color;
        func_0887B778(text, matrix);
        func_0887B820(text, c);
    }
}
}
