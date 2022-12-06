#include "arg_to_vec.h"
#include "file_loader.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <span>
#include <sstream>
#include <string>

using namespace jabartek::aoc;

std::string task_1(const std::vector<std::string> &input);
std::string task_2(const std::vector<std::string> &lines);

using Stack = std::deque<char>;
using Stacks = std::vector<Stack>;

Stacks toStacks(const std::vector<std::string> &stackLines);

struct Command {
  int count{-1};
  int from{-1};
  int to{-1};
};

using Commands = std::vector<Command>;

Commands toCommands(const std::vector<std::string> &commandLines);

std::pair<Stacks, Commands> parseLines(const std::vector<std::string> &lines);

void applyCommand(const Command &command, Stacks &stacks);
void applyCommand9001(const Command &command, Stacks &stacks);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  const auto lines = file_loader.as<std::string>();

  std::cout << "Task 5.1:\t" << task_1(lines) << "\n";
  std::cout << "Task 5.2:\t" << task_2(lines) << "\n";
}

std::string task_1(const std::vector<std::string> &lines) {
  auto [stacks, commands] = parseLines(lines);

  for (const auto &command : commands) {
    applyCommand(command, stacks);
  }

  std::stringstream ss;

  for (const auto &stack : stacks) {
    if (!stack.empty()) {
      ss << stack.front();
    }
  }

  return ss.str();
}

std::string task_2(const std::vector<std::string> &lines) {
  auto [stacks, commands] = parseLines(lines);

  for (const auto &command : commands) {
    applyCommand9001(command, stacks);
  }

  std::stringstream ss;

  for (const auto &stack : stacks) {
    if (!stack.empty()) {
      ss << stack.front();
    }
  }

  return ss.str();
}

std::pair<Stacks, Commands> parseLines(const std::vector<std::string> &lines) {
  const auto separator_it =
      std::find_if(lines.begin(), lines.end(),
                   [](const std::string &s) { return s.empty(); });
  std::vector<std::string> stack_lines{lines.begin(), separator_it};
  std::vector<std::string> command_lines{separator_it + 1, lines.end()};

  return {toStacks(stack_lines), toCommands(command_lines)};
}

Stacks toStacks(const std::vector<std::string> &stackLines) {
  int max_idx{-1};
  {
    const auto &indices = stackLines.back();
    std::stringstream indices_ss;
    indices_ss << indices;
    while (indices_ss >> max_idx) {
    }
  }
  if (max_idx < 0) {
    throw std::runtime_error("Invalid stack lines!");
  }
  if (max_idx > 9) {
    throw std::runtime_error("No more than 9 stacks supported!");
  }

  Stacks stacks(max_idx + 1);

  for (auto line_it = stackLines.begin(); line_it < stackLines.end() - 1;
       ++line_it) {
    const auto &line = *line_it;
    const auto max_elem = (line.size() + 1) / 4;
    for (int i = 1; i <= max_elem; ++i) {
      const auto elem = line[i * 4 - 3];
      if (elem >= 'A' && elem <= 'Z') {
        stacks[i].push_back(elem);
      }
    }
  }

  return stacks;
}
Commands toCommands(const std::vector<std::string> &commandLines) {
  Commands commands{};
  commands.reserve(commandLines.size());
  for (const auto &line : commandLines) {
    std::stringstream ss;
    ss << line;
    Command c;
    std::string void_string;
    ss >> void_string >> c.count >> void_string >> c.from >> void_string >>
        c.to;
    commands.emplace_back(std::move(c));
  }
  return commands;
}

void applyCommand(const Command &command, Stacks &stacks) {
  for (int i = 0; i < command.count; ++i) {
    stacks[command.to].push_front(stacks[command.from].front());
    stacks[command.from].pop_front();
  }
}

void applyCommand9001(const Command &command, Stacks &stacks) {
  std::deque<char> hold;
  for (int i = 0; i < command.count; ++i) {
    hold.push_front(stacks[command.from].front());
    stacks[command.from].pop_front();
  }
  while (!hold.empty()) {
    stacks[command.to].push_front(hold.front());
    hold.pop_front();
  }
}