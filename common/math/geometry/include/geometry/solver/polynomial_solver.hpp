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

#ifndef GEOMETRY__SOLVER__POLYNOMIAL_SOLVER_HPP_
#define GEOMETRY__SOLVER__POLYNOMIAL_SOLVER_HPP_

#include <vector>

namespace math
{
namespace geometry
{
class PolynomialSolver
{
public:
  /**
 * @brief solve linear equation a*x + b = 0
 *
 * @param a
 * @param b
 * @return std::vector<double> real root of the quadratic functions (from 0 to 1)
 */
  std::vector<double> solveLinearEquation(
    double a, double b, double min_value = 0, double max_value = 1) const;
  /**
 * @brief solve quadratic equation a*x^2 + b*x + c = 0
 *
 * @param a
 * @param b
 * @return std::vector<double> real root of the quadratic functions (from 0 to 1)
 */
  std::vector<double> solveQuadraticEquation(
    double a, double b, double c, double min_value = 0, double max_value = 1) const;
  /**
 * @brief solve cubic function a*t^3 + b*t^2 + c*t + d = 0
 *
 * @param a
 * @param b
 * @param c
 * @param d
 * @return std::vector<double> real root of the cubic functions (from 0 to 1)
 */
  std::vector<double> solveCubicEquation(
    double a, double b, double c, double d, double min_value = 0, double max_value = 1) const;
  /**
 * @brief calculate result of cubic function a*t^3 + b*t^2 + c*t + d
 *
 * @param a
 * @param b
 * @param c
 * @param d
 * @param t
 * @return double result of cubic function
 */
  double cubicFunction(double a, double b, double c, double d, double t) const;
  /**
 * @brief calculate result of quadratic function a*t^2 + b*t + c
 *
 * @param a
 * @param b
 * @param c
 * @param t
 * @return double result of quadratic function
 */
  double quadraticFunction(double a, double b, double c, double t) const;
  /**
 * @brief calculate result of quadratic function a*t + b
 *
 * @param a
 * @param b
 * @param t
 * @return double result of quadratic function
 */
  double linearFunction(double a, double b, double t) const;

  /**
   * @brief Hard coded parameter, tolerance of calculation results of the PolynomialSolver
   * This value was determined by Masaya Kataoka (@hakuturu583).
   * The reason this value is not std::numeric_limits<double>::epsilon is that when using 
   * this set of functions to find the intersection of a Catmull-Rom spline curve and a line segment, 
   * the solution is not a Catmull-Rom spline, it was confirmed that in cases where the solution is 
   * very close to the endpoints of the Hermite curves that make up the Catmull-Rom spline, 
   * the solution will no longer be obtained.
   */
  constexpr static double tolerance = 1e-7;

private:
  /**
 * @brief solve cubic equation x^3 + a*x^2 + b*x + c = 0, this code is public domain
 * @sa http://math.ivanovo.ac.ru/dalgebra/Khashin/poly/index.html
 * @param x
 * @param a
 * @param b
 * @param c
 * @return int
           if return value is 3, 3 real roots: x[0], x[1], x[2],
           if return value is 2, 2 real roots: x[0], x[1],
           if return value is 1, 1 real root : x[0], x[1] ± i*x[2],
 */
  int solveP3(std::vector<double> & x, double a, double b, double c) const;
  double _root3(double x) const;
  double root3(double x) const;
};
}  // namespace geometry
}  // namespace math

#endif  // GEOMETRY__SOLVER__POLYNOMIAL_SOLVER_HPP_
