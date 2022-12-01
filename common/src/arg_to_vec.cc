#include "arg_to_vec.h"

namespace jabartek::aoc::utilities {
Args to_args(CArgs cargs) {
  return Args{cargs.argv[0], {cargs.argv[1], cargs.argv[cargs.argc - 1]}};
}
} // namespace jabartek::aoc::utilities
