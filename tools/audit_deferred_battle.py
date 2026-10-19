"""Validate the deferred callback path and explicit battle-mode load barrier.

The setup handler is the only additional source of overlay roots. Update and
teardown callbacks are recorded without assuming their overlay state.
"""

import struct


def inspect_deferred_battle(image, functions, review, slots):
    by_address = {int(f["address"], 16): f for f in functions}

    def addr(field):
        return int(review[field], 16)

    def expect(pc, instruction, label):
        if image.word(pc, True) != instruction:
            raise ValueError(f"Deferred battle {label} differs")

    def call(pc, target):
        word = image.word(pc, True)
        if word >> 26 != 3 or (((pc+4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)) != target:
            raise ValueError("Deferred battle call target differs")

    required = [addr(k) for k in ("registration", "setter", "dispatcher", "setup", "ensure_helper", "controller_constructor")]
    for callback in review["callbacks"]:
        required += [int(callback[k], 16) for k in ("wrapper", "handler", "runner")]
    if any(entry not in by_address for entry in required):
        raise ValueError("Deferred battle path lacks a main function entry")
    slot = next((s for s in slots if s["index"] == review["mode_index"]), None)
    if slot is None or slot["module"] != "battle":
        raise ValueError("Deferred battle mode is bound to another overlay")
    mode = review["mode_index"]
    if (mode != 1 or review["callback_object_offset"] != 4 or
            [c["phase"] for c in review["callbacks"]] != ["setup", "update", "teardown"]):
        raise ValueError("Deferred battle mode/object offset differs from the reviewed path")

    # The entry reaches the ensure call without a branch, jump or indirect
    # transfer. Its delay slot loads the manager, not an overlay object.
    setup_entry = addr("setup")
    setup_prologue = (0x27BDEF80, 0xAFB61070, 0x0080B025, 0x3C04089E,
                      0xAFB01058, 0xAFB1105C, 0xAFB21060, 0xAFB31064,
                      0xAFB41068, 0xAFB5106C, 0xAFB71074, 0xAFBE1078,
                      0xAFBF107C)
    if addr("ensure_call_pc") != setup_entry+len(setup_prologue)*4:
        raise ValueError("Deferred setup entry does not lead directly to the battle ensure")
    for index, instruction in enumerate(setup_prologue):
        expect(setup_entry+index*4, instruction, "setup entry before mode barrier")
    expect(addr("ensure_call_pc")+4, 0x8C8485E8, "ensure manager argument")

    # The helper either returns with mode 1 already active, or invokes the
    # already-audited main loader with a1=1. Verify both arms and the return.
    helper = addr("ensure_helper")
    helper_words = [0x27BDFFF0, 0x80850000, 0x34060000 | mode, 0xAFBF0000,
                    0x10A60003, 0, 0x0C000000 | ((0x08933ACC >> 2) & 0x3FFFFFF),
                    0x34050000 | mode, 0x8FBF0000, 0x03E00008, 0x27BD0010]
    if by_address[helper]["body_bytes"] != 44:
        raise ValueError("Battle ensure helper boundary differs")
    for index, word in enumerate(helper_words):
        expect(helper+index*4, word, "ensure-mode control flow")
    call(addr("ensure_call_pc"), helper)
    call(addr("prefix_call_pc"), addr("prefix"))
    if addr("ensure_call_pc") >= addr("prefix_call_pc"):
        raise ValueError("Battle prefix precedes its mode barrier")

    registration = addr("registration_call_pc")
    call(registration, addr("setter"))
    expect(0x088896C4, 0x26040004, "registration object pointer")
    setter = addr("setter")
    for index, word in enumerate((0xAC850018, 0xAC86001C, 0x03E00008, 0xAC870020)):
        expect(setter+index*4, word, "callback setter slots")

    # The common constructor installs this member table. The state-machine
    # dispatcher uses records +8/+16/+24, each {s16 adjustment,s16 flags,u32 fn}.
    expect(0x08864FF0, 0x3C08089D, "member table high address")
    expect(0x08864FF4, 0x25089348, "member table low address")
    expect(0x08864FF8, 0xAC880030, "member table object field")
    if addr("member_table") != 0x089C9348:
        raise ValueError("Deferred member table address differs")
    expect(0x0888821C, 0x26040004, "controller callback-block construction")
    call(0x08888228, 0x08864FF0)
    call(addr("startup_constructor_call_pc"), addr("controller_constructor"))
    expect(addr("startup_constructor_call_pc")+4, 0x02E02025, "startup controller argument")
    expect(addr("startup_global_store_pc")-4, 0x3C04089D, "startup controller global high address")
    expect(addr("startup_global_store_pc"), 0xAC976A10, "startup controller global store")

    callbacks = []
    for index, callback in enumerate(review["callbacks"]):
        register = 5+index
        high = image.word(int(callback["high_pc"], 16), True)
        low = image.word(int(callback["low_pc"], 16), True)
        immediate = low & 0xFFFF
        immediate = immediate if immediate < 0x8000 else immediate-0x10000
        wrapper, handler, runner = (int(callback[k], 16) for k in ("wrapper", "handler", "runner"))
        if (high >> 16 != (0x3C00 | register) or low >> 26 != 9 or
                (low >> 21) & 31 != register or (low >> 16) & 31 != register or
                ((high & 0xFFFF) << 16)+immediate != wrapper):
            raise ValueError("Registered callback pointer differs")
        call(int(callback["call_pc"], 16), handler)
        expect(wrapper+4, 0x3C04089D, "wrapper controller high address")
        expect(wrapper+16, 0x8C846A10, "wrapper controller load")
        if addr("controller_global") != 0x089D6A10:
            raise ValueError("Callback wrapper controller global differs")
        member = addr("member_table")+callback["member_offset"]
        adjustment, flags, target = struct.unpack("<hhI", image.read(member, 8))
        if adjustment or flags or target != runner:
            raise ValueError("Deferred member record differs from its direct form")
        offset = callback["slot_offset"]
        if offset != (0x18, 0x1C, 0x20)[index] or callback["member_offset"] != (8, 16, 24)[index]:
            raise ValueError("Deferred callback/member phase offsets differ")
        expect(runner+4, 0x8C840000 | offset, "runner callback slot")
        expect(runner+12, 0x10800003, "runner null guard")
        expect(runner+20, 0x0080F809, "runner indirect callback")
        pc = int(review["dispatcher_calls"][index], 16)
        for relative, word in ((-20 if index == 0 else -16, 0x8E050030 if index == 0 else 0x8E040030),
                               (-12, (0x24A40000 if index == 0 else 0x24840000) | callback["member_offset"]),
                               (-8, 0x84850000), (-4, 0x8C860004), (0, 0x00C0F809), (4, 0x02052021)):
            expect(pc+relative, word, "member dispatch")
        callbacks.append({"phase": callback["phase"], "wrapper": f"main:0x{wrapper:08X}",
                          "handler": f"main:0x{handler:08X}", "runner": f"main:0x{runner:08X}",
                          "controller_field_offset": 4+offset, "member_record": f"0x{member:08X}"})

    # Include only actual setup JAL targets after the ensure call. Guard this
    # interval against direct calls to another overlay-manager transition.
    setup = by_address[addr("setup")]
    roots, calls = [], []
    lower, upper = int(slot["payload_start"], 16), int(slot["payload_end"], 16)
    for region in setup["ranges"]:
        for pc in range(int(region["start"], 16), int(region["end"], 16), 4):
            word = image.word(pc, True)
            if word >> 26 != 3:
                continue
            target = ((pc+4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
            calls.append(pc)
            if addr("overlay_manager_start") <= target < addr("overlay_manager_end") and pc != addr("ensure_call_pc"):
                raise ValueError("Deferred setup contains another direct overlay transition")
            if lower <= target < upper:
                if pc <= addr("ensure_call_pc"):
                    raise ValueError("Overlay setup call precedes the battle mode barrier")
                roots.append({"address": f"0x{target:08X}", "caller": setup["id"], "call_pc": f"0x{pc:08X}",
                              "dispatch_index": mode, "context": "explicit battle ensure in deferred setup"})
    if not calls or calls[0] != addr("ensure_call_pc"):
        raise ValueError("Battle ensure is not the setup handler's first direct call")
    if not any(r["address"] == review["prefix"] and r["call_pc"] == review["prefix_call_pc"] for r in roots):
        raise ValueError("Battle prefix call is outside the checked setup body")
    return {"scope": "Static callback registration/member dispatch and battle ensure at setup entry; direct setup overlay targets only. Runtime scenarios, indirect transitions and update/teardown overlay effects unverified",
            "registration": f"main:{review['registration']}", "setup": setup["id"],
            "mode_index": mode, "callbacks": callbacks, "roots": roots,
            "ensure_helper": f"main:{review['ensure_helper']}", "prefix": f"battle:{review['prefix']}"}
