
#!/usr/bin/env python3
"""Regression tests for JSON number handling."""

import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[2]
EXECUTABLE = ROOT / "bin" / "json"

CASES = [
    ("positive integer", '{"n":123}', True),
    ("negative integer", '{"n":-123}', True),
    ("decimal", '{"n":12.34}', True),
    ("negative decimal", '{"n":-0.25}', True),
    ("positive exponent", '{"n":1e10}', True),
    ("negative exponent", '{"n":1E-10}', True),
    ("zero", '{"n":0}', True),
    ("multiple decimal points", '{"n":1.2.3}', False),
    ("leading zero", '{"n":01}', False),
    ("missing digits", '{"n":-}', False),
    ("missing fraction digits", '{"n":1.}', False),
    ("missing exponent digits", '{"n":1e}', False),
    ("leading decimal point", '{"n":.5}', False),
    ("positive sign", '{"n":+1}', False),
    ("negative zero", '{"n":-0}', True),
    ("positive exponent sign", '{"n":1e+10}', True),
    ("zero exponent", '{"n":0e0}', True),
    ("fraction and exponent", '{"n":-12.34E+5}', True),
    ("multiple leading zeros", '{"n":000}', False),
    ("negative leading zero", '{"n":-01}', False),
    ("missing exponent after sign", '{"n":1e+}', False),
    ("multiple exponent markers", '{"n":1e2e3}', False),
    ("multiple negative signs", '{"n":--1}', False),
    ("embedded alphabetic suffix", '{"n":123abc}', False),
]


class JsonNumberTests(unittest.TestCase):

    def test_number_cases(self):
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

