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

#ifndef SCENARIO_RUNNER__SYNTAX__ACTORS_HPP_
#define SCENARIO_RUNNER__SYNTAX__ACTORS_HPP_

#include <scenario_runner/syntax/entity_ref.hpp>

namespace scenario_runner
{
inline namespace syntax
{
/* ==== Actors ===============================================================
 *
 * <xsd:complexType name="Actors">
 *   <xsd:sequence>
 *     <xsd:element name="EntityRef" minOccurs="0" maxOccurs="unbounded" type="EntityRef"/>
 *   </xsd:sequence>
 *   <xsd:attribute name="selectTriggeringEntities" type="Boolean" use="required"/>
 * </xsd:complexType>
 *
 * ======================================================================== */
struct Actors
{
  // Indicates whether the triggering entities are considered actors.
  const Boolean select_triggering_entities;

  template<typename Node, typename Scope>
  explicit Actors(const Node & node, Scope & scope)
  : select_triggering_entities{readAttribute<Boolean>(node, scope, "selectTriggeringEntities",
        Boolean())}
  {
    callWithElements(node, "EntityRef", 0, unbounded, [&](auto && node)
      {
        scope.actors.emplace_back(node, scope);
      });
  }
};
}
}  // namespace scenario_runner

#endif  // SCENARIO_RUNNER__SYNTAX__ACTORS_HPP_