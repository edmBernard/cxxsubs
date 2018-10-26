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
auto for_each_impl(T &&t, std::index_sequence<Indices...>, Function &&f, int argc, char *argv[]) -> std::vector<decltype(f(std::get<0>(t), argc, argv))> {
  return {f(std::get<Indices>(t), argc, argv)...};
}

template <typename... Types, typename Function>
auto for_each(std::tuple<Types...> &t, Function &&f, int argc, char *argv[]) {
  return for_each_impl(t, std::index_sequence_for<Types...>(), f, argc, argv);
}

} // namespace utils


namespace functors {

struct execute_options {
  template <typename T>
  bool operator()(T &&t, int argc, char *argv[]) {
    if (t.match(argc, argv)) {
      t.parse(argc, argv);
      t.exec();
      return true;
    }
    return false;
  }
};

struct get_verbs_options {
  template <typename T>
  std::string operator()(T &&t, int argc, char *argv[]) {
    return t.get_verbs();
  }
};

} // namespace functors

//! Interface for Options Parser.
//!
//!
class IOptions {
public:
  IOptions() {
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
      this->parsing_result =
          std::make_unique<cxxopts::ParseResult>(this->options->parse(tmp_argc, tmp_argv));
      this->validate();
    }
  }

  virtual void validate() = 0;
  virtual void exec() = 0;

  std::string get_verbs() {
    return utils::join(this->verbs, " ");
  }

protected:
  std::vector<std::string> verbs;
  std::unique_ptr<cxxopts::Options> options;
  std::unique_ptr<cxxopts::ParseResult> parsing_result;
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
    this->parse(argc, argv);
  }

  void parse(int argc, char *argv[]) {
    auto ret = utils::for_each(this->parsers, functors::execute_options(), argc, argv);

    int count = 0;
    for (auto &&i : ret) {
      if (i) {
        ++count;
      }
    }

    if (count == 0) {
      std::cout << "Available command: " << std::endl;
      auto verbs_list = utils::for_each(this->parsers, functors::get_verbs_options(), argc, argv);
      for (auto &&i : verbs_list) {
        std::cout << "    - " << i << std::endl;
      }
    }
  }

private:
  std::tuple<FirstOptionsTypes, OptionsTypes...> parsers;
  std::size_t length = sizeof...(OptionsTypes) + 1;
};

} // namespace cxxsubs


#endif // !OPTIONS_INTERFACE_HPP_
