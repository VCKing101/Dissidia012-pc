"""Record button input and paused framebuffer captures through PPSSPP's API."""
import argparse
import base64
import hashlib
import json
import re
import time
import uuid
import struct
from datetime import datetime, timezone
from pathlib import Path
from collections import deque

import websocket
import av

ROOT = Path(__file__).resolve().parents[1]


class SnapshotReader:
    """Expose a fixed readable extent even while the recorder appends data."""
    def __init__(self, path):
        self.name = str(path)
        self.stream = open(path, 'rb')
        self.limit = Path(path).stat().st_size

    def read(self, size=-1):
        remaining = max(0, self.limit - self.stream.tell())
        return self.stream.read(remaining if size < 0 else min(size, remaining))

    def seek(self, offset, whence=0):
        if whence == 2:
            offset, whence = self.limit + offset, 0
        return self.stream.seek(offset, whence)

    def tell(self):
        return self.stream.tell()

    def readable(self):
        return True

    def seekable(self):
        return True

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.stream.close()


class Debugger:
    def __init__(self, session):
        self.session = Path(session).resolve()
        self.session.relative_to(ROOT / 'build/ULUS10566/runtime/sessions')
        self.record = json.loads((self.session / 'session.json').read_text())
        if not self.record['debugger_enabled'] or not self.record['started'] or self.record['stopped']:
            raise ValueError('Expected a running protected session')
        self.evidence = self.session / 'evidence'
        self.evidence.mkdir(exist_ok=True)
        self.ws = websocket.create_connection(f"ws://[::1]:{self.record['configured_port']}/debugger", timeout=15, http_proxy_host=None, subprotocols=['debugger.ppsspp.org'])
        self.version = self.request('version', name='dissidia-runtime', version='1')
        if self.record['ppsspp_version'] != '1.20.4' or self.version.get('version') != 'v1.20.4':
            self.ws.close()
            raise ValueError('This client requires the pinned PPSSPP 1.20.4 build')
        self.request('broadcast.config.set', disallowed={'logger': True, 'game': True, 'input': True, 'stepping': False})
        self.game = self.request('game.status')
        if self.game.get('game', {}).get('id') != 'ULUS10566' or self.game.get('game', {}).get('version') != '1.00':
            self.ws.close()
            raise ValueError('Debugger attached to a different game')

    def log(self, direction, data):
        data = {k: v for k, v in data.items() if k not in ('uri', 'base64')}
        with (self.evidence / 'events.jsonl').open('a', encoding='utf-8') as stream:
            stream.write(json.dumps({'utc': datetime.now(timezone.utc).isoformat(), 'direction': direction, **data}) + '\n')

    def send(self, event, **parameters):
        ticket = uuid.uuid4().hex
        message = {'event': event, 'ticket': ticket, **parameters}
        self.log('request', message)
        self.ws.send(json.dumps(message))
        return ticket

    def request(self, event, **parameters):
        ticket = self.send(event, **parameters)
        deadline = time.monotonic() + 20
        while time.monotonic() < deadline:
            response = json.loads(self.ws.recv())
            self.log('response', response)
            if response.get('ticket') == ticket:
                if response.get('event') == 'error':
                    raise RuntimeError(response)
                return response
        raise TimeoutError(event)

    def stepping(self, enabled):
        if self.request('cpu.status')['stepping'] == enabled:
            return
        self.send('cpu.stepping' if enabled else 'cpu.resume')
        deadline = time.monotonic() + 10
        while time.monotonic() < deadline:
            if self.request('cpu.status')['stepping'] == enabled:
                return
            time.sleep(.05)
        raise TimeoutError('CPU stepping state')

    def capture(self, name):
        if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_-]{0,80}', name):
            raise ValueError('Invalid capture name')
        output = self.evidence / f'{name}.png'
        if output.exists():
            raise ValueError('Capture already exists; use a new name')
        was_stepping = self.request('cpu.status')['stepping']
        self.stepping(True)
        try:
            method = 'gpu.buffer.screenshot'
            try:
                response = self.request(method, type='uri')
            except RuntimeError as error:
                if 'Could not download output' not in str(error):
                    raise
                method = 'gpu.buffer.renderColor'
                try:
                    response = self.request(method, type='uri')
                except RuntimeError as render_error:
                    if 'Could not download output' not in str(render_error):
                        raise
                    return self.capture_video(name)
            prefix = 'data:image/png;base64,'
            if not response['uri'].startswith(prefix):
                raise ValueError('Unexpected screenshot format')
            payload = base64.b64decode(response['uri'][len(prefix):], validate=True)
            if not payload.startswith(b'\x89PNG\r\n\x1a\n'):
                raise ValueError('Not a PNG')
            output.write_bytes(payload)
            result = {'name': name, 'path': output.relative_to(ROOT).as_posix(),
                      'sha256': hashlib.sha256(payload).hexdigest(),
                      'width': response['width'], 'height': response['height'], 'method': method}
            self.log('capture', result)
            return result
        finally:
            if not was_stepping:
                self.stepping(False)

    def close(self):
        self.ws.close()

    def hold_button(self, button, seconds):
        if not 0.01 <= seconds <= 2:
            raise ValueError('Held-button duration must be 0.01..2 seconds')
        self.request('input.buttons.send', buttons={button: True})
        try:
            time.sleep(seconds)
        finally:
            self.request('input.buttons.send', buttons={button: False})

    def hold_analog(self, x, y, seconds, stick='left'):
        """Send bounded PSP stick input and return it to neutral on interruption."""
        if not (-1 <= x <= 1 and -1 <= y <= 1 and .01 <= seconds <= 2):
            raise ValueError('Analog coordinates require -1..1 and duration .01..2 seconds')
        if stick not in ('left', 'right'):
            raise ValueError('Analog stick must be left or right')
        try:
            self.request('input.analog.send', x=x, y=y, stick=stick)
            time.sleep(seconds)
        finally:
            self.request('input.analog.send', x=0, y=0, stick=stick)

    def capture_video(self, name):
        # AVI capture runs on the emulator's render thread. Its last complete
        # packet can precede the pause slightly; this is not an exact CPU-frame
        # snapshot. Decode from the last keyframe rather than the entire video.
        videos = sorted((Path(self.record['memstick']) / 'PSP/VIDEO').glob('*.avi'), key=lambda p: p.stat().st_mtime_ns)
        if not videos:
            raise ValueError('No local video for screenshot fallback')
        with SnapshotReader(videos[-1]) as snapshot:
            # Read the small AVI codec header and a bounded tail directly.
            # Generic demux can scan gigabytes when a live AVI has no final index.
            header = snapshot.read(1024 * 1024)
            if header[:4] != b'RIFF' or header[8:12] != b'AVI ':
                raise ValueError('Expected a PPSSPP AVI recording')
            pos = header.find(b'strf')
            if pos < 0 or pos + 48 > len(header):
                raise ValueError('Missing AVI bitmap codec header')
            length = struct.unpack_from('<I', header, pos + 4)[0]
            info = header[pos + 8:pos + 8 + length]
            if length < 40 or length > 4096 or len(info) != length or info[16:20] != b'FFV1':
                raise ValueError('Fallback requires the configured lossless FFV1 recorder')
            width, height = struct.unpack_from('<ii', info, 4)
            if not 1 <= width <= 4096 or not 1 <= abs(height) <= 4096:
                raise ValueError('Unexpected video dimensions')
            def new_decoder():
                decoder = av.CodecContext.create('ffv1', 'r')
                decoder.width, decoder.height = width, abs(height)
                decoder.extradata = info[40:]
                return decoder
            tail_size = min(snapshot.limit, 64 * 1024 * 1024)
            snapshot.seek(-tail_size, 2)
            tail = snapshot.read(tail_size)
            group = deque(maxlen=64)
            offset = 0
            while True:
                offset = tail.find(b'00dc', offset)
                if offset < 0 or offset + 8 > len(tail):
                    break
                size = struct.unpack_from('<I', tail, offset + 4)[0]
                if 0 < size <= 8 * 1024 * 1024 and offset + 8 + size <= len(tail):
                    group.append((snapshot.limit - tail_size + offset, tail[offset + 8:offset + 8 + size]))
                    offset += 8 + size + (size & 1)
                else:
                    offset += 4
            last = None
            frame_offset = None
            packets = list(group)
            for start in range(len(packets) - 1, -1, -1):
                decoder = new_decoder()
                try:
                    initial = decoder.decode(av.Packet(packets[start][1]))
                except av.error.InvalidDataError:
                    continue
                if not initial or not initial[-1].key_frame:
                    continue
                last, frame_offset = initial[-1], packets[start][0]
                for byte_offset, payload in packets[start + 1:]:
                    try:
                        for frame in decoder.decode(av.Packet(payload)):
                            last, frame_offset = frame, byte_offset
                    except av.error.InvalidDataError:
                        break
                break
            if last is None:
                raise ValueError('No complete frame in live recording')
            output = self.evidence / f'{name}.png'
            last.to_image().save(output)
            result = {'name': name, 'path': output.relative_to(ROOT).as_posix(),
                      'sha256': hashlib.sha256(output.read_bytes()).hexdigest(),
                      'width': last.width, 'height': last.height,
                      'method': 'recorded-video-last-complete-frame',
                      'video': videos[-1].relative_to(ROOT).as_posix(), 'encoded_time': None,
                      'snapshot_bytes': snapshot.limit, 'video_chunk_offset': frame_offset}
            self.log('capture', result)
            return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('session', type=Path)
    parser.add_argument('--name', required=True)
    parser.add_argument('--button', choices=['cross', 'circle', 'triangle', 'square', 'up', 'down', 'left', 'right', 'start', 'select', 'ltrigger', 'rtrigger'])
    parser.add_argument('--duration', type=int, default=2)
    parser.add_argument('--wait', type=float, default=1)
    args = parser.parse_args()
    if not 0 <= args.wait <= 60 or not 1 <= args.duration <= 120:
        parser.error('Wait must be 0..60 seconds and duration 1..120 frames')
    client = Debugger(args.session)
    try:
        if args.button:
            client.request('input.buttons.press', button=args.button, duration=args.duration)
        time.sleep(args.wait)
        print(json.dumps({'game': client.game, 'capture': client.capture(args.name)}, indent=2))
    finally:
        client.close()
