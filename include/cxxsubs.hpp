//
//  cxxsubs
//
//  github.com/ebernard
//
//  Created by Erwan BERNARD on 23/10/2018.
//
//  Copyright (c) 2018 Erwan BERNARD. All rights reserved.
//  Distributed under the Apache License, Version 2.0. (See accompanying
//  file LICENSE or copy at http://www.apache.org/licenses/LICENSE-2.0)
//

#pragma once
#ifndef OPTIONS_INTERFACE_HPP_
#define OPTIONS_INTERFACE_HPP_

#include "cxxopts.hpp"
#include <iomanip>

namespace cxxsubs {

namespace utils {

std::string join(const std::vector<std::string> &tokens, const std::string &delimiter) {
  std::stringstream stream;
  stream << tokens.front();
  std::for_each(begin(tokens) + 1, end(tokens),
                [&](const std::string &elem) { stream << delimiter << elem; });
  return stream.str();
}

bool match(std::vector<std::string> verbs, int argc, char *argv[]) {
  if (std::size_t(argc - 1) < verbs.size()) {
    return false;
  }
  for (std::size_t i = 0; i < verbs.size(); ++i) {
    if (argv[i + 1] != verbs[i]) {
      return false;
    }
  }
  return true;
}

template <typename T, std::size_t... Indices, typename Function>
auto for_each_impl(T &&t, std::index_sequence<Indices...>, Function &&f) -> std::vector<decltype(f(std::get<0>(t)))> {
  return {f(std::get<Indices>(t))...};
}

template <typename... Types, typename Function>
auto for_each(std::tuple<Types...> &t, Function &&f) {
  return for_each_impl(t, std::index_sequence_for<Types...>(), f);
}

} // namespace utils


namespace functors {

struct execute_options {
  execute_options(int argc, char *argv[]) : argc(argc) {
    this->argv = argv;
  }
  int argc;
  char **argv;

  template <typename T>
  bool operator()(T &&t) {
    if (t.match(this->argc, this->argv)) {
      t.parse(this->argc, this->argv);
      t.exec();
      return true;
    }
    return false;
  }
};

struct get_verbs_options {
  template <typename T>
  std::string operator()(T &&t) {
    return t.get_verbs();
  }
};

struct get_description_options {
  template <typename T>
  std::string operator()(T &&t) {
    return t.get_desc();
  }
};

} // namespace functors


//! Interface for Options Parser.
//!
//!
class IOptions {
public:
  IOptions(std::vector<std::string> verbs, std::string description = "")
      : verbs(verbs), description(description) {
  }
  ~IOptions() {
  }

  bool match(int argc, char *argv[]) {
    return utils::match(this->verbs, argc, argv);
  }

  virtual void parse(int argc, char *argv[]) {
    if (this->match(argc, argv)) {
      int tmp_argc = argc - verbs.size();
      char **tmp_argv = argv + verbs.size();
      this->parse_result =
          std::make_unique<cxxopts::ParseResult>(this->options.parse(tmp_argc, tmp_argv));
      this->validate();
    }
  }

  virtual void validate() = 0;
  virtual void exec() = 0;

  std::string get_verbs() {
    return utils::join(this->verbs, " ");
  }

  std::string get_desc() {
    return this->description;
  }

protected:
  std::vector<std::string> verbs;
  std::string description;
  cxxopts::Options options = cxxopts::Options(this->get_verbs(), this->description);
  std::unique_ptr<cxxopts::ParseResult> parse_result;
};


class CompletionCommand : public cxxsubs::IOptions {
public:
  CompletionCommand()
      : cxxsubs::IOptions({"completion"}, "Completion Command for bash") {

    // clang-format off
    this->options.add_options()
      ("verbs", "verbs list for each command", cxxopts::value<std::vector<std::string>>())
      ("help", "Print help");

    // clang-format on
    options.parse_positional({"verbs"});

  }

  void validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }
  }

  void exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;
    if ((*this->parse_result).count("verbs")) {
      for (auto &&i : (*this->parse_result)["verbs"].as<std::vector<std::string>>()) {
        std::cout << "verbs :" << i << std::endl;
      }
    }

    std::cout << "Others :" << std::endl;

    for (auto &&i : this->other_verbs_list) {
      std::cout << "verbs :" << i << std::endl;
    }
  };

  void set_verbs_list(std::vector<std::string> verbs) {
    this->other_verbs_list = verbs;
  }
private:
  std::vector<std::string> other_verbs_list;
};


//! Subcommand Parser.
//!
//! \tparam FirstOptionsTypes  Used to force at least one argument in template
//! \tparam OptionsTypes  Other template arguments
//!
template <typename FirstOptionsTypes, typename... OptionsTypes>
class Verbs {
public:
  Verbs(int argc, char *argv[]) {
    try {
      this->parse(argc, argv);
    } catch (const cxxopts::OptionException &e) {
      std::cout << "Error: parsing options: " << e.what() << std::endl;
      exit(1);
    }
  }

  void parse(int argc, char *argv[]) {
    auto verbs_list = utils::for_each(this->parsers, functors::get_verbs_options());
    auto descriptions_list = utils::for_each(this->parsers, functors::get_description_options());

    // inject all verb list in Completion Command
    std::get<0>(this->parsers).set_verbs_list(verbs_list);

    auto ret = utils::for_each(this->parsers, functors::execute_options(argc, argv));

    // Check if at least an options has match
    bool has_match = false;
    for (auto &&i : ret) {
      if (i) {
        has_match = true;
      }
    }

    // If no option match show help
    if (!has_match) {
      std::cout << "Available command: " << std::endl;
      for (std::size_t i = 0; i < verbs_list.size(); ++i) {
        std::cout << "    - " << std::left << std::setw(20) << verbs_list[i] << descriptions_list[i] << std::endl;
      }
    }
  }

private:
  std::tuple<CompletionCommand, FirstOptionsTypes, OptionsTypes...> parsers;
  std::size_t length = sizeof...(OptionsTypes) + 1;
};

} // namespace cxxsubs

#endif // !OPTIONS_INTERFACE_HPP_
