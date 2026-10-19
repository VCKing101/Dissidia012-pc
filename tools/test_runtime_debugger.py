import tempfile
import unittest
import importlib.util
from pathlib import Path
from unittest.mock import Mock

ROOT = Path(__file__).resolve().parents[1]
RUNTIME_AVAILABLE = all(importlib.util.find_spec(name) for name in ('av', 'PIL', 'websocket'))
if RUNTIME_AVAILABLE:
    import av
    from PIL import Image
    from runtime_debugger import Debugger, SnapshotReader


@unittest.skipUnless(RUNTIME_AVAILABLE, 'Install requirements-runtime.txt for debugger/video checks')
class RuntimeDebuggerChecks(unittest.TestCase):
    def test_snapshot_reader_does_not_follow_appended_bytes(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'growing.avi'
            path.write_bytes(b'original')
            with SnapshotReader(path) as reader:
                with path.open('ab') as writer:
                    writer.write(b' appended')
                self.assertEqual(reader.read(), b'original')
                self.assertEqual(reader.read(), b'')
                self.assertEqual(reader.seek(0, 2), len(b'original'))
                reader.seek(-4, 2)
                self.assertEqual(reader.read(), b'inal')
    def test_failed_readback_resumes_previously_running_cpu(self):
        client = Debugger.__new__(Debugger)
        client.evidence = ROOT / 'build/ULUS10566/runtime/nonexistent-test-evidence'
        client.request = Mock(side_effect=[{'stepping': False}, RuntimeError('unexpected readback failure')])
        client.stepping = Mock()
        with self.assertRaisesRegex(RuntimeError, 'unexpected readback'):
            client.capture('failed-readback')
        self.assertEqual([call.args[0] for call in client.stepping.call_args_list], [True, False])

    def test_failed_readback_preserves_previously_paused_cpu(self):
        client = Debugger.__new__(Debugger)
        client.evidence = ROOT / 'build/ULUS10566/runtime/nonexistent-test-evidence'
        client.request = Mock(side_effect=[{'stepping': True}, RuntimeError('unexpected readback failure')])
        client.stepping = Mock()
        with self.assertRaisesRegex(RuntimeError, 'unexpected readback'):
            client.capture('failed-readback')
        self.assertEqual([call.args[0] for call in client.stepping.call_args_list], [True])

    def test_held_button_is_released_if_wait_is_interrupted(self):
        client = Debugger.__new__(Debugger)
        client.request = Mock()
        from unittest.mock import patch
        with patch('runtime_debugger.time.sleep', side_effect=KeyboardInterrupt):
            with self.assertRaises(KeyboardInterrupt):
                client.hold_button('cross', .2)
        self.assertEqual(client.request.call_args_list[0].kwargs['buttons'], {'cross': True})
        self.assertEqual(client.request.call_args_list[-1].kwargs['buttons'], {'cross': False})

    def test_video_fallback_rejects_non_avi_input(self):
        parent = ROOT / 'build/ULUS10566/runtime/sessions'
        parent.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=parent) as directory:
            client = Debugger.__new__(Debugger)
            client.session = Path(directory)
            client.record = {'memstick': str(client.session / 'memstick')}
            client.evidence = client.session / 'evidence'
            client.evidence.mkdir()
            video_root = Path(client.record['memstick']) / 'PSP/VIDEO'
            video_root.mkdir(parents=True)
            (video_root / 'bad.avi').write_bytes(b'not an AVI')
            with self.assertRaisesRegex(ValueError, 'Expected a PPSSPP AVI'):
                client.capture_video('bad-frame')

    def test_analog_returns_to_neutral_if_wait_is_interrupted(self):
        client = Debugger.__new__(Debugger)
        client.request = Mock()
        from unittest.mock import patch
        with patch('runtime_debugger.time.sleep', side_effect=KeyboardInterrupt):
            with self.assertRaises(KeyboardInterrupt):
                client.hold_analog(.5, -1, .2)
        self.assertEqual(client.request.call_args_list[0].kwargs,
                         {'x': .5, 'y': -1, 'stick': 'left'})
        self.assertEqual(client.request.call_args_list[-1].kwargs,
                         {'x': 0, 'y': 0, 'stick': 'left'})

    def test_invalid_analog_parameters_send_no_input(self):
        client = Debugger.__new__(Debugger)
        client.request = Mock()
        for args in [(float('nan'), 0, .2), (0, -2, .2),
                     (0, 0, float('inf')), (0, 0, .2, 'invalid')]:
            with self.subTest(args=args), self.assertRaises(ValueError):
                client.hold_analog(*args)
        client.request.assert_not_called()

    def test_video_fallback_exports_final_lossless_frame(self):
        parent = ROOT / 'build/ULUS10566/runtime/sessions'
        parent.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=parent) as directory:
            client = Debugger.__new__(Debugger)
            client.session = Path(directory)
            client.record = {'memstick': str(client.session / 'memstick')}
            client.evidence = client.session / 'evidence'
            client.evidence.mkdir()
            client.log = Mock()
            video_root = Path(client.record['memstick']) / 'PSP/VIDEO'
            video_root.mkdir(parents=True)
            with av.open(str(video_root / 'test.avi'), 'w') as container:
                stream = container.add_stream('ffv1', rate=30)
                stream.width = stream.height = 8
                stream.pix_fmt = 'bgr0'
                stream.codec_context.gop_size = 12
                # More than the bounded packet tail, including partial GOPs.
                for index in range(100):
                    expected = Image.new('RGB', (8, 8))
                    expected.putdata([((pixel * 3 + index) % 256, (pixel * 7 + index) % 256, (pixel * 11 + index) % 256) for pixel in range(64)])
                    frame = av.VideoFrame.from_image(expected)
                    for packet in stream.encode(frame):
                        container.mux(packet)
                for packet in stream.encode():
                    container.mux(packet)
            result = client.capture_video('last-frame')
            self.assertEqual(result['method'], 'recorded-video-last-complete-frame')
            with Image.open(ROOT / result['path']) as exported:
                self.assertEqual(exported.convert('RGB').tobytes(), expected.tobytes())


if __name__ == '__main__':
    unittest.main()
