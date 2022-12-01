#include "arg_to_vec.h"
#include "file_loader.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

using namespace jabartek::aoc;

int task_1(utilities::FileLoader &file_loader);
int task_2(utilities::FileLoader &file_loader);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);

  std::cout << "Task 1.1:\t" << task_1(file_loader) << "\n";
  std::cout << "Task 1.2:\t" << task_2(file_loader) << "\n";
}

int task_1(utilities::FileLoader &file_loader) {
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
  return std::ranges::max(sums);
}

int task_2(utilities::FileLoader &file_loader) {
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
  std::ranges::sort(sums);
  return std::accumulate(sums.end() - 3, sums.end(), 0);
}