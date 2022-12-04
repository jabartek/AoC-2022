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

int priority(char item);
std::pair<std::string, std::string> split(const std::string &text);
int rucksackPriority(const std::string &text);

using Group = std::array<std::string, 3>;

std::vector<Group> toGroups(const std::vector<std::string> &lines);
int groupPriority(Group group);
std::set<char> itemOcurrences(const std::string &text);

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  const auto lines = file_loader.as<std::string>();

  std::cout << "Task 2.1:\t" << task_1(lines) << "\n";
  std::cout << "Task 2.2:\t" << task_2(lines) << "\n";
}

int task_1(const std::vector<std::string> &lines) {
  int sum{0};
  for (const auto &line : lines) {
    sum += rucksackPriority(line);
  }
  return sum;
}

int task_2(const std::vector<std::string> &lines) {
  const auto groups = toGroups(lines);
  int sum{0};
  for (const auto &group : groups) {
    sum += groupPriority(group);
  }
  return sum;
}

int priority(char item) {
  if (item >= 'a' && item <= 'z') {
    return item - 'a' + 1;
  }
  if (item >= 'A' && item <= 'Z') {
    return item - 'A' + 27;
  }
  throw std::runtime_error("Invalid item!");
}

std::pair<std::string, std::string> split(const std::string &text) {
  return std::make_pair(
      std::string{text.begin(), text.begin() + text.size() / 2},
      std::string{text.begin() + text.size() / 2, text.end()});
}

int rucksackPriority(const std::string &text) {
  const auto [first, second] = split(text);
  std::set<char> first_items;
  for (const auto &item : first) {
    first_items.insert(item);
  }
  for (const auto &item : second) {
    if (first_items.contains(item)) {
      return priority(item);
    }
  }
  throw std::runtime_error("Rucksack does not have repeating items!");
}

std::vector<Group> toGroups(const std::vector<std::string> &lines) {
  std::vector<Group> groups;
  for (int i{0}; i < lines.size(); i += 3) {
    groups.emplace_back(Group{lines[i], lines[i + 1], lines[i + 2]});
  }
  return groups;
}

int groupPriority(Group group) {
  std::array<std::set<char>, 3> items = {itemOcurrences(group.at(0)),
                                         itemOcurrences(group.at(1)),
                                         itemOcurrences(group.at(2))};
  std::array<int, 55> occurenceCounts{};

  const auto incrementItem = [&occurenceCounts](char item) {
    occurenceCounts.at(priority(item))++;
  };

  std::for_each(items.begin(), items.end(),
                [&occurenceCounts, &incrementItem](const auto &occurences) {
                  std::for_each(occurences.begin(), occurences.end(),
                                incrementItem);
                });

  const auto found =
      std::find(occurenceCounts.begin(), occurenceCounts.end(), 3);

  if (found == occurenceCounts.end()) {
    throw std::runtime_error("Rucksacks do not have items repeating 3 times!");
  }

  const auto result = std::distance(occurenceCounts.begin(), found);
  return result;
}

std::set<char> itemOcurrences(const std::string &text) {
  std::set<char> occurences;
  for (const auto &item : text) {
    occurences.insert(item);
  }
  return occurences;
}