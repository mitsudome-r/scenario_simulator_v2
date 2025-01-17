// Copyright 2015 TIER IV, Inc. All rights reserved.
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

#include <openscenario_interpreter/reader/attribute.hpp>
#include <openscenario_interpreter/reader/element.hpp>
#include <openscenario_interpreter/syntax/parameter_condition.hpp>  // for ParameterCondition::compare
#include <openscenario_interpreter/syntax/value_constraint.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
ValueConstraint::ValueConstraint(const pugi::xml_node & node, Scope & scope)
: rule(readAttribute<Rule>("rule", node, scope)), value(readAttribute<String>("value", node, scope))
{
}

ValueConstraint::ValueConstraint(const openscenario_msgs::msg::ValueConstraint & message)
: rule(boost::lexical_cast<Rule>(message.rule)), value(message.value)
{
}

auto ValueConstraint::evaluate(const Object & object) const -> bool
{
  return ParameterCondition::compare(object, rule, value);
}
}  // namespace syntax
}  // namespace openscenario_interpreter
