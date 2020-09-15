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

#ifndef SCENARIO_RUNNER__TYPE_TRAITS__IF_EVALUABLE_HPP_
#define SCENARIO_RUNNER__TYPE_TRAITS__IF_EVALUABLE_HPP_

#include <scenario_runner/concepts/evaluable.hpp>

namespace scenario_runner
{inline namespace type_traits
{
template<typename T, typename = void>
struct IfEvaluable
{
  template<typename Result, typename U, typename ... Us>
  static Result invoke(const Result & as_is, U &&, Us && ...)
  {
    return as_is;
  }
};

template<typename T>
struct IfEvaluable<T, typename std::enable_if<Evaluable<T>::value>::type>
{
  template<typename Result, typename U, typename ... Us>
  static Result invoke(const Result &, U & object, Us && ... xs)
  {
    return object.evaluate(std::forward<decltype(xs)>(xs)...);
  }
};
}}  // namespace scenario_runner::type_traits

#endif  // SCENARIO_RUNNER__TYPE_TRAITS__IF_EVALUABLE_HPP_
