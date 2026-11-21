#ifndef MAIN_AGENT33_TEXTURE_H
#define MAIN_AGENT33_TEXTURE_H

/*
 * Texture-loading path of main (EBOOT): package files, the 2D texture manager,
 * model (GMO) holders and FEP effect textures.
 *
 * Names follow build/scratch/texmap/texture_types.h and static-map.md (the
 * texture map). There every guest pointer is a uint32_t "_addr" field so the
 * layouts also work in a 64-bit host; here they are real pointers (the PSP is
 * 32-bit), keeping the map's field names.
 */

/* ------------------------------------------------------------------------
 * Effect (FEP) VRAM cache: FEP textures are copied into VRAM by a GE block
 * transfer and kept there in slots keyed by D012FepTex.cache_key.
 * ---------------------------------------------------------------------- */

typedef struct D012FepCacheSlot {
    unsigned int vram_offset;          /* +0x00 added to D012FepCache.vram_base */
    unsigned int size;                 /* +0x04 16-byte aligned */
    int key;                           /* +0x08 D012FepTex.cache_key, -1 = free */
    unsigned int age;                  /* +0x0C 0 on (re)use; the oldest is evicted */
    struct D012FepCacheSlot *next;     /* +0x10 next slot, in VRAM order */
} D012FepCacheSlot;

#define D012_FEP_CACHE_SLOTS 64

typedef struct D012FepCache {
    unsigned int vram_base;            /* +0x000 VRAM address of the cache area */
    unsigned int vram_size;            /* +0x004 bytes in the cache area */
    D012FepCacheSlot slot_pool[D012_FEP_CACHE_SLOTS]; /* +0x008 */
    D012FepCacheSlot *free_slots[D012_FEP_CACHE_SLOTS]; /* +0x508 unused pool slots (a stack) */
    int free_count;                    /* +0x608 */
    D012FepCacheSlot *slots;           /* +0x60C slots in use or free, covering the whole area */
} D012FepCache;

/* ------------------------------------------------------------------------
 * File system object at *(0x089D5EA0): how package files are read.
 * ---------------------------------------------------------------------- */

typedef struct D012FileSystem {
    unsigned char sync;                /* +0x00 1: read synchronously (FUN_0885CBD8) */
    char pad_01[3];
    struct D012Device *device;         /* +0x04 C++ device object (D012Device below) */
    void *package;                     /* +0x08 PACKAGE.BIN reader (index at 0x09B26BC0) */
} D012FileSystem;

/* What a lookup in the preloaded-file cache (FUN_08883538) hands back. */
typedef struct D012FileCacheHit {
    void *data;                        /* +0x00 the file in memory */
    int size;                          /* +0x04 */
    int id;                            /* +0x08 */
} D012FileCacheHit;

/* ------------------------------------------------------------------------
 * Model (GMO) holders: *(0x089D5F94) + 0x1C, 0xC0 holders of 0x13C bytes.
 * A model file may hold several GMOs ("parts"); each gets its own model.
 * ---------------------------------------------------------------------- */

typedef struct D012ModelPart {
    int unk_00;                        /* +0x00 */
    int unk_04;                        /* +0x04 */
    void *model_addr;                  /* +0x08 D012GmoModel* (0 = holder free) */
    void *gmo_data_addr;               /* +0x0C source GMO inside the loaded file */
#ifdef __cplusplus
    /* A holder is in use while its first part has a model. */
    static bool loaded(D012ModelPart *part) { return part->model_addr != 0; }
#endif
} D012ModelPart;

/* Holder build flags (+0x90). */
#define D012_MODEL_BUILD_PASS1   0x01
#define D012_MODEL_BUILD_PASS2   0x02
#define D012_MODEL_BUILD_PARTS   0x04  /* always set for fighters */
#define D012_MODEL_BUILD_VRAM    0x08  /* copy the textures into VRAM (stages; fighters named R_*) */
#define D012_MODEL_BUILD_ALT     0x10
#define D012_MODEL_BUILT         0x20
#define D012_MODEL_BUILD_TRIED   0x40

typedef struct D012ModelHolder {
    unsigned int unk_00;               /* +0x00 */
    D012ModelPart parts[8];            /* +0x04 */
    D012ModelPart *current_part_addr;  /* +0x84 */
    unsigned int unk_88;               /* +0x88 non-zero skips the lazy build */
    signed char part_count;            /* +0x8C GMOs in the file (parts used) */
    signed char pending_part;          /* +0x8D part to switch to (FUN_0886A54C), -1 = none */
    unsigned char motion_count;        /* +0x8E motions of the model (FUN_0886A090) */
    unsigned char unk_8F;              /* +0x8F */
    unsigned int flags;                /* +0x90 D012_MODEL_BUILD_* */
    unsigned int *draw_lists[2];       /* +0x94 precompiled GE lists of the model (FUN_0886E86C), 0 = none */
    unsigned int draw_list_sizes[2];   /* +0x9C bytes */
    unsigned char unk_A4[0x10C - 0xA4];
    unsigned char *motion_slots;       /* +0x10C 3 bytes per motion (FUN_0886A090) */
    char name[0x10];                   /* +0x110 (15 chars) */
    unsigned char unk_120[0x128 - 0x120];
    int release_delay;                 /* +0x128 frames until the holder is released (0 = not pending) */
    unsigned char unk_12C[0x13C - 0x12C];
} D012ModelHolder;

#define D012_MODEL_HOLDERS 0xC0

typedef struct D012ModelBank {
    unsigned char unk_00[0x1C];
    D012ModelHolder holders[D012_MODEL_HOLDERS]; /* +0x1C */
} D012ModelBank;

/* ------------------------------------------------------------------------
 * Image library (Sony, 0x0899B6B0-0x0899F3FC): 2D GIM/TIM2/BMP textures.
 * ---------------------------------------------------------------------- */

typedef struct D012ImageObject D012ImageObject;   /* 0x90 bytes, see texture_types.h */

#ifdef __cplusplus
extern "C" {
#endif
D012ImageObject *func_0899B9F0(void *picture);                                 /* img_create */
void func_0899BB1C(D012ImageObject *image);                                    /* img_destroy */
int func_0899BB68(D012ImageObject *image, const void *data, int size, int index); /* img_load */
int func_0899D2F8(D012ImageObject *image);                                     /* img_bind */
#ifdef __cplusplus
}
#endif

/* ------------------------------------------------------------------------
 * 2D texture manager (*(0x089D7830) == 0x08B87E7C).
 * ---------------------------------------------------------------------- */

/* A loaded 2D texture (270 in a pool at manager + 0xF444). */
typedef struct D012TexEntry {
    D012ImageObject *image_addr;       /* +0x00 0 when free */
    unsigned short refcount;           /* +0x04 1 after load */
    unsigned short unk_06;             /* +0x06 0xFFFF from every package caller */
    unsigned short id;                 /* +0x08 pack base_id + index, or the caller's id */
    unsigned char group;               /* +0x0A owner group (e.g. 0x23: party-battle title) */
    unsigned char pad_0B;
    struct D012TexEntry *next_addr;    /* +0x0C manager list */
    struct D012TexEntry *prev_addr;    /* +0x10 */
} D012TexEntry;

/* Slot of the manager's object stock rings (take/return pointers walk these). */
typedef struct D012StockSlot {
    void *object_addr;                 /* +0x00 object, 0 while taken */
    struct D012StockSlot *next_addr;   /* +0x04 next slot (ring) */
} D012StockSlot;

/* A sprite instance (0x2C bytes, 252 of them): one drawn picture of a layout. */
typedef struct D012Sprite {
    int unk_00;                        /* +0x00 */
    int unk_04;                        /* +0x04 */
    int unk_08;                        /* +0x08 */
    int unk_0C;                        /* +0x0C */
    D012TexEntry *tex;                 /* +0x10 texture drawn (0 = none) */
    unsigned char flags;               /* +0x14 bit 0: in use, bit 4: in draw list 1 */
    unsigned char layer;               /* +0x15 draw order key (FUN_088DB4E4) */
    unsigned char unk_16;
    unsigned char unk_17;              /* +0x17 */
    unsigned char unk_18[0x24 - 0x18];
    struct D012Sprite *next;           /* +0x24 draw list */
    struct D012Sprite *prev;           /* +0x28 */
} D012Sprite;

/* A list of sprite instances (linked through D012Sprite.next). */
typedef struct D012SpriteList {
    D012Sprite *head;                  /* +0x00 */
    D012Sprite *tail;                  /* +0x04 */
    int count;                         /* +0x08 */
} D012SpriteList;

/* The texture manager (only the parts the texture path uses; the object is 0x109xx bytes). */
typedef struct D012TexManager {
    unsigned int unk_00000;
    void *heap_a;                      /* +0x00004 staging heap when heap_b is 0 */
    void *heap_b;                      /* +0x00008 preferred staging heap */
    unsigned char unk_0000C[0x20 - 0xC];
    unsigned char draw_enabled;        /* +0x00020 */
    unsigned char unk_00021[2];
    unsigned char use_file_cache;      /* +0x00023 1: read through the preloaded-file cache */
    D012SpriteList lists[2];           /* +0x00024 sprites drawn by FUN_088CED50 / FUN_088CEE0C */
    unsigned char unk_0003C[0x336C - 0x3C];
    D012StockSlot *sprite_take;        /* +0x0336C sprite stock: next to take */
    D012StockSlot *sprite_return;      /* +0x03370 next to give back */
    unsigned char unk_03374[0xC4F4 - 0x3374];
    struct D012SeqObject *seq_head;    /* +0x0C4F4 seq objects (0x74 bytes): next +0x6C, prev +0x70 */
    struct D012SeqObject *seq_tail;    /* +0x0C4F8 */
    int seq_count;                     /* +0x0C4FC */
    unsigned char unk_0C500[0xEBC0 - 0xC500];
    D012StockSlot *seq_take;           /* +0x0EBC0 */
    D012StockSlot *seq_return;         /* +0x0EBC4 */
    D012TexEntry *tex_head;            /* +0x0EBC8 loaded textures */
    D012TexEntry *tex_tail;            /* +0x0EBCC */
    int tex_count;                     /* +0x0EBD0 */
    D012StockSlot tex_slots[270];      /* +0x0EBD4 ring */
    D012TexEntry tex_pool[270];        /* +0x0F444 */
    D012StockSlot *tex_take;           /* +0x1095C */
    D012StockSlot *tex_return;         /* +0x10960 */
    unsigned int unk_10964;
    D012TexEntry *(*tex_alloc_cb)(void);   /* +0x10968 optional: replaces the take */
    void (*tex_free_cb)(D012TexEntry *);   /* +0x1096C optional: replaces the return */
} D012TexManager;

/* ------------------------------------------------------------------------
 * FEP effect textures (TEX blocks inside an effect file).
 * ---------------------------------------------------------------------- */

typedef struct D012FepTexLevel {
    unsigned int width;                /* +0x00 */
    unsigned int height;               /* +0x04 */
    unsigned int pixel_offset;         /* +0x08 from the TEX block start */
} D012FepTexLevel;

typedef struct D012FepTex {
    unsigned int unk_00;               /* +0x00 */
    unsigned int cache_key;            /* +0x04 key of the effect VRAM cache */
    unsigned int unk_08;               /* +0x08 */
    unsigned int unk_0C;               /* +0x0C */
    unsigned int format_index;         /* +0x10 0..7 -> GE texture format (table 0x089D0628) */
    int level_count;                   /* +0x14 */
    unsigned int clut_offset;          /* +0x18 D012FepClut, from the TEX block start */
    D012FepTexLevel levels[1];         /* +0x1C level_count of them; always swizzled */
} D012FepTex;

typedef struct D012FepClut {
    unsigned int entry_count;          /* +0x00 */
    unsigned int format_index;         /* +0x04 0..3 -> GE CLUT format (table 0x089D066C) */
    unsigned int unk_08;               /* +0x08 */
    unsigned int unk_0C;               /* +0x0C */
    /* palette data                       +0x10 */
} D012FepClut;

/* What the effect manager keeps per texture: the first word is the TEX block. */
typedef struct D012FepTexRef {
    D012FepTex *tex;
} D012FepTexRef;

/* The parts of the effect manager used to bind textures. */
typedef struct D012FepManager {
    D012FepTexRef **textures;          /* +0x000 one per texture index */
    unsigned char unk_004[0x18 - 0x4];
    int current_texture;               /* +0x018 index bound last */
    unsigned char unk_01C[0x30 - 0x1C];
    D012FepCache tex_cache;            /* +0x030 VRAM cache for textures */
    D012FepCache clut_cache;           /* +0x640 VRAM cache for palettes */
    unsigned char unk_C50_pad[0xC50 - 0xC50];
    int use_tex_cache;                 /* +0xC50 */
    int use_clut_cache;                /* +0xC54 */
} D012FepManager;

/* Language container a package sub-file may be wrapped in ("mlng"). */
typedef struct D012MlngHeader {
    char magic[4];                     /* +0x00 "mlng" */
    unsigned int count;                /* +0x04 number of languages */
    unsigned int table[1];             /* +0x08 offset[count] (from this header), then size[count] */
} D012MlngHeader;

/* ------------------------------------------------------------------------
 * Archive files (a loaded file holding named entries; FUN_08869A7C/08869AB4).
 * ---------------------------------------------------------------------- */

typedef struct D012ArchiveEntry {
    const char *name;                  /* +0x00 (0 = unnamed) */
    void *data;                        /* +0x04 */
    int size;                          /* +0x08 */
    int unk_0C;                        /* +0x0C */
} D012ArchiveEntry;

typedef struct D012ArchiveHeader {
    int unk_00;                        /* +0x00 */
    int unk_04;                        /* +0x04 */
    int count;                         /* +0x08 */
    int unk_0C;                        /* +0x0C */
    D012ArchiveEntry entries[1];       /* +0x10 count of them */
} D012ArchiveHeader;

typedef struct D012Archive {
    D012ArchiveHeader *header;         /* +0x00 */
} D012Archive;

/* One entry of a package load list; the list ends with kind 0xFFFF. */
typedef struct D012PkgListEntry {
    unsigned short kind;               /* +0x00 0 = GIM pack (textures), 1 = seq pack (layout objects) */
    unsigned short pad_02;
    const char *path;                  /* +0x04 package path, localised before the lookup */
} D012PkgListEntry;

/* ------------------------------------------------------------------------
 * PACKAGE.BIN reader (D012FileSystem.package).
 * ---------------------------------------------------------------------- */

/* One entry of the PACKAGE_INFO index (sorted by crc; count at 0x09B26BC8, entries at 0x09B26BD0). */
typedef struct D012PkgIndexEntry {
    unsigned int crc;                  /* +0x00 pkg_path_hash of the path */
    unsigned int sector;               /* +0x04 low 20 bits: 2048-byte sector in PACKAGE.BIN */
    unsigned int size;                 /* +0x08 low 28 bits: bytes; high 4 bits: flags */
} D012PkgIndexEntry;

#ifdef __cplusplus
/* The device object that does the reads (its vtable pointer sits after 0x154 bytes of data). */
struct D012Device {
    char data[0x154];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual int read(int file, void *dst, unsigned int size, int flag);    /* 3 */
    virtual void v04();
    virtual void v05();
    virtual int read_path(const char *path, void *dst, int size);          /* 6 */
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual int size_path(const char *path);                               /* 15 */
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual int seek(int file, int offset, int whence);                    /* 27 */
};
#else
typedef struct D012Device D012Device;
#endif

typedef struct D012Package {
    int file;                          /* +0x00 PACKAGE.BIN handle */
    struct D012Device *device;         /* +0x04 */
    char last_path[0x50];              /* +0x08 one-entry lookup cache */
    D012PkgIndexEntry *last_entry;     /* +0x58 */
} D012Package;

/* PACKAGE_INFO.BIN, read whole into 0x09B26BC0. */
typedef struct D012PkgIndex {
    unsigned int unk_00;               /* +0x00 */
    unsigned int unk_04;               /* +0x04 */
    int count;                         /* +0x08 */
    unsigned int unk_0C;               /* +0x0C */
    D012PkgIndexEntry entries[1];      /* +0x10 sorted by crc */
} D012PkgIndex;

/* A seq (layout) object (0x74 bytes, 80 of them; list at manager + 0xC4F4). */
typedef struct D012SeqObject {
    unsigned char unk_00[0x5C];
    void *block;                       /* +0x5C block from the manager's heap_a */
    int unk_60;                        /* +0x60 -1 = none, else freed through FUN_088A1638 */
    short refcount;                    /* +0x64 1 after seq_load */
    unsigned short id;                 /* +0x66 pack base_id + index */
    unsigned char group;               /* +0x68 */
    unsigned char unk_69[3];
    struct D012SeqObject *next;        /* +0x6C */
    struct D012SeqObject *prev;        /* +0x70 */
} D012SeqObject;

#ifdef __cplusplus
extern "C" D012TexManager *D_089D7830;

/* The texture stock as its own object (manager + 0xEBD4): a ring of slots over a pool. */
struct D012TexPool {
    D012StockSlot slots[270];          /* +0x0000 */
    D012TexEntry entries[270];         /* +0x0870 */
    D012StockSlot *take_ptr;           /* +0x1D88 (manager + 0x1095C) */
    D012StockSlot *return_ptr;         /* +0x1D8C */

    static D012TexPool *of(D012TexManager *mgr) { return (D012TexPool *)((char *)mgr + 0xEBD4); }
    /* Take the next free entry off the ring. */
    D012TexEntry *take() {
        D012TexEntry *entry = (D012TexEntry *)take_ptr->object_addr;
        take_ptr->object_addr = 0;
        take_ptr = take_ptr->next_addr;
        return entry;
    }
};

/* The manager's texture entries: allocation (callback or stock ring) and the loaded list. */
struct D012TexEntries {
    static D012TexEntry *alloc(D012TexManager *mgr) {
        if (mgr->tex_alloc_cb != 0) {
            return mgr->tex_alloc_cb();
        }
        return D012TexPool::of(D_089D7830)->take();
    }
    /* Put an entry back on the ring (the walk to a free slot is kept from the original;
     * its result is not used). */
    static void give_back(D012TexManager *mgr, D012TexEntry *entry) {
        D012StockSlot *slot;
        D012StockSlot *ret = mgr->tex_return;
        for (slot = mgr->tex_take; slot->object_addr != 0; slot = slot->next_addr) {
        }
        ret->object_addr = entry;
        mgr->tex_return = mgr->tex_return->next_addr;
    }
    static void release(D012TexManager *mgr, D012TexEntry *entry) {
        if (mgr->tex_free_cb != 0) {
            mgr->tex_free_cb(entry);
        } else {
            give_back(mgr, entry);
        }
    }
    /* Append to the loaded-texture list. */
    static void push_back(D012TexManager *mgr, D012TexEntry *entry) {
        if (mgr->tex_tail == 0) {
            mgr->tex_tail = entry;
            mgr->tex_head = entry;
            entry->next_addr = 0;
            entry->prev_addr = 0;
        } else {
            D012TexEntry **link = &mgr->tex_tail->next_addr;
            *link = entry;
            D012TexEntry *prev = mgr->tex_tail;
            entry->next_addr = 0;
            entry->prev_addr = prev;
            mgr->tex_tail = entry;
        }
        mgr->tex_count++;
    }
};
#endif

/* @@append */

#endif
