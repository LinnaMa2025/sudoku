# How to build?

```bash
make # this will produce the executable file `sudoku`
```

# How to run?

**Usage:**

sudoku --solve <inputfile> [outputfile] Solve the Sudoku puzzle.

sudoku --validate <inputfile> Check if the puzzle has a unique solution.

sudoku --level <inputfile> Get difficulty level.

**Arguments:**

<inputfile> Path to the Sudoku puzzle input file.

[outputfile] Optional output file to save the solved puzzle.

**Example:**

```bash
./sudoku --solve test/simple1.txt /tmp/simple1.out
```

If no output file is specified, the solution will be printed to standard output.

# How to test?

```bash
./runTests.py
```

# Folder structure

**src/**

C++ source files

**test/unique**

sudoku test cases. Each test case contains two files:

- \*.txt: the input Sudoku puzzle
- \*.ref: the unique solution, i.e., the reference file of the test case
  Each file is a plain text file, that only contains character '1'-'9', '.' and newlines.
  Each character represents an cell in the Sudoku, and a dot '.' means the cell is empty and should be filled.

**test/invalid**

invalid sudoku puzzels, which may contain zero or more than one solutions.
