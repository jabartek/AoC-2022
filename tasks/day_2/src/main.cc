#include "arg_to_vec.h"
#include "file_loader.h"
#include <__algorithm/ranges_for_each.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace jabartek::aoc;

enum class Symbol { Rock, Paper, Scissors };
enum class Result { Win, Loss, Draw };

Symbol symbolFromLetter(char letter);
int symbolScore(Symbol symbol);
int resultScore(Result result);
Result roundResult(Symbol theirs, Symbol yours);
int roundScore(Symbol theirs, Symbol yours);

int task_1(const std::vector<std::string> &lines);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  const auto lines = file_loader.as<std::string>();

  std::cout << "Task 2.1:\t" << task_1(lines) << "\n";
}

Symbol symbolFromLetter(char letter) {
  switch (letter) {
  case 'A':
  case 'X':
    return Symbol::Rock;
  case 'B':
  case 'Y':
    return Symbol::Paper;
  case 'C':
  case 'Z':
    return Symbol::Scissors;
  default:
    throw std::runtime_error("Invalid letter!");
  }
}
int symbolScore(Symbol symbol) {
  switch (symbol) {
  case Symbol::Rock:
    return 1;
  case Symbol::Paper:
    return 2;
  case Symbol::Scissors:
    return 3;
  default:
    throw std::runtime_error("Invalid symbol!");
  }
}
int resultScore(Result result) {
  switch (result) {
  case Result::Win:
    return 6;
  case Result::Draw:
    return 3;
  case Result::Loss:
    return 0;
  default:
    throw std::runtime_error("Invalid symbol!");
  }
}
Result roundResult(Symbol theirs, Symbol yours) {
  switch (yours) {
  case Symbol::Rock: {
    switch (theirs) {
    case Symbol::Rock:
      return Result::Draw;
    case Symbol::Paper:
      return Result::Loss;
    case Symbol::Scissors:
      return Result::Win;
    default:
      throw std::runtime_error("Invalid symbol!");
    }
  }
  case Symbol::Paper: {
    switch (theirs) {
    case Symbol::Rock:
      return Result::Win;
    case Symbol::Paper:
      return Result::Draw;
    case Symbol::Scissors:
      return Result::Loss;
    default:
      throw std::runtime_error("Invalid symbol!");
    }
  }
  case Symbol::Scissors: {
    switch (theirs) {
    case Symbol::Rock:
      return Result::Loss;
    case Symbol::Paper:
      return Result::Win;
    case Symbol::Scissors:
      return Result::Draw;
    default:
      throw std::runtime_error("Invalid symbol!");
    }
  }
  default:
    throw std::runtime_error("Invalid symbol!");
  }
}

int roundScore(Symbol theirs, Symbol yours) {
  return symbolScore(yours) + resultScore(roundResult(theirs, yours));
}

int roundScore(char theirs_c, char yours_c) {
  const auto theirs = symbolFromLetter(theirs_c);
  const auto yours = symbolFromLetter(yours_c);
  return symbolScore(yours) + resultScore(roundResult(theirs, yours));
}

int task_1(const std::vector<std::string> &lines) {
  int total{0};
  std::ranges::for_each(lines, [&total](const auto &line) {
    char theirs_c;
    char yours_c;
    std::stringstream ss;
    ss << line;
    ss >> theirs_c;
    ss >> yours_c;
    total += roundScore(theirs_c, yours_c);
  });
  return total;
}
