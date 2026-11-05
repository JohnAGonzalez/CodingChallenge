# C++ JSON Parser — AI-Assisted Engineering and Cross-Platform CI

[![JSON Parser CI](https://github.com/JohnAGonzalez/CodingChallenge/actions/workflows/json-ci.yml/badge.svg?branch=feature%2Fai-assisted-json-review)](https://github.com/JohnAGonzalez/CodingChallenge/actions/workflows/json-ci.yml)

A C++ JSON parser project demonstrating table-driven parsing, systematic defect investigation, regression testing, and cross-platform continuous integration.

This repository is also a case study in using AI as an engineering assistant while retaining human responsibility for reviewing changes, validating results, and documenting limitations.

## Engineering Objectives

The project focuses on:

- Maintaining and improving an existing C++ codebase.
- Identifying defects through reproducible regression tests.
- Correcting JSON number and string tokenization behavior.
- Evaluating grammar coverage and identifying unsupported cases.
- Establishing reproducible builds using CMake.
- Running automated tests on macOS, Linux, and Windows.
- Documenting engineering decisions and remaining limitations.

## Architecture

The parser uses a table-driven, shift/reduce approach with lexical tokenization and parser state transitions.

The principal components include:

| Component | Responsibility |
|---|---|
| `JsonParser` | Tokenization and table-driven parsing |
| `InputProcessor` | Input processing |
| `CommandLine` | Command-line handling |
| `json.cpp` | Application entry point |
| `tests/regression/` | Automated regression tests |

The project uses the C++ standard library without requiring external C++ dependencies.

## Building the Project

### Requirements

- C++17-compatible compiler
- CMake 3.20 or newer
- Python 3 for regression testing

### Configure and Build

From the `02-json` directory:

```sh
cmake -S . -B build
cmake --build build --config Debug --parallel
```

The executable is generated under the CMake build directory. Its exact location depends on the platform and generator.

### Run

For a single-configuration build on macOS or Linux:

```sh
./build/bin/json tests/step4/valid.json
```

For a Visual Studio multi-configuration build, the executable may be under `build/bin/Debug/`.

The original Makefile remains available as an alternative build method.

## Automated Regression Testing

Run the registered regression suites:

```sh
ctest --test-dir build -C Debug --output-on-failure
```

The passing regression baseline includes:

| Area | Cases passing |
|---|---:|
| JSON number tokenization | 24/24 |
| JSON string tokenization | 15/15 |
| **Total** | **39/39** |

The numeric tests cover valid and invalid number forms, including fractional and exponent syntax.

The string tests cover escape handling, Unicode escape syntax, escaped quotation marks, and invalid unescaped control characters.

These cases are executed through two Python `unittest` suites registered with CTest.

## Continuous Integration

GitHub Actions builds the project and executes the registered regression suites on:

- Ubuntu Linux
- macOS
- Windows

The workflow uses CMake for compilation and CTest for automated verification.

The first cross-platform CI run completed successfully on all three operating systems.

Workflow definition: `.github/workflows/json-ci.yml` at the repository root.

## Grammar Evaluation and Known Limitations

A separate grammar regression suite evaluates additional JSON structures and nesting behavior.

Current baseline: **17 of 28 grammar cases pass**.

Known limitations include:

- Rejection of valid top-level JSON values other than objects.
- Incorrect handling of certain arrays containing multiple elements.
- Incomplete validation of Unicode encoding and surrogate-pair requirements.

The grammar suite is retained for investigation but is not currently part of the required passing CI checks.

**This parser should not be considered fully RFC 8259 compliant.**

Future work could include regenerating the parser tables from a formal grammar, expanding Unicode validation, and resolving the remaining grammar failures.

## AI-Assisted Engineering Methodology

AI assistance was used to help investigate parser behavior, propose focused changes, and develop regression tests.

The engineering workflow followed these principles:

1. Establish reproducible failing cases before making corrections.
2. Review proposed source changes before incorporating them.
3. Run regression tests after each correction.
4. Preserve and verify previously working behavior.
5. Document unresolved defects rather than presenting incomplete functionality as finished.
6. Validate the build and tests independently through GitHub Actions.

AI-generated suggestions were treated as proposals requiring engineering judgment and verification, not as authoritative implementations.

## Engineering Outcomes

This exercise demonstrates practical experience with:

- C++ source-level debugging and maintenance
- Lexical analysis and table-driven parsing
- Regression test design
- CMake and CTest
- Git feature-branch development
- GitHub Actions CI/CD
- Cross-platform build verification
- Technical documentation and defect assessment
- Responsible AI-assisted software engineering

## Project Status

The numeric and string regression suites pass across macOS, Linux, and Windows.

The remaining grammar limitations are documented and intentionally deferred as future work.
