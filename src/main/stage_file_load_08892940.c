/* The stage object, only the fields used here. */
typedef struct Stage08892940 {
    void *heap;                        /* +0x000 */
    unsigned char unk_004[0x730 - 0x4];
    void *skill_work;                  /* +0x730 0x2800-byte block for "SUPER SKILL" stages */
} Stage08892940;

extern void *func_0885C6CC(void *heap, int size, int align, int tag, int one, int from_end); /* heap_alloc */
extern void func_0885C538(void *heap, void *block, int align);                              /* heap_free */
extern int func_088FE7C8(const char *path, unsigned char alt);                   /* file_cache_size */
extern int func_088FE708(const char *path, void *dst, int size, unsigned char alt); /* file_cache_read */
extern void func_088FE944(const char *path, void *data, int size);              /* post-process the file */
/* stage_setup2: build the stage's models; textures to VRAM unless the descriptor's +0x48 is set. */
extern void func_08893FF4(Stage08892940 *self, void *data, int size, void *desc, unsigned char super_skill);

/* stage_file_load: read a stage file (into `buffer`, or a staging block when it is 0) and
 * build its models. */
void func_08892940(Stage08892940 *self, const char *path, void *buffer, unsigned char post,
                   void *desc, unsigned char super_skill)
{
    int own = 0;
    int size;
    int read;

    if (super_skill == 1) {
        int one = 1;

        self->skill_work = func_0885C6CC(self->heap, 0x2800, 0, 0, one, 0);
    }
    size = func_088FE7C8(path, 0);
    if (buffer == 0) {
        own = 1;
        buffer = func_0885C6CC(self->heap, size, 0x40, 0, own, 1);
    }
    read = func_088FE708(path, buffer, size, 0);
    if (post) {
        func_088FE944(path, buffer, size);
    }
    func_08893FF4(self, buffer, read, desc, super_skill);
    if (own) {
        func_0885C538(self->heap, buffer, 0x40);
    }
}
