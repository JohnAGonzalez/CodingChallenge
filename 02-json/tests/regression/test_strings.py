
#!/usr/bin/env python3
"""Regression tests for JSON number handling."""

import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[2]
EXECUTABLE = ROOT / "bin" / "json"


CASES = [
    ("ordinary string", '{"s":"hello"}', True),
    ("empty string", '{"s":""}', True),
    ("escaped quote", '{"s":"a\\"b"}', True),
    ("escaped backslash", '{"s":"a\\\\b"}', True),
    ("escaped slash", '{"s":"a\\/b"}', True),
    ("newline escape", '{"s":"a\\nb"}', True),
    ("tab escape", '{"s":"a\\tb"}', True),
    ("unicode escape", '{"s":"\\u0041"}', True),
    ("invalid escape", '{"s":"\\q"}', False),
    ("incomplete unicode", '{"s":"\\u123"}', False),
    ("invalid unicode hex", '{"s":"\\u12G4"}', False),
    ("unterminated string", '{"s":"hello}', False),
    ("raw newline", '{"s":"hello\nworld"}', False),
    ("raw tab", '{"s":"hello\tworld"}', False),
    ("unescaped quote", '{"s":"a"b"}', False),
]


class JsonStringTests(unittest.TestCase):

    def test_string_cases(self):
        self.assertTrue(
            EXECUTABLE.is_file(),
            f"Executable not found: {EXECUTABLE}"
        )

        with tempfile.TemporaryDirectory() as directory:
            for name, content, expected_valid in CASES:
                with self.subTest(name=name):
                    path = pathlib.Path(directory) / "input.json"
                    path.write_text(content, encoding="utf-8")

                    result = subprocess.run(
                        [str(EXECUTABLE), str(path)],
                        capture_output=True,
                        text=True,
                        timeout=5,
                    )

                    actual_valid = result.returncode == 0

                    self.assertEqual(
                        actual_valid,
                        expected_valid,
                        f"{name}: exit={result.returncode}\n"
                        f"{result.stdout}\n{result.stderr}",
                    )


if __name__ == "__main__":
    unittest.main(verbosity=2)
