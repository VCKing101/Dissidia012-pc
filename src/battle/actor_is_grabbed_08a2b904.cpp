/* True when the fighter's current action is a throw (action 0x1F) in one of
 * its eight hold steps. */
struct ActorAction {
    int id;
};

struct ActorMoveState {
    unsigned char pad_00[0x10];
    unsigned char step;
};

struct GrabActor {
    unsigned char pad_000[0x3A4];
    ActorAction *action;
    ActorMoveState *move;

    static bool has_action(GrabActor *actor)
    {
        return actor->action != 0;
    }
};

extern "C" int func_08A2B904(GrabActor *actor)
{
    if (actor == 0) {
        return 0;
    }
    if (!GrabActor::has_action(actor)) {
        return 0;
    }
    if (actor->action->id != 0x1F) {
        return 0;
    }
    switch (actor->move->step) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        return 1;
    }
    return 0;
}
