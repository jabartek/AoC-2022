#include "arg_to_vec.h"
#include "file_loader.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <set>

using namespace jabartek::aoc;

int task_1(const std::string &text);
int task_2(const std::string &text);

int task(const std::string &text, int buffer_len);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  auto lines = file_loader.as<std::string>();

  const std::string text = std::move(lines.front());

  std::cout << "Task 6.1:\t" << task_1(text) << "\n";
  std::cout << "Task 6.2:\t" << task_2(text) << "\n";
}

int task(const std::string &text, int buffer_len) {
  std::deque<char> buffer;
  for (int i{0}; i < std::ssize(text); ++i) {
    const auto letter = text[i];
    if (buffer.size() == buffer_len) {
      buffer.pop_front();
    }
    buffer.push_back(letter);

    if (buffer.size() < buffer_len)
      continue;

    std::set<char> buffer_set(buffer.begin(), buffer.end());
    if (buffer_set.size() == buffer_len) {
      return i + 1;
    }
  }
  return -1;
}

int task_1(const std::string &text) { return task(text, 4); }

int task_2(const std::string &text) { return task(text, 14); }