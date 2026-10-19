"""Perform one reviewed input/checkpoint step on independent emulator sessions."""
import argparse
import json
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
from runtime_debugger import Debugger, ROOT


def step(case, run_id, name, button, wait, duration, hold=None):
    client = Debugger(ROOT / f'build/ULUS10566/runtime/sessions/{case}/{run_id}')
    try:
        if button:
            if hold is None:
                client.request('input.buttons.press', button=button, duration=duration)
            else:
                client.hold_button(button, hold)
        time.sleep(wait)
        return {'case': case, 'capture': client.capture(name)}
    finally:
        client.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cases', nargs='+', choices=['retail', 'original', 'rebuilt'], required=True)
    parser.add_argument('--run-id', default='debugger-baseline-v1')
    parser.add_argument('--name', required=True)
    parser.add_argument('--button', choices=['cross','circle','triangle','square','up','down','left','right','start','select','ltrigger','rtrigger'])
    parser.add_argument('--wait', type=float, default=3)
    parser.add_argument('--duration', type=int, default=2)
    parser.add_argument('--hold', type=float, help='Use a held-button pulse in seconds instead of the frame-counted API')
    args = parser.parse_args()
    if not 0 <= args.wait <= 60 or not 1 <= args.duration <= 120 or len(args.cases) != len(set(args.cases)):
        parser.error('Invalid wait, duration, or duplicate case')
    if args.hold is not None and (not args.button or not .01 <= args.hold <= 2):
        parser.error('Hold requires a button and 0.01..2 seconds')
    session = (ROOT / f'build/ULUS10566/runtime/sessions/retail/{args.run_id}').resolve()
    session.relative_to(ROOT / 'build/ULUS10566/runtime/sessions/retail')
    with ThreadPoolExecutor(max_workers=len(args.cases)) as pool:
        futures = [pool.submit(step, case, args.run_id, args.name, args.button, args.wait, args.duration, args.hold) for case in args.cases]
        results = [future.result() for future in futures]
        print(json.dumps([{'case': r['case'], 'path': r['capture']['path']} for r in results]))
