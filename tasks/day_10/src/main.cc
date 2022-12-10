#include "arg_to_vec.h"
#include "file_loader.h"

#include <__iterator/size.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace jabartek::aoc;

int task_1(const std::vector<std::string> &lines);
int task_2(const std::vector<std::string> &lines);

enum class CommandType {
  noop,
  addx,
};

struct Command {
  CommandType command{CommandType::noop};
  int value{0};
};

int delay(CommandType command_type);

Command toCommand(const std::string &line);

int strengthAt(const std::vector<int> &x, int at);
std::vector<int> xOverTime(std::vector<Command> &commands);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  const auto lines = file_loader.as<std::string>();

  std::cout << "Task 10.1:\t" << task_1(lines) << "\n";
  std::cout << "Task 10.2:\t" << task_2(lines) << "\n";
}

int task_1(const std::vector<std::string> &lines) {
  std::vector<Command> commands;
  std::ranges::transform(lines, std::back_inserter(commands), toCommand);
  const auto x = xOverTime(commands);
  return strengthAt(x, 20) + strengthAt(x, 60) + strengthAt(x, 100) +
         strengthAt(x, 140) + strengthAt(x, 180) + strengthAt(x, 220);
}

int task_2(const std::vector<std::string> &lines) {
  std::vector<Command> commands;
  std::ranges::transform(lines, std::back_inserter(commands), toCommand);
  const auto x = xOverTime(commands);
  std::cout << '\n';
  for (int line{0}; line < 6; ++line) {
    for (int column{0}; column < 40; ++column) {
      std::cout << (std::abs(x[line * 40 + column] - column) <= 1 ? '#' : ' ');
    }
    std::cout << '\n';
  }
  return 0;
}

Command toCommand(const std::string &line) {
  std::stringstream ss;
  ss << line;
  std::string command_str;
  ss >> command_str;
  if (command_str == "noop") {
    return Command{.command = CommandType::noop};
  }
  Command c{.command = CommandType::addx};
  ss >> c.value;
  return c;
}

int delay(CommandType command_type) {
  switch (command_type) {
  case CommandType::addx:
    return 2;
  case CommandType::noop:
    return 1;
  default:
    throw std::runtime_error("Wrong command type!");
  }
}

int strengthAt(const std::vector<int> &x, int at) { return at * x.at(at - 1); }

std::vector<int> xOverTime(std::vector<Command> &commands) {

  std::vector<int> x(1, 1);

  for (int i{0}; i < std::ssize(commands) && i < std::ssize(x); ++i) {
    const int last = x.empty() ? 1 : x.back();
    switch (commands[i].command) {
    case CommandType::addx:
      x.push_back(last);
      x.push_back(last + commands[i].value);
      break;
    case CommandType::noop:
      x.push_back(last);
      break;
    default:
      throw std::runtime_error("Wrong command type!");
    }
  }
  return x;
}