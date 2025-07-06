#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Sudoku
{
public:
  explicit Sudoku(const string &inputfile);

  void solve() { backtrack(0, 0); }
  void saveToFile(const string &outfile) const;
  void print() const;

private:
  bool isValid(int i, int j, char n) const;
  bool backtrack(int i, int j);
  vector<vector<char>> board;
};

Sudoku::Sudoku(const string &inputfile)
{
  ifstream input_file(inputfile);
  if (!input_file)
  {
    throw runtime_error("Failed to open input file.");
  }

  string l;
  while (getline(input_file, l))
  {
    if (l.size() != 9)
    {
      throw runtime_error(
          "Invalid board: each row must have exactly 9 characters.\n");
    }
    board.emplace_back(vector<char>(l.begin(), l.end()));
  }
  if (board.size() != 9)
  {
    throw runtime_error(
        "Invalid board: each column must have exactly 9 characters.\n");
  }
}

bool Sudoku::isValid(int i, int j, char n) const
{
  assert(n != '.');
  // row i
  for (int k = 0; k < 9; k++)
  {
    if (board[i][k] == n)
      return false;
  }

  // col j
  for (int k = 0; k < 9; k++)
  {
    if (board[k][j] == n)
      return false;
  }

  // box
  int bi = i / 3 * 3;
  int bj = j / 3 * 3;
  for (int ki = bi; ki < bi + 3; ki++)
  {
    for (int kj = bj; kj < bj + 3; kj++)
    {
      if (board[ki][kj] == n)
        return false;
    }
  }

  return true;
}

// backtrack, brute-force
bool Sudoku::backtrack(int i, int j)
{
  if (j == 9)
  {
    i++;
    j = 0;
  }

  if (i == 9)
  {
    return true;
  }

  if (board[i][j] != '.')
  {
    return backtrack(i, j + 1);
  }

  for (char n = '1'; n <= '9'; n++)
  {
    if (isValid(i, j, n))
    {
      board[i][j] = n;
      if (backtrack(i, j + 1))
      {
        return true;
      }
      board[i][j] = '.';
    }
  }
  return false;
}

void Sudoku::saveToFile(const string &outfile) const
{
  ofstream of(outfile);

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      of << board[i][j];
    }
    if (i < 8) // except for the last line
    {
      of << '\n';
    }
  }
}

void Sudoku::print() const
{
  for (const auto &row : board)
  {
    for (char ele : row)
    {
      cout << ele << " ";
    }
    cout << '\n';
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    cerr << "Wrong number of inputs!";
    return 1;
  }

  Sudoku ss(argv[1]);
  ss.solve();

  if (argc >= 3)
  {
    ss.saveToFile(argv[2]);
  }
  else
  {
    ss.print();
  }

  return 0;
}
