/* Resolves a voice id, builds its file name, loads the file and creates its player objects (id, buffer, handle). */
typedef struct Voice { int buf; int player; int handle; int id; } Voice;
extern int func_08B009A0(int, int, int, unsigned char, int);
extern int func_08867964(void);
extern void func_0893476C(char *, const char *, int, int);
extern int func_088FE7C8(char *, int);
extern int func_088909C8(int);
extern void func_088FE708(char *, int, int, int);
extern int func_088A3E9C(void);
extern int func_088A3CAC(int, int, int);
extern const char D_08B11DD4[];
extern unsigned char D_089D5F1C;
void func_08B0D7F4(Voice *self, int a, int b, int c, unsigned char d, int e) {
    int id = func_08B009A0(a, b, c, d, e);
    char path[256];
    self->id = id;
    if (id != 0) {
        int file;
        int p;
        func_0893476C(path, D_08B11DD4, func_08867964(), self->id);
        D_089D5F1C = 1;
        file = func_088FE7C8(path, 0);
        if (file != 0) {
            int t = func_088909C8(file);
            self->buf = t;
            if (t != 0) {
                func_088FE708(path, self->buf, file, 0);
                self->player = func_088A3E9C();
                p = self->player;
                if (p != 0) {
                    self->handle = func_088A3CAC(p, self->buf, file);
                }
            }
        }
        D_089D5F1C = 0;
    }
}
