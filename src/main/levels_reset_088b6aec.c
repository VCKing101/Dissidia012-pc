/* Resets the global level block: both counters to 0, both limits to 100. */
typedef struct Levels088B6AEC {
    short current;
    short pending;
    short limit;
    short max;
} Levels088B6AEC;

extern Levels088B6AEC D_09B4D488;

Levels088B6AEC *func_088B6AEC(void)
{
    Levels088B6AEC *levels;
    D_09B4D488.current = 0;
    levels = &D_09B4D488;
    levels->pending = 0;
    levels->max = 100;
    levels->limit = 100;
    return levels;
}
