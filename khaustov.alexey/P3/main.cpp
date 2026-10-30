#include <iostream>
#include <string>
#include <fstream>
#include <climits>
#include <stdexcept>
#include <new>

const int MAX_SIZE = 100;

bool checkParameters(int argc, char* argv[])
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return false;
  }
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return false;
  }

  const std::string first_parameter = argv[1];
  if (first_parameter.length() != 1) {
    std::cerr << "First parameter must be 1 or 2\n";
    return false;
  }

  if (first_parameter != "1" && first_parameter != "2") {
    std::cerr << "First parameter must be 1 or 2\n";
    return false;
  }
  return true;
}

bool checkFile(std::ifstream& file)
{
  if (!file) {
    std::cerr << "ERROR: cannot open file\n";
    return false;
  }
  if (!file.is_open()) {
    std::cerr << "ERROR: cannot open file\n";
    return false;
  }
  return true;
}

bool checkFile(std::ofstream& file)
{
  if (!file) {
    std::cerr << "ERROR: cannot open file\n";
    return false;
  }
  if (!file.is_open()) {
    std::cerr << "ERROR: cannot open file\n";
    return false;
  }
  return true;
}

int min(int rows, int cols)
{
  if (rows > cols) {
    return cols;
  } else {
    return rows;
  }
}

int max(int rows, int cols)
{
  if (rows < cols) {
    return cols;
  } else {
    return rows;
  }
}

long long max(long long num1, long long num2)
{
  if (num1 < num2) {
    return num2;
  } else {
    return num1;
  }
}

void assembleArrayOnTheStack(std::ifstream& input_file, int matrix[][MAX_SIZE], int& rows, int& cols)
{
  if (!(input_file >> rows) || !(input_file >> cols)) {
    throw std::invalid_argument("Invalid argument\n");
  }

  if (rows > MAX_SIZE || cols > MAX_SIZE || rows < 0 || cols < 0) {
    throw std::out_of_range("Dimensions exceed MAX_SIZE\n");
  }

  int number = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (!(input_file >> number)) {
        throw std::invalid_argument("Invalid argument\n");
      }
      matrix[i][j] = number;
    }
  }
  std::string extra;
  if (input_file >> extra) {
    throw std::invalid_argument("Too many elements\n");
  }
}

void freeMatrix(int** matrix, int rows)
{
  for (int i = 0; i < rows; i++) {
    delete[] matrix[i];
  }
  delete[] matrix;
}

int** createMatrix(const int rows, const int cols)
{
  int** const matrix = new int*[rows]();

  try {
    for (int i = 0; i < rows; i++) {
      matrix[i] = new int[cols]();
    }
  } catch (...) {
    freeMatrix(matrix, rows);
    throw;
  }

  return matrix;
}

int** assembleArrayInTheFreeStore(std::ifstream& input_file, int& rows, int& cols)
{
  if (!(input_file >> rows) || !(input_file >> cols)) {
    throw std::invalid_argument("Invalid argument\n");
  }

  if (rows > MAX_SIZE || cols > MAX_SIZE || rows < 0 || cols < 0) {
    throw std::out_of_range("Dimensions exceed MAX_SIZE\n");
  }

  int** const matrix = createMatrix(rows, cols);

  int number = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (!(input_file >> number)) {
        freeMatrix(matrix, rows);
        throw std::invalid_argument("Invalid argument\n");
      }
      matrix[i][j] = number;
    }
  }
  std::string extra;
  if (input_file >> extra) {
    freeMatrix(matrix, rows);
    throw std::invalid_argument("Too many elements\n");
  }
  return matrix;
}

void maxSumSdg(int matrix[][MAX_SIZE], const int rows, const int cols, long long& max_sum)
{
  const int size = min(cols, rows);
  if (size == 0) {
    max_sum = 0;
    return;
  }

  if (size == 1) {
    max_sum = matrix[0][0];
    return;
  }

  for (int j = 1; j < size; j++) {
    long long sum = 0;
    for (int i = 0, k = j; i < size && k < size; i++, k++) {
      sum += matrix[i][k];
    }
    max_sum = max(max_sum, sum);
  }

  for (int i = 1; i < size; i++) {
    long long sum = 0;
    for (int k = i, j = 0; k < size && j < size; k++, j++) {
      sum += matrix[k][j];
    }
    max_sum = max(max_sum, sum);
  }
}

void maxSumSdg(int** matrix, const int rows, const int cols, long long& max_sum)
{
  const int size = min(cols, rows);

  if (size == 0) {
    max_sum = 0;
    return;
  }

  if (size == 1) {
    max_sum = matrix[0][0];
    return;
  }

  for (int j = 1; j < size; j++) {
    long long sum = 0;
    for (int i = 0, k = j; i < size && k < size; i++, k++) {
      sum += matrix[i][k];
    }
    max_sum = max(max_sum, sum);
  }

  for (int i = 1; i < size; i++) {
    long long sum = 0;
    for (int k = i, j = 0; k < size && j < size; k++, j++) {
      sum += matrix[k][j];
    }
    max_sum = max(max_sum, sum);
  }
}

int main(int argc, char* argv[])
{
  const bool check = checkParameters(argc, argv);
  if (!check) {
    return 1;
  }

  std::ifstream input_file(argv[2]);
  const bool check_input = checkFile(input_file);
  if (!check_input) {
    return 2;
  }

  if (input_file.peek() == std::ifstream::traits_type::eof()) {
    std::cerr << "ERROR: input file is empty\n";
    return 2;
  }

  int rows = 0, cols = 0;
  long long max_sum = LLONG_MIN;
  try {
    if (std::string(argv[1]) == "1") {
      int matrix[MAX_SIZE][MAX_SIZE] = {};
      assembleArrayOnTheStack(input_file, matrix, rows, cols);
      maxSumSdg(matrix, rows, cols, max_sum);
    } else if (std::string(argv[1]) == "2") {
      int** const matrix = assembleArrayInTheFreeStore(input_file, rows, cols);
      maxSumSdg(matrix, rows, cols, max_sum);
      freeMatrix(matrix, rows);
    }
  } catch (const std::bad_alloc& e) {
    std::cerr << "Error: memory allocation failed\n";
    return 2;
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 2;
  } catch (const std::out_of_range& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 2;
  }

  std::ofstream output_file(argv[3]);
  const bool check_output = checkFile(output_file);
  if (!check_output) {
    return 2;
  }

  output_file << max_sum << "\n";

  input_file.close();
  output_file.close();
  return 0;
}
