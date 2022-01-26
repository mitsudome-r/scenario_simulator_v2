// Copyright 2015-2020 Tier IV, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPENSCENARIO_INTERPRETER__SCOPE_HPP_
#define OPENSCENARIO_INTERPRETER__SCOPE_HPP_

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <memory>
#include <openscenario_interpreter/name.hpp>
#include <openscenario_interpreter/syntax/catalog_locations.hpp>
#include <openscenario_interpreter/syntax/entity_ref.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

namespace openscenario_interpreter
{
template <typename T>
struct is
{
  auto operator()(const Object & object) const { return object.is<T>(); }
};

template <>
struct is<Object>
{
  auto operator()(const Object &) const noexcept { return true; }
};

template <typename T>
auto as(const Object & object) -> decltype(auto)
{
  return object.as<T>();
}

template <>
constexpr auto as<Object>(const Object & object) -> decltype(auto)
{
  return object;
}

class EnvironmentFrame
{
  friend struct Scope;

  std::multimap<std::string, Object> variables;  // NOTE: must be ordered.

  EnvironmentFrame * const outer_frame = nullptr;

  std::multimap<std::string, EnvironmentFrame *> inner_frames;  // NOTE: must be ordered.

  std::vector<EnvironmentFrame *> unnamed_inner_frames;

#define DEFINE_SYNTAX_ERROR(TYPENAME, ...)                 \
  struct TYPENAME : public SyntaxError                     \
  {                                                        \
    explicit TYPENAME(const std::string & variable)        \
    : SyntaxError(__VA_ARGS__, std::quoted(variable), ".") \
    {                                                      \
    }                                                      \
  }

  DEFINE_SYNTAX_ERROR(AmbiguousReferenceTo, "Ambiguous reference to ");
  DEFINE_SYNTAX_ERROR(NoSuchVariableNamed, "No such variable named ");

#undef DEFINE_SYNTAX_ERROR

  template <typename T>
  using reference = typename std::conditional<
    std::is_same<Object, typename std::decay<T>::type>::value,
    typename std::remove_reference<T>::type, typename std::add_lvalue_reference<T>::type>::type;

  explicit EnvironmentFrame() = default;

  explicit EnvironmentFrame(EnvironmentFrame &, const std::string &);

public:
  explicit EnvironmentFrame(const EnvironmentFrame &) = delete;

  explicit EnvironmentFrame(EnvironmentFrame &&) = delete;

  auto define(const Name &, const Object &) -> void;

  template <typename T>
  auto find(const Name & name) const -> reference<T>
  {
    // NOTE: breadth first search
    for (std::vector<const EnvironmentFrame *> frames{this}; not frames.empty();) {
      auto objects = [&]() {
        std::vector<Object> result;
        for (auto && frame : frames) {
          boost::range::for_each(frame->variables.equal_range(name), [&](auto && name_and_value) {
            return result.push_back(name_and_value.second);
          });
        }
        return result;
      }();

      switch (boost::range::count_if(objects, is<T>())) {
        case 0:
          frames = [&]() {
            std::vector<const EnvironmentFrame *> result;
            for (auto && current_frame : frames) {
              boost::range::copy(current_frame->unnamed_inner_frames, std::back_inserter(result));
            }
            return result;
          }();
          break;
        case 1:
          return as<T>(*boost::range::find_if(objects, is<T>()));
        default:
          throw AmbiguousReferenceTo(name);
      }
    }

    return isOutermost() ? throw NoSuchVariableNamed(name) : outer_frame->find<T>(name);
  }

  template <typename T>
  auto find(const Prefixed<Name> & prefixed_name) const -> reference<T>
  {
    if (not prefixed_name.prefixes.empty()) {
      const auto found = resolveFrontPrefix(prefixed_name);
      switch (found.size()) {
        case 0:
          throw NoSuchVariableNamed(boost::lexical_cast<std::string>(prefixed_name));
        case 1:
          return found.front()->find<T>(prefixed_name.strip<1>());
        default:
          throw AmbiguousReferenceTo(boost::lexical_cast<std::string>(prefixed_name));
      }
    } else {
      return find<T>(prefixed_name.name);
    }
  }

  template <typename T>
  auto ref(const Prefixed<Name> & prefixed_name) const -> reference<T>
  {
    if (prefixed_name.absolute) {
      return outermostFrame().find<T>(prefixed_name);
    } else if (prefixed_name.prefixes.empty()) {
      return find<T>(prefixed_name.name);
    } else {
      return lookupFrame(prefixed_name)->find<T>(prefixed_name.strip<1>());
    }
  }

  auto isOutermost() const noexcept -> bool;

private:
  auto resolveFrontPrefix(const Prefixed<Name> &) const -> std::list<const EnvironmentFrame *>;

  auto lookupFrame(const Prefixed<Name> &) const -> const EnvironmentFrame *;

  auto outermostFrame() const noexcept -> const EnvironmentFrame &;
};

class Scope
{
  struct GlobalEnvironment
  {
    const boost::filesystem::path pathname;  // for substitution syntax '$(dirname)'

    std::unordered_map<std::string, Object> entities;  // ScenarioObject or EntitySelection

    const CatalogLocations * catalog_locations = nullptr;

    explicit GlobalEnvironment(const boost::filesystem::path &);

    auto entityRef(const EntityRef &) const -> Object;  // TODO: RETURN ScenarioObject TYPE!

    auto isAddedEntity(const EntityRef &) const -> bool;
  };

  const std::shared_ptr<EnvironmentFrame> frame;

  const std::shared_ptr<GlobalEnvironment> global_environment;

public:
  const std::string name;

  std::list<EntityRef> actors;

  Scope() = delete;

  Scope(const Scope &) = default;  // NOTE: shallow copy

  Scope(Scope &&) noexcept = default;

  explicit Scope(const std::string &, const Scope &);

  explicit Scope(const boost::filesystem::path &);

  template <typename T, typename... Ts>
  auto ref(Ts &&... xs) const -> decltype(auto)
  {
    return frame->ref<T>(std::forward<decltype(xs)>(xs)...);
  }

  auto global() const -> const GlobalEnvironment &;

  auto global() -> GlobalEnvironment &;

  auto local() const noexcept -> const Scope &;

  auto local() noexcept -> Scope &;

  auto insert(const Name &, const Object &) -> void;
};
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SCOPE_HPP_
