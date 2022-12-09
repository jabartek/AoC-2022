#include "arg_to_vec.h"
#include "file_loader.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace jabartek::aoc;

int task_1(const std::vector<std::string> &lines);
int task_2(const std::vector<std::string> &lines);

class Tree {
public:
  int height{-1};
  int max_left{-1};
  int max_right{-1};
  int max_top{-1};
  int max_bottom{-1};

  bool isTop() const;
};

std::vector<std::vector<Tree>> toTrees(const std::vector<std::string> &lines);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  auto lines = file_loader.as<std::string>();

  std::cout << "Task 8.1:\t" << task_1(lines) << "\n";
  std::cout << "Task 8.2:\t" << task_2(lines) << "\n";
}

int task_1(const std::vector<std::string> &lines) {
  const auto trees = toTrees(lines);
  return std::accumulate(
      trees.begin(), trees.end(), 0, [](int left, const auto &row) -> int {
        return left + std::accumulate(row.begin(), row.end(), 0,
                                      [](int left, const auto &tree) -> int {
                                        return left + tree.isTop();
                                      });
      });
}
int task_2(const std::vector<std::string> &lines) {
  const auto trees = toTrees(lines);

  int max_score = -1;

  for (auto x{0}; x < std::ssize(trees.front()); ++x) {
    for (auto y{0}; y < std::ssize(trees); ++y) {
      auto x_left{x == 0 ? x : x - 1};
      for (; x_left > 0 && trees[y][x_left].height < trees[y][x].height;
           x_left--) {
      }
      x_left = x - x_left;

      auto x_right{x == std::ssize(trees[y]) - 1 ? x : x + 1};
      for (; x_right < std::ssize(trees[y]) - 1 &&
             trees[y][x_right].height < trees[y][x].height;
           x_right++) {
      }
      x_right -= x;

      auto y_top{y == 0 ? y : y - 1};
      for (; y_top > 0 && trees[y_top][x].height < trees[y][x].height;
           y_top--) {
      }
      y_top = y - y_top;

      auto y_bottom{y == std::ssize(trees) - 1 ? y : y + 1};
      for (; y_bottom < std::ssize(trees) - 1 &&
             trees[y_bottom][x].height < trees[y][x].height;
           y_bottom++) {
      }
      y_bottom -= y;

      const int score = x_left * x_right * y_top * y_bottom;

      if (score > max_score) {
        max_score = score;
      }
    }
  }
  return max_score;
}
std::vector<std::vector<Tree>> toTrees(const std::vector<std::string> &lines) {
  std::vector<std::vector<Tree>> trees{};
  for (const auto &line : lines) {
    trees.emplace_back();
    for (const auto &digit : line) {
      trees.back().push_back(Tree{.height = digit - '0'});
    }
  }
  for (auto x{0}; x < std::ssize(trees.front()); ++x) {
    for (auto y{1}; y < std::ssize(trees); ++y) {
      trees[y][x].max_top = trees[y - 1][x].height > trees[y - 1][x].max_top
                                ? trees[y - 1][x].height
                                : trees[y - 1][x].max_top;
    }
    for (auto y{std::ssize(trees) - 2}; y >= 0; --y) {
      trees[y][x].max_bottom =
          trees[y + 1][x].height > trees[y + 1][x].max_bottom
              ? trees[y + 1][x].height
              : trees[y + 1][x].max_bottom;
    }
  }
  for (auto y{0}; y < std::ssize(trees); ++y) {
    for (auto x{1}; x < std::ssize(trees.front()); ++x) {
      trees[y][x].max_left = trees[y][x - 1].height > trees[y][x - 1].max_left
                                 ? trees[y][x - 1].height
                                 : trees[y][x - 1].max_left;
    }
    for (auto x{std::ssize(trees.front()) - 2}; x >= 0; --x) {
      trees[y][x].max_right = trees[y][x].max_right =
          trees[y][x + 1].height > trees[y][x + 1].max_right
              ? trees[y][x + 1].height
              : trees[y][x + 1].max_right;
    }
  }
  return {trees};
}

bool Tree::isTop() const {
  return height > max_left || height > max_right || height > max_top ||
         height > max_bottom;
}