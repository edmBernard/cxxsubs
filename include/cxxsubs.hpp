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

struct my_functor {
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

// template <typename T, std::size_t... Indices, typename Function>
// std::vector<bool> for_each_impl(T &&t, std::index_sequence<Indices...>, Function&& f, int argc, char *argv[]) {
//   return {f(std::get<Indices>(t), argc, argv)...};
//   // static_cast<void>(l);  // just to remove l unused warning
// }
template <typename T, std::size_t... Indices, typename Function>
auto for_each_impl(T &&t, std::index_sequence<Indices...>, Function &&f, int argc, char *argv[]) -> std::vector<decltype(f(std::get<0>(t), argc, argv))> {
  return {f(std::get<Indices>(t), argc, argv)...};
}

template <typename... Types, typename Function>
auto for_each(std::tuple<Types...> &t, Function &&f, int argc, char *argv[]) {
  return for_each_impl(t, std::index_sequence_for<Types...>(), f, argc, argv);
}

class IOptions {
public:
  IOptions() {
  }
  ~IOptions() {
  }

  bool match(int argc, char *argv[]) {
    verbs_match = std::vector<bool>(false, verbs.size());
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

protected:
  std::vector<std::string> verbs;
  std::vector<bool> verbs_match;
  std::unique_ptr<cxxopts::Options> options;
  std::unique_ptr<cxxopts::ParseResult> parsing_result;
};

template <typename... OptionsTypes>
class Verbs {
public:
  Verbs(int argc, char *argv[]) {
    this->parse(argc, argv);
  }

  void parse(int argc, char *argv[]) {
    auto ret = for_each(this->parsers, my_functor(), argc, argv);
    for (auto&& i : ret) {
      std::cout << "i :" << i << std::endl;
    }
  }

private:
  std::tuple<OptionsTypes...> parsers;
  std::size_t length = sizeof...(OptionsTypes);
};

} // namespace cxxsubs

#endif // !OPTIONS_INTERFACE_HPP_
