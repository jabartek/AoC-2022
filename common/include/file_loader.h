#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace jabartek::aoc::utilities {
class FileLoader {
public:
  FileLoader(std::filesystem::path filepath);

  template <class LineType> std::vector<LineType> as();
  template <class LineType> std::vector<std::optional<LineType>> as_optional();

protected:
private:
  std::ifstream stream_;

  void resetStream();
};
} // namespace jabartek::aoc::utilities