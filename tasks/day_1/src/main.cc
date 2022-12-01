#include "arg_to_vec.h"
#include "file_loader.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

using namespace jabartek::aoc;

std::vector<int> carriedSums(utilities::FileLoader &file_loader);
int task_1(const std::vector<int> &sums);
int task_2(std::vector<int> &sums);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  auto sums = carriedSums(file_loader);

  std::cout << "Task 1.1:\t" << task_1(sums) << "\n";
  std::cout << "Task 1.2:\t" << task_2(sums) << "\n";
}

std::vector<int> carriedSums(utilities::FileLoader &file_loader) {
  const auto data = file_loader.as_optional<int>();
  std::vector<int> sums;
  int local = 0;
  std::ranges::for_each(data, [&local, &sums](const auto &value) {
    if (!value && local != 0) {
      sums.push_back(local);
      local = 0;
      return;
    }
    local += *value;
  });
  if (local) {
    sums.push_back(local);
  }
  return sums;
}

int task_1(const std::vector<int> &sums) { return std::ranges::max(sums); }

int task_2(std::vector<int> &sums) {
  std::ranges::sort(sums);
  return std::accumulate(sums.end() - 3, sums.end(), 0);
}