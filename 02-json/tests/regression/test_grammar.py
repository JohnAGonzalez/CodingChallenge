
#!/usr/bin/env python3
"""Regression tests for JSON grammar and Unicode handling."""

import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[2]
EXECUTABLE = ROOT / "bin" / "json"

CASES = [
    # Top-level JSON values
    ("top-level object", '{"a":1}', True),
    ("top-level array", '[1,2,3]', True),
    ("top-level string", '"hello"', True),
    ("top-level number", '42', True),
    ("top-level true", 'true', True),
    ("top-level false", 'false', True),
    ("top-level null", 'null', True),

    # Empty and nested structures
    ("empty object", '{}', True),
    ("empty array", '[]', True),
    ("nested object", '{"a":{"b":{"c":1}}}', True),
    ("nested arrays", '[[1,2],[3,4]]', True),
    ("mixed nesting", '{"a":[1,{"b":[true,null]}]}', True),

    # Invalid structure
    ("trailing object comma", '{"a":1,}', False),
    ("trailing array comma", '[1,2,]', False),
    ("missing colon", '{"a" 1}', False),
    ("missing array comma", '[1 2]', False),
    ("missing object comma", '{"a":1 "b":2}', False),
    ("extra top-level value", '{"a":1} {"b":2}', False),
    ("trailing garbage", '{"a":1}xyz', False),
    ("empty input", '', False),

    # Unicode escapes
    ("unicode BMP escape", '{"s":"\\u0041"}', True),
    ("unicode surrogate pair", '{"s":"\\uD83D\\uDE00"}', True),

    # Raw UTF-8
    ("UTF-8 accented text", '{"s":"café"}', True),
    ("UTF-8 emoji", '{"s":"😀"}', True),

    # Arrays
    ("single-element array in object", '{"a":[1]}', True),
    ("two-element array in object", '{"a":[1,2]}', True),
    ("nested single-element array", '{"a":[[1]]}', True),
    ("nested multi-element array", '{"a":[[1,2]]}', True),
]


class JsonGrammarTests(unittest.TestCase):

    def test_grammar_cases(self):
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
