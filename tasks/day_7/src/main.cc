#include "arg_to_vec.h"
#include "file_loader.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <numeric>
#include <set>
#include <sstream>

using namespace jabartek::aoc;

int task_1(const std::vector<std::string> &text);
int task_2(const std::vector<std::string> &text);

class Entity {
public:
  virtual std::string_view name() const;
  virtual std::string path() const;
  virtual std::shared_ptr<Entity> parent();
  virtual std::shared_ptr<Entity> cd(const std::string &path) {
    return nullptr;
  };

  virtual int size() const { return -1; }
  virtual ~Entity() = default;

  bool operator==(const Entity &rhs);

protected:
  Entity(std::string name = "/");
  Entity(std::string name, std::shared_ptr<Entity> parent);
  std::weak_ptr<Entity> parent_{};

private:
  std::string name_{};
};

class Directory : public Entity,
                  public std::enable_shared_from_this<Directory> {
public:
  Directory();
  Directory(std::string name, std::shared_ptr<Entity> parent);

  void addFile(std::string name, int size);
  void addDirectory(std::string name);
  std::shared_ptr<Entity> parent() override;

  std::shared_ptr<Entity> cd(const std::string &path) override;

  int size() const override;
  virtual ~Directory() = default;

private:
  std::set<std::shared_ptr<Entity>> items_;
};

class File : public Entity, public std::enable_shared_from_this<File> {
public:
  File(std::string name, int size, std::shared_ptr<Entity> parent);
  int size() const override;
  std::shared_ptr<Entity> cd(const std::string &path) override;
  std::shared_ptr<Entity> parent() override;

  virtual ~File() = default;

private:
  int size_{};
};

int main(int argc, char **argv) {
  const auto args = utilities::to_args({argc, argv});
  auto file_loader = utilities::FileLoader(args.args[0]);
  auto lines = file_loader.as<std::string>();
  std::cout << "Task 7.1:\t" << task_1(lines) << "\n";
  std::cout << "Task 7.2:\t" << task_2(lines) << "\n";
}

int task_1(const std::vector<std::string> &lines) {

  std::shared_ptr<Entity> cursor = std::make_shared<Directory>();

  auto line_it = lines.begin();
  while (line_it <= lines.end()) {
    const auto &line = *line_it;
    std::cout << "\n" << line << "\n";
    std::stringstream ss;
    ss << line;
    std::string word;
    ss >> word;
    if (word == "$") {
      ss >> word;
      if (word == "cd") {
        ss >> word;
        cursor = cursor->cd(word);
        line_it++;
        continue;
      }
      if (word == "ls") {
        const auto dir = dynamic_cast<Directory *>(cursor.get());
        do {
          line_it++;
          const auto &next_line = *line_it;
          std::stringstream nss;
          nss << next_line;
          nss >> word;
          if (word == "$") {
            break;
          }
          if (word == "dir") {
            nss >> word;
            dir->addDirectory(word);
          } else {
            const auto size = std::stoi(word);
            nss >> word;
            dir->addFile(word, size);
          }

        } while (word != "$" && line_it != lines.end());
      }
    }
  }

  return 0;
}
int task_2(const std::vector<std::string> &text) { return 0; }

Entity::Entity(std::string name) : name_{std::move(name)} {}
Entity::Entity(std::string name, std::shared_ptr<Entity> parent)
    : name_{std::move(name)}, parent_(parent) {}

bool Entity::operator==(const Entity &rhs) { return name() == rhs.name(); }

std::string_view Entity::name() const { return name_; }
std::string Entity::path() const {
  const auto locked_parent = parent_.lock();
  const auto slash_string = std::string("/");
  const std::string prefix =
      locked_parent ? locked_parent->path() + slash_string : slash_string;
  return prefix + std::string(name());
}
std::shared_ptr<Entity> Entity::parent() { return parent_.lock(); }
std::shared_ptr<Entity> Directory::parent() {
  auto parent_lock = parent_.lock();
  return parent_lock ? parent_lock : shared_from_this();
}
std::shared_ptr<Entity> File::parent() {
  auto parent_lock = parent_.lock();
  return parent_lock ? parent_lock : shared_from_this();
}

Directory::Directory() : Entity() {}
Directory::Directory(std::string name, std::shared_ptr<Entity> parent)
    : Entity(std::move(name), parent) {}

void Directory::addFile(std::string name, int size) {
  items_.insert(
      std::make_shared<File>(std::move(name), size, shared_from_this()));
}
void Directory::addDirectory(std::string name) {
  items_.insert(
      std::make_shared<Directory>(std::move(name), shared_from_this()));
}

std::shared_ptr<Entity> Directory::cd(const std::string &path) {
  const auto cdl = [&]() {
    if (path == "..") {
      return parent();
    }
    if (path == "/") {
      std::shared_ptr<Entity> this_ptr = shared_from_this();
      std::shared_ptr<Entity> parent_ptr = this_ptr->parent();
      while (parent_ptr && parent_ptr != this_ptr) {
        this_ptr = parent_ptr;
        parent_ptr = parent_ptr->parent();
      }
      return this_ptr;
    }
    const auto it =
        std::find_if(items_.begin(), items_.end(), [&path](const auto &item) {
          return item->name() == path;
        });
    if (it != items_.end()) {
      return *it;
    }
    throw std::runtime_error("No such directory!");
  };
  auto result = cdl();
  std::cout << "\n" << result->path() << "\n";
  return result;
}

int Directory::size() const {
  return std::accumulate(
      items_.begin(), items_.end(), 0,
      [](int size, const auto &item) { return size + item->size(); });
}
File::File(std::string name, int size, std::shared_ptr<Entity> parent)
    : Entity(std::move(name), parent), size_{size} {}
int File::size() const { return size_; }

std::shared_ptr<Entity> File::cd(const std::string &path) {
  throw std::runtime_error("Oops, can't cd on file!");
}