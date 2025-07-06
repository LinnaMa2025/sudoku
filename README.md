# How to build?

```bash
cd sudoku
make # this will produce the executable file `sudoku`
```

# How to run?

```bash
cd sudoku
./sudoku test/simple1.txt /tmp/simple1.out
```

If no output file is specified, the solution will be printed to standard output.

# How to test?

```bash
cd sudoku
./runTests.py
```

# Folder structure

src/

C++ source files

test/

sudoku test cases. Each test case contains two files:

- \*.txt: the input Sudoku puzzle
- \*.ref: the unique solution, i.e., the reference file of the test case
  Each file is a plain text file, that only contains character '1'-'9', '.' and newlines.
  Each character represents an cell in the Sudoku, and a dot '.' means the cell is empty and should be filled.
