typedef struct Callbacks08863D4C {
    int open;                          /* +0x00 */
    int read;                          /* +0x04 */
    int size;                          /* +0x08 */
    int seek;                          /* +0x0C */
    int power;                         /* +0x10 */
} Callbacks08863D4C;

extern char D_089C92B0[];              /* "FMI_OpenAsyncCallback" */
extern char D_089C92C8[];              /* "FMI_ReadAsyncCallback" */
extern char D_089C92E0[];              /* "FMI_SizeAsyncCallback" */
extern char D_089C92F8[];              /* "FMI_SeekAsyncCallback" */
extern char D_089C9310[];              /* "FMI_PowerCallback" */
extern int func_089C5F80(const char *name, void *fn, void *arg);   /* create callback */
extern int func_089C57F8(int slot, int callback);                  /* register the power callback */
extern void func_088636C4(void);
extern void func_088637A8(void);
extern void func_08863890(void);
extern void func_08863968(void);
extern void func_08863C98(void);

/* Create the file manager's async callbacks (open, read, size, seek) and its power callback. */
int func_08863D4C(Callbacks08863D4C *self)
{
    self->open = func_089C5F80(D_089C92B0, func_088636C4, self);
    self->read = func_089C5F80(D_089C92C8, func_088637A8, self);
    self->size = func_089C5F80(D_089C92E0, func_08863890, self);
    self->seek = func_089C5F80(D_089C92F8, func_08863968, self);
    if (self->open <= 0 || self->read <= 0 || self->size <= 0 || self->seek <= 0) {
        return 0;
    }
    self->power = func_089C5F80(D_089C9310, func_08863C98, self);
    func_089C57F8(3, self->power);
    return 1;
}
