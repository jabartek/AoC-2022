#include "arg_to_vec.h"
#include "file_loader.h"

#include <iostream>
#include <string>

using namespace jabartek::aoc;

int task_1(const std::vector<std::string> &lines);
int task_2(const std::vector<std::string> &lines);

using Range = std::pair<int, int>;

std::pair<Range, Range> getRanges(const std::string &line);
Range getRange(std::string_view range_text);

bool doFullyOverlap(const Range &range_1, const Range &range_2);
bool doPartiallyOverlap(const Range &range_1, const Range &range_2);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  const auto lines = file_loader.as<std::string>();

  std::cout << "Task 4.1:\t" << task_1(lines) << "\n";
  std::cout << "Task 4.2:\t" << task_2(lines) << "\n";
}

int task_1(const std::vector<std::string> &lines) {
  int result{0};
  for (const auto &line : lines) {
    const auto [left, right] = getRanges(line);
    result += doFullyOverlap(left, right);
  }
  return result;
}

int task_2(const std::vector<std::string> &lines) {
  int result{0};
  for (const auto &line : lines) {
    const auto [left, right] = getRanges(line);
    result += doPartiallyOverlap(left, right);
  }
  return result;
}
std::pair<Range, Range> getRanges(const std::string &line) {
  const auto comma_it = std::find(line.begin(), line.end(), ',');
  const std::string_view left{line.begin(), comma_it};
  const std::string_view right{comma_it + 1, line.end()};
  return {getRange(left), getRange(right)};
}

Range getRange(std::string_view range_text) {
  const auto hyphen_it = std::find(range_text.begin(), range_text.end(), '-');

  const std::string left{range_text.begin(), hyphen_it};
  const std::string right{hyphen_it + 1, range_text.end()};

  return Range{std::stoi(left), std::stoi(right)};
}

bool doFullyOverlap(const Range &range_1, const Range &range_2) {
  constexpr auto firstInSecond = [](const Range &first, const Range &second) {
    return first.first >= second.first && first.second <= second.second;
  };
  return firstInSecond(range_1, range_2) || firstInSecond(range_2, range_1);
}

bool doPartiallyOverlap(const Range &range_1, const Range &range_2) {
  return range_1.first <= range_2.second && range_2.first < = range_1.second;
}