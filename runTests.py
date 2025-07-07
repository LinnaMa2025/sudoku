#! /usr/bin/env python3
from pathlib import Path
import subprocess
import sys

def test_solve_sudoku():
    print("\nTesting solving sudoku\n")
    pathlist = Path("./test/unique").glob('**/*.txt')
    id = 1
    for path in pathlist:
        infile = str(path)
        print(f"[{id}] Solving {infile} ...")
        reffile = infile.rsplit('.', 1)[0] + '.ref'
        outfile = infile.rsplit('.', 1)[0] + '.out'

        # check=True: if sudoku fails to execute, an exception will be raised and
        # no more tests will run
        subprocess.run(f"./sudoku --solve {infile} {outfile}", shell=True, check=True)

        ret = subprocess.run(f"diff {reffile} {outfile}", shell=True, check=False)
        if ret.returncode == 0:
            print("PASS")
        else:
            print("\nFAIL\n")

        id += 1

def test_validate_sudoku():
    infiles = ["./test/unique/hard1.txt", "./test/invalid/no_solution1.txt", "./test/invalid/two_solutions1.txt"]
    answers = ["Valid", "Invalid", "Invalid"]

    print("\nTesting validating sudoku\n")

    for id in range(len(infiles)):
        print(f"[{id}] Validating {infiles[id]} ...")
        ret = subprocess.run(f"./sudoku --validate {infiles[id]}", shell=True, check=True, capture_output=True)
        if answers[id] in str(ret.stdout):
            print("PASS")
        else:
            print(str(ret.stdout))
            print("FAIL")


def main():
    test_solve_sudoku()
    test_validate_sudoku()



if __name__ == "__main__":
    sys.exit(main())