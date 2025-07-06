#! /usr/bin/env python3
from pathlib import Path
import subprocess
import sys

def main():
    pathlist = Path("./test").glob('**/*.txt')
    id = 1
    for path in pathlist:
        infile = str(path)
        print(f"[{id}] Testing {infile} ...")
        reffile = infile.rsplit('.', 1)[0] + '.ref'
        outfile = infile.rsplit('.', 1)[0] + '.out'

        # check=True: if sudoku fails to execute, an exception will be raised and
        # no more tests will run
        subprocess.run(f"./sudoku {infile} {outfile}", shell=True, check=True)

        ret = subprocess.run(f"diff {reffile} {outfile}", shell=True, check=False)
        if ret.returncode == 0:
            print("PASS")
        else:
            print("\nFAIL\n")

        id += 1



if __name__ == "__main__":
    sys.exit(main())