#include "file_loader.h"

namespace jabartek::aoc::utilities {
FileLoader::FileLoader(std::filesystem::path filepath) {
  stream_.open(filepath);
}

template <class LineType> std::vector<LineType> FileLoader::as() {
  std::string line{};
  std::vector<LineType> data;
  while (std::getline(stream_, line)) {
    std::stringstream conv;
    conv << line;
    LineType tmp;
    conv >> tmp;
    data.emplace_back(std::move(tmp));
  }
  resetStream();
  return data;
}

template <> std::vector<std::string> FileLoader::as() {
  std::string line{};
  std::stringstream conv;
  std::vector<std::string> data;
  while (std::getline(stream_, line)) {
    data.emplace_back(std::move(line));
  }
  resetStream();
  return data;
}

template std::vector<int> FileLoader::as();

template <class LineType>
std::vector<std::optional<LineType>> FileLoader::as_optional() {
  std::string line{};
  std::vector<std::optional<LineType>> data;
  while (std::getline(stream_, line)) {
    if (line.empty()) {
      data.emplace_back(std::nullopt);
      continue;
    }
    std::stringstream conv;
    conv << line;
    LineType tmp;
    conv >> tmp;
    data.emplace_back(std::move(tmp));
  }
  resetStream();
  return data;
}

template std::vector<std::optional<int>> FileLoader::as_optional();

void FileLoader::resetStream() {
  stream_.clear();
  stream_.seekg(0, std::ios::beg);
}

} // namespace jabartek::aoc::utilities