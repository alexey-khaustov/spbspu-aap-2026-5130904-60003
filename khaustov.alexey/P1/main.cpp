#include <iostream>

namespace khaustov {

  int checkLocalMax(const int num1, const int num2, const int num3)
  {
    return num2 > num1 && num2 > num3;
  }

  int checkDivision(const int num1, const int num2)
  {
    return (num2 % num1) == 0;
  }

  int run()
  {
    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    unsigned long long numbers_count = 0;
    unsigned long long local_max = 0;
    unsigned long long divisible_count = 0;
    bool terminated = false;
    int current_number = 0;

    const unsigned long long first_element = 1;
    const unsigned long long second_element = 2;
    const unsigned long long third_element = 3;

    while (std::cin >> current_number) {
      if (current_number == 0) {
        terminated = true;
        break;
      }

      numbers_count += 1;

      switch (numbers_count) {
      case first_element:
        num1 = current_number;
        break;
      case second_element:
        num2 = current_number;
        divisible_count += checkDivision(num1, num2);
        break;
      case third_element:
        num3 = current_number;
        local_max += checkLocalMax(num1, num2, num3);
        divisible_count += checkDivision(num2, num3);
        break;
      default:
        num1 = num2;
        num2 = num3;
        num3 = current_number;
        local_max += checkLocalMax(num1, num2, num3);
        divisible_count += checkDivision(num2, num3);
        break;
      }
    }

    if (!terminated) {
      std::cerr << "Error: invalid input\n";
      const int invalid_input = 1;
      return invalid_input;
    }

    const int border1 = 0;
    if (numbers_count > border1) {
      std::cout << local_max << "\n";
    } else {
      std::cerr << "Error: LOC-MAX cannot be calculated\n";
    }

    const int border2 = 1;
    if (numbers_count > border2) {
      std::cout << divisible_count << "\n";
    } else {
      std::cerr << "Error: DIV-REM cannot be calculated\n";
    }

    if (numbers_count == border2 || numbers_count == border1) {
      const int calculation_error = 2;
      return calculation_error;
    }

    return 0;
  }
}

int main()
{
  return khaustov::run();
}
