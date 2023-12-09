#include <cassert>
#include <concepts>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <range/v3/view/getlines.hpp>
#include <range/v3/view/transform.hpp>

enum class Instruction { Cd, Ls };

struct Command {
    Instruction m_instruction;
    std::string m_argument;
};

struct File {
    std::size_t m_size;
};

struct Directory {
    std::size_t m_size;
    std::string m_name;

    std::unordered_map<std::string, std::unique_ptr<Directory>> m_children;

    Directory(std::size_t size, std::string name) : m_size{size}, m_name{std::move(name)} {}

    void visit(std::invocable<Directory const&> auto fn) const {
        bool cont = fn(*this);
        if (!cont)
            return;

        for (auto const& [_, child] : m_children) {
            child.get()->visit(fn);
        }
    }
};

using Line = std::variant<Command, File, std::monostate>;

struct State {
    Directory                                      m_root{0, ""};
    std::vector<std::reference_wrapper<Directory>> m_directories{m_root};

    void operator()(Command command) {
        switch (command.m_instruction) {
        case Instruction::Ls:
            break;
        case Instruction::Cd:
            if (command.m_argument == "..") {
                m_directories.pop_back();
                return;
            }
            if (command.m_argument == "/") {
                m_directories.clear();
                m_directories.push_back(m_root);
                return;
            }

            auto dir = m_directories.back().get().m_children.try_emplace(
                command.m_argument, std::make_unique<Directory>(0, command.m_argument));

            m_directories.push_back(*dir.first->second);
        }
    }

    void operator()(File file) {
        for (Directory& dir : m_directories)
            dir.m_size += file.m_size;
    }

    void operator()(Line line) { std::visit(*this, std::move(line)); }
    void operator()(std::monostate) {}
};

Line parse(std::string_view line) {
    if (line.starts_with("dir"))
        return std::monostate{};

    if (line.starts_with('$')) {
        auto const command = line.substr(2);

        if (command.starts_with("ls")) {
            return Command{Instruction::Ls, ""};
        }

        assert(command.starts_with("cd"));
        return Command{Instruction::Cd, std::string{command.substr(command.find(' ') + 1)}};
    }

    auto const space = line.find(' ');

    return File{std::stoul(std::string{line.substr(0, space)})};
}

int main(int argc, char** argv) {
    assert(argc == 2);

    std::ifstream file{argv[1]};
    State         state;

    for (auto line : ranges::getlines(file) | ranges::views::transform(parse))
        state(std::move(line));

    std::size_t total = 0;
    state.m_root.visit([&total](auto const& dir) {
        if (dir.m_size <= 100000)
            total += dir.m_size;

        return true;
    });
    std::cout << total << "\n";

    auto const  to_free  = 30000000 - (70000000 - state.m_root.m_size);
    std::size_t smallest = state.m_root.m_size;
    state.m_root.visit([&](auto const& dir) {
        if (dir.m_size >= to_free && dir.m_size < smallest)
            smallest = dir.m_size;

        return dir.m_size >= to_free;
    });
    std::cout << smallest << "\n";
}
