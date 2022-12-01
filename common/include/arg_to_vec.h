#pragma once

#include <string>
#include <vector>

namespace jabartek::aoc::utilities {
struct CArgs {
  int argc;
  char **argv;
};
struct Args {
  std::string executable;
  std::vector<std::string> args;
};

Args to_args(CArgs cargs);
} // namespace jabartek::aoc::utilities
