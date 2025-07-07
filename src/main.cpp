#include <algorithm>
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class Sudoku
{
public:
  explicit Sudoku(const string &inputfile);

  bool solve();
  bool hasUniqueSolution();
  string level();
  void saveToFile(const string &outfile) const;
  void print() const;

private:
  bool isValid(int i, int j, char n) const;
  bool backtrack(const vector<pair<int, int>> &cells, size_t idx);
  void backtrack_all_solutions(int i, int j);
  void initCandidates();
  void updateCandidates(int i, int j, char n);

  vector<vector<char>> original_board_;
  vector<vector<char>> board_;
  // The possible values of each cell
  vector<vector<unordered_set<char>>> candidates_;
  int nof_solutions_ = -1;
  int steps = -1;
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
    original_board_.emplace_back(vector<char>(l.begin(), l.end()));
  }
  if (original_board_.size() != 9)
  {
    throw runtime_error(
        "Invalid board: each column must have exactly 9 characters.\n");
  }

  board_ = original_board_;

  initCandidates();
}

void Sudoku::initCandidates()
{
  candidates_ = vector<vector<unordered_set<char>>>(
      9,
      vector<unordered_set<char>>(
          9, unordered_set<char>{'1', '2', '3', '4', '5', '6', '7', '8', '9'}));

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      char n = original_board_[i][j];
      if (n != '.')
      {
        candidates_[i][j].clear();
        updateCandidates(i, j, n);
      }
    }
  }
}

// Remove 'n' from the possible values in the row, column, and 3*3 box
// containing cell (i, j)
void Sudoku::updateCandidates(int i, int j, char n)
{
  // row i
  for (int k = 0; k < 9; k++)
  {
    candidates_[i][k].erase(n);
  }
  // col j
  for (int k = 0; k < 9; k++)
  {
    candidates_[k][j].erase(n);
  }
  // 3*3 box
  int bi = i / 3 * 3;
  int bj = j / 3 * 3;
  for (int ki = bi; ki < bi + 3; ki++)
  {
    for (int kj = bj; kj < bj + 3; kj++)
    {
      candidates_[ki][kj].erase(n);
    }
  }
}

// Check if it's possible to put n at cell(i, j)
bool Sudoku::isValid(int i, int j, char n) const
{
  assert(n != '.');
  // row i
  for (int k = 0; k < 9; k++)
  {
    if (board_[i][k] == n)
      return false;
  }

  // col j
  for (int k = 0; k < 9; k++)
  {
    if (board_[k][j] == n)
      return false;
  }

  // 3*3 box
  int bi = i / 3 * 3;
  int bj = j / 3 * 3;
  for (int ki = bi; ki < bi + 3; ki++)
  {
    for (int kj = bj; kj < bj + 3; kj++)
    {
      if (board_[ki][kj] == n)
        return false;
    }
  }

  return true;
}

// backtracking: starts from the empty cell has least candidates
// 'cells' contains all empty cells, sorted by the size of candidates_[i][j]
// (i.e., possible values).
bool Sudoku::backtrack(const vector<pair<int, int>> &cells, size_t idx)
{
  steps++;
  if (idx >= cells.size())
  {
    return true;
  }

  const auto &[i, j] = cells[idx];

  for (char n : candidates_[i][j])
  {
    if (isValid(i, j, n))
    {
      board_[i][j] = n;
      if (backtrack(cells, idx + 1))
      {
        return true;
      }
      board_[i][j] = '.';
    }
  }
  return false;
}

// backtracking, brute-force
// search to find all possible solutions, and increment 'nof_solutions_' for
// each possible solutions found
void Sudoku::backtrack_all_solutions(int i, int j)
{
  if (j == 9)
  {
    i++;
    j = 0;
  }

  if (i == 9)
  {
    nof_solutions_++;
    return;
  }

  if (board_[i][j] != '.')
  {
    backtrack_all_solutions(i, j + 1);
  }

  for (char n : candidates_[i][j])
  {
    if (isValid(i, j, n))
    {
      board_[i][j] = n;
      backtrack_all_solutions(i, j + 1);
      board_[i][j] = '.';
    }
  }

  return;
}

void Sudoku::saveToFile(const string &outfile) const
{
  ofstream of(outfile);

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      of << board_[i][j];
    }
    if (i < 8) // except for the last line
    {
      of << '\n';
    }
  }
}

void Sudoku::print() const
{
  for (const auto &row : board_)
  {
    for (char cell : row)
    {
      cout << cell << " ";
    }
    cout << '\n';
  }
}

bool Sudoku::hasUniqueSolution()
{
  board_ = original_board_;

  nof_solutions_ = 0;
  backtrack_all_solutions(0, 0);
  if (nof_solutions_ != 1)
  {
    cout << nof_solutions_ << " solution(s)\n";
  }
  return nof_solutions_ == 1;
}

bool Sudoku::solve()
{
  board_ = original_board_;
  steps = 0;

  // Collect all empty cells and sort them by the number of possible candidates
  // (ascending)
  vector<pair<int, int>> cells;

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      if (board_[i][j] == '.')
      {
        cells.emplace_back(i, j);
      }
    }
  }
  sort(cells.begin(), cells.end(),
       [&](const auto &a, const auto &b)
       {
         return candidates_[a.first][a.second].size() <
                candidates_[b.first][b.second].size();
       });

  // Solve the sudoku starting from the cell with fewest possible values
  bool ret = backtrack(cells, 0);
  // cout << steps << '\n';
  return ret;
}

string Sudoku::level()
{
  // !!!dummy implementation
  this->solve();
  if (steps < 500)
    return "EAZY";
  else if (steps < 1000)
    return "MEDIUM";
  else if (steps < 5000)
    return "HARD";
  else
    return "SAMURAI";
}

// printHelp() is generated by AI
void printHelp()
{
  cout << "Usage:\n";
  cout << "  sudoku --solve <inputfile> [outputfile]   Solve the Sudoku "
          "puzzle.\n";
  cout << "  sudoku --validate <inputfile>             Check if the puzzle "
          "has a unique solution.\n";
  cout << "  sudoku --level <inputfile>                Get difficulty level.\n";
  cout << "\n";
  cout << "Arguments:\n";
  cout << "  <inputfile>   Path to the Sudoku puzzle input file.\n";
  cout << "  [outputfile]  Optional output file to save the solved puzzle.\n";
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    cerr << "Wrong number of inputs!\n";
    printHelp();
    return 1;
  }

  Sudoku ss(argv[2]);

  if (string(argv[1]) == "--solve")
  {
    if (!ss.solve())
    {
      cerr << "No solution!";
      return 1;
    }

    if (argc >= 4)
    {
      ss.saveToFile(argv[3]);
    }
    else
    {
      ss.print();
    }
  }
  else if (string(argv[1]) == "--validate")
  {
    if (ss.hasUniqueSolution())
    {
      cout << "Valid\n";
    }
    else
    {
      cout << "Invalid\n";
    }
  }
  else if (string(argv[1]) == "--level")
  {
    cout << ss.level();
  }

  return 0;
}
