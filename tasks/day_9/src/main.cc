#include "arg_to_vec.h"
#include "file_loader.h"
#include <array>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace jabartek::aoc;

int task_1(const std::vector<std::string> &lines);
int task_2(const std::vector<std::string> &lines);

using Position = std::pair<int, int>;

constexpr auto ROPE_LENGTH = 10z;
using Rope = std::array<Position, ROPE_LENGTH>;

enum class Direction {
  Up,
  Down,
  Left,
  Right,
};

int distance(Position a, Position b);
bool neighbors(Position a, Position b);
std::pair<Position, Position> move(Position head, Position tail, Direction dir);
std::pair<Position, Position> move(Position head, Position tail,
                                   Position target_head);
Direction toDirection(char c);

Rope moveRope(Rope rope, Direction dir);

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
  Rope rope;
  rope.fill({0, 0});
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
      std::array<std::array<char, 22>, 12> a;
      for (auto &arr : a) {
        arr.fill('x');
      }
      rope = moveRope(rope, dir);
      tail_positions.insert(rope.back());
    }
  }
  return tail_positions.size();
}

int distance(Position a, Position b) {
  return std::max(std::abs(a.first - b.first), std::abs(a.second - b.second));
}
bool neighbors(Position a, Position b) {
  return (a.first == b.first && (std::abs(a.second - b.second) <= 1)) ||
         (a.second == b.second && (std::abs(a.first - b.first) <= 1));
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

std::pair<Position, Position> move(Position head, Position tail,
                                   Position target_head) {
  if (distance(head, target_head) > 1) {
    throw std::runtime_error("That's too far!");
  }
  if (distance(target_head, tail) > 1) {
    static const std::initializer_list<std::pair<int, int>> offsets = {
        // First corners:
        {1, 1},
        {1, -1},
        {-1, -1},
        {-1, 1},
        // Then straigts:
        {1, 0},
        {-1, 0},
        {0, 1},
        {0, -1},
    };
    bool found{false};
    for (const auto &o : offsets) {
      if (neighbors({tail.first + o.first, tail.second + o.second},
                    target_head)) {
        tail = {tail.first + o.first, tail.second + o.second};
        found = true;
        break;
      }
    }
    if (!found) {
      tail = head;
    }
  }

  head = target_head;

  return {head, tail};
}

Rope moveRope(Rope rope, Direction dir) {

  const auto prev_rope = rope;

  {
    const auto [head_new, tail_new] = move(rope.at(0), rope.at(1), dir);
    rope.at(0) = head_new;
    rope.at(1) = tail_new;
  }

  for (int i{1}; i < rope.size() - 1; ++i) {
    const auto [_, tail_new] =
        move(prev_rope.at(i), prev_rope.at(i + 1), rope.at(i));
    rope.at(i + 1) = tail_new;
  }
  return rope;
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