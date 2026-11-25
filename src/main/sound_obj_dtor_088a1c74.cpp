// Deleting destructor of a pooled sound object: resets it to the base class, and when freeing, returns it to its pool's free list.
struct SoundPool1 {
    char storage[0x500];
    void *free_list[0x40];
    int free_count;
    static unsigned char Owns(SoundPool1 *pool, void *p)
    {
        int owns = 0;
        if ((char *)p >= pool->storage && (char *)p < pool->storage + 0x500) { owns = 1; }
        return owns;
    }
    static void Free(SoundPool1 *pool, void *p)
    {
        if (Owns(pool, p) == 1) { pool->free_list[pool->free_count++] = p; }
    }
};
struct SoundObj { void *vtable; };
extern SoundPool1 *D_09B4D2A0;
extern char D_089CAC90[];
extern "C" void func_088A30D8(SoundObj *self, int flags);
struct H {
    static void run(SoundObj *self, unsigned int flags)
    {
        if (self != 0) {
            self->vtable = D_089CAC90;
            func_088A30D8(self, 0);
            if (flags & 1) { SoundPool1::Free(D_09B4D2A0, self); }
        }
    }
};
extern "C" void func_088A1C74(SoundObj *self, unsigned int flags) { H::run(self, flags); }
