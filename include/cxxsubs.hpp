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
  template<class T>
  bool operator()(T&& t, int argc, char *argv[]) {
    if (t.match(argc, argv)) {
      t.parse(argc, argv);
      t.exec();
    }
    return true;
  }
};

template <typename T, std::size_t... Is>
void for_each(T &&t, std::index_sequence<Is...>, int argc, char *argv[]) {
  auto l = {my_functor()(std::get<Is>(t), argc, argv)...};
  static_cast<void>(l);  // just to remove l unused
}

template<typename... Ts>
void for_each_in_tuple(std::tuple<Ts...> & t, int argc, char *argv[])
{
    for_each(t, std::index_sequence_for<Ts...>(), argc, argv);
}

class IOptions {
public:
  IOptions() {}
  ~IOptions() {}

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

template <class... OptionsTypes> class Verbs {
public:
  Verbs(int argc, char *argv[]) {this->parse(argc, argv);}

  void parse(int argc, char *argv[]) {
    for_each_in_tuple(this->parsers, argc, argv);
  }

private:
  std::tuple<OptionsTypes...> parsers;
  std::size_t length = sizeof...(OptionsTypes);
};

} // namespace cxxsubs

#endif // !OPTIONS_INTERFACE_HPP_
