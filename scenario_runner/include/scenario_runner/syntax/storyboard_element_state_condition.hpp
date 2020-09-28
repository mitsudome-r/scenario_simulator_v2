// Copyright 2015-2020 Autoware Foundation. All rights reserved.
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

#ifndef SCENARIO_RUNNER__SYNTAX__STORYBOARD_ELEMENT_STATE_CONDITION_HPP_
#define SCENARIO_RUNNER__SYNTAX__STORYBOARD_ELEMENT_STATE_CONDITION_HPP_

#include <scenario_runner/syntax/storyboard_element_state.hpp>
#include <scenario_runner/syntax/storyboard_element_type.hpp>

namespace scenario_runner
{
inline namespace syntax
{
/* ==== StoryboardElementStateCondition ======================================
 *
 * <xsd:complexType name="StoryboardElementStateCondition">
 *   <xsd:attribute name="storyboardElementType" type="StoryboardElementType" use="required"/>
 *   <xsd:attribute name="storyboardElementRef" type="String" use="required"/>
 *   <xsd:attribute name="state" type="StoryboardElementState" use="required"/>
 * </xsd:complexType>
 *
 * ======================================================================== */
struct StoryboardElementStateCondition
{
  const String name;

  const StoryboardElementType type;

  const StoryboardElementState state;

  Scope inner_scope;

  template<typename Node, typename Scope>
  explicit StoryboardElementStateCondition(const Node & node, Scope & outer_scope)
  : name{readAttribute<String>(node, outer_scope, "storyboardElementRef")},
    type{readAttribute<StoryboardElementType>(node, outer_scope, "storyboardElementType")},
    state{readAttribute<StoryboardElementState>(node, outer_scope, "state")},
    inner_scope{outer_scope}
  {}

  auto compare(const Object & lhs, StoryboardElementState rhs) const
  {
    return asBoolean(lhs.as<StoryboardElementState>() == rhs);
  }

  auto evaluate() const
  {
    const auto result {compare(inner_scope.storyboard_elements.at(name).currentState(), state)};

    std::cout << indent <<
      "StoryboardElementState [Is " <<
      cyan <<
      "\"" <<
      name <<
      "\"" <<
      reset <<
      " in " <<
      state <<
      "? => " <<
      result <<
      "]" <<
      std::endl;

    return result;
  }
};
}
}  // namespace scenario_runner

#endif  // SCENARIO_RUNNER__SYNTAX__STORYBOARD_ELEMENT_STATE_CONDITION_HPP_