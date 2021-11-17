//
//  cxxsubs
//
//  https://github.com/edmBernard/cxxsubs
//
//  Created by Erwan BERNARD on 23/10/2018.
//
//  Copyright (c) 2018 Erwan BERNARD. All rights reserved.
//  Distributed under the Apache License, Version 2.0. (See accompanying
//  file LICENSE or copy at http://www.apache.org/licenses/LICENSE-2.0)
//

#pragma once

#include "cxxopts.hpp"
#include <iomanip>
#include <regex>

namespace cxxsubs {

namespace utils {

inline std::string join(const std::vector<std::string> &tokens, const std::string &delimiter) {
  std::stringstream stream;
  stream << tokens.front();
  std::for_each(begin(tokens) + 1, end(tokens),
                [&](const std::string &elem) { stream << delimiter << elem; });
  return stream.str();
}

inline std::vector<std::string> split(std::string const &original, char separator) {
  std::vector<std::string> results;
  std::string::const_iterator start = original.begin();
  std::string::const_iterator end = original.end();
  std::string::const_iterator next = std::find(start, end, separator);

  while (next != end) {
    results.push_back(std::string(start, next));
    start = next + 1;
    next = std::find(start, end, separator);
  }

  results.push_back(std::string(start, next));

  return results;
}

inline bool match(std::vector<std::string> verbs, int argc, char *argv[]) {
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
  execute_options(int argc, char *argv[])
      : argc(argc) {
    this->argv = argv;
  }
  int argc;
  char **argv;

  template <typename T>
  std::tuple<bool, int> operator()(T &&t) {
    if (t.match(this->argc, this->argv)) {
      // Parse argument if return is not a success early return
      int retParse = t.parse(this->argc, this->argv);
      if (retParse != EXIT_SUCCESS) {
        return {true, retParse};
      }
      // Execute command
      int retExec = t.exec();
      return {true, retExec};
    }
    return {false, EXIT_SUCCESS};
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

struct set_completions {
  set_completions(std::vector<std::string> verbs_list)
      : verbs_list(verbs_list) {
  }
  std::vector<std::string> verbs_list;

  template <typename T>
  bool operator()(T &&t) {
    return false;
  }
};

} // namespace functors

template <typename FirstOptionsTypes, typename... OptionsTypes>
int Verbs(int argc, char *argv[]); // Early declaration

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

  //! Function called for parameters validation
  [[nodiscard]] virtual int validate() = 0;

  //! Function that contain execution
  [[nodiscard]] virtual int exec() = 0;

private:
  template <typename FirstOptionsTypes, typename... OptionsTypes>
  friend int Verbs(int argc, char *argv[]);

  //! Functor use to process all options, there need access to private member
  friend class functors::execute_options;
  friend class functors::get_verbs_options;
  friend class functors::get_description_options;

  bool match(int argc, char *argv[]) {
    return utils::match(this->verbs, argc, argv);
  }

  virtual int parse(int argc, char *argv[]) {
    if (this->match(argc, argv)) {
      int tmp_argc = argc - verbs.size();
      char **tmp_argv = argv + verbs.size();
      this->parse_result =
          std::make_unique<cxxopts::ParseResult>(this->options.parse(tmp_argc, tmp_argv));
      return this->validate();
    }
    return EXIT_SUCCESS;
  }

  std::vector<std::string> verbs;
  std::string description;

protected:
  //! Get the verb of this option
  std::string get_verbs() {
    return utils::join(this->verbs, " ");
  }

  //! Get the description of this option
  std::string get_desc() {
    return this->description;
  }

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
      ("show", "show completion code to add in bashrc", cxxopts::value<bool>())
      ("exec_name", "the executable name to add in completion script", cxxopts::value<std::string>())
      ("help", "Print help");

    // clang-format on
    options.parse_positional({"verbs"});
  }

  int validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      return EXIT_FAILURE;
    }

    if (this->parse_result->count("show") ^ this->parse_result->count("exec_name")) {
      std::cout << "Error: parsing options: 'show' and 'exec_name' must be filled" << std::endl;
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  int exec() {
    if (this->parse_result->count("show")) {
      std::string my_name = (*this->parse_result)["exec_name"].as<std::string>();
      std::cout << "_" + my_name + "_completions() {\n"
                                   "  local cur_word args type_list\n"
                                   "  cur_word=\"${COMP_WORDS[COMP_CWORD]}\"\n"
                                   "  args=(\"${COMP_WORDS[@]}\")\n\n"
                                   "  type_list=$(" +
                       my_name + " completion ${args[@]:1})\n"
                                 "  COMPREPLY=( $(compgen -W \"${type_list}\" -- ${cur_word}) )\n\n"
                                 "  # if no match was found, fall back to filename completion\n"
                                 "  if [ ${#COMPREPLY[@]} -eq 0 ]; then\n"
                                 "    COMPREPLY=( $(compgen -f -- \"${cur_word}\" ) )\n"
                                 "  fi\n"
                                 "  return 0\n"
                                 "}\n"
                                 "complete -F _" +
                       my_name + "_completions " + my_name
                << std::endl;
    } else {

      for (auto &&i : this->other_verbs_list) {
        if ((*this->parse_result).count("verbs")) {
          std::regex patterns(utils::join((*this->parse_result)["verbs"].as<std::vector<std::string>>(), " ") + ".*");
          std::smatch pieces_match;
          if (std::regex_match(i, pieces_match, patterns)) {
            std::cout << i << std::endl;
          }
        } else {
          // this part is never used in real case autocompetion pass empty string so there is a verb
          std::cout << utils::split(i, ' ')[0] << std::endl;
        }
      }
    }
    return EXIT_SUCCESS;
  };

  void set_verbs_list(std::vector<std::string> verbs) {
    this->other_verbs_list = verbs;
  }

private:
  std::vector<std::string> other_verbs_list;
};

namespace functors {

// Specialize set completion for Completion Command class
template <>
inline bool set_completions::operator()<CompletionCommand &>(CompletionCommand &t) {
  t.set_verbs_list(verbs_list);
  return true;
}

} // namespace functors

//! Subcommand Parser.
//!
//! \tparam FirstOptionsTypes  Used to force at least one argument in template
//! \tparam OptionsTypes  Other template arguments
//!
template <typename FirstOptionsTypes, typename... OptionsTypes>
int Verbs(int argc, char *argv[]) {
  try {
    std::tuple<FirstOptionsTypes, OptionsTypes...> parsers;
    std::size_t length = sizeof...(OptionsTypes) + 1;

    auto verbs_list = utils::for_each(parsers, functors::get_verbs_options());
    auto descriptions_list = utils::for_each(parsers, functors::get_description_options());

    // inject all verb list in Completion Command
    utils::for_each(parsers, functors::set_completions(verbs_list));

    // execute all Parser
    auto ret = utils::for_each(parsers, functors::execute_options(argc, argv));

    // Check if at least an options has match
    bool has_match = false;
    int returnCode = EXIT_SUCCESS;
    for (auto &[retMatch, retExec] : ret) {
      if (retMatch) {
        has_match = true;
        returnCode = retExec;
      }
    }

    if (has_match) {
      return returnCode;
    }

    // If no option match show help
    std::cout << "Available command: " << std::endl;
    for (std::size_t i = 0; i < verbs_list.size(); ++i) {
      std::cout << "    - " << std::left << std::setw(20) << verbs_list[i] << descriptions_list[i] << std::endl;
    }

  } catch (const cxxopts::OptionException &e) {
    std::cout << "Error: parsing options: " << e.what() << std::endl;
  }
  return EXIT_FAILURE;
}

} // namespace cxxsubs
