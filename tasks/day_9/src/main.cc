#include "arg_to_vec.h"
#include "file_loader.h"
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace jabartek::aoc;

int task_1(const std::vector<std::string> &lines);
int task_2(const std::vector<std::string> &lines);

using Position = std::pair<int, int>;

enum class Direction {
  Up,
  Down,
  Left,
  Right,
};

int distance(Position a, Position b);
std::pair<Position, Position> move(Position head, Position tail, Direction dir);
Direction toDirection(char c);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  const auto lines = file_loader.as<std::string>();

  std::cout << "Task 9.1:\t" << task_1(lines) << "\n";
  std::cout << "Task 9.2:\t" << task_2(lines) << "\n";
}

int task_1(const std::vector<std::string> &lines) {
  Position head_pos{0, 0};
  Position tail_pos{0, 0};
  std::set<Position> tail_positions{
      {0, 0},
  };

  for (const auto &line : lines) {
    std::stringstream ss;
    ss << line;
    char dir_c;
    int dist;
    ss >> dir_c >> dist;
    const auto dir = toDirection(dir_c);
    for (int i{0}; i < dist; ++i) {
      const auto [head_new, tail_new] = move(head_pos, tail_pos, dir);
      head_pos = head_new;
      tail_pos = tail_new;
      tail_positions.insert(tail_pos);
    }
  }

  return tail_positions.size();
}
int task_2(const std::vector<std::string> &lines) {
  //
  return 0;
}

int distance(Position a, Position b) {
  return std::max(std::abs(a.first - b.first), std::abs(a.second - b.second));
}

std::pair<Position, Position> move(Position head, Position tail,
                                   Direction dir) {
  const auto head_prev = head;
  switch (dir) {
  case Direction::Up:
    head.second -= 1;
    break;
  case Direction::Down:
    head.second += 1;
    break;
  case Direction::Left:
    head.first -= 1;
    break;
  case Direction::Right:
    head.first += 1;
    break;
  default:
    break;
  }
  if (distance(head, tail) > 1) {
    tail = head_prev;
  }
  return {head, tail};
}

Direction toDirection(char c) {
  switch (c) {
  case 'U':
    return Direction::Up;
  case 'D':
    return Direction::Down;
  case 'L':
    return Direction::Left;
  case 'R':
    return Direction::Right;
  default:
    throw std::runtime_error("Wrong direction!");
  }
}
