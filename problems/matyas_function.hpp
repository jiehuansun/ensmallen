/**
 * @file matyas_function.hpp
 * @author Marcus Edel
 *
 * Definition of the Matyas function.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_OPTIMIZERS_PROBLEMS_MATYAS_FUNCTION_HPP
#define MLPACK_CORE_OPTIMIZERS_PROBLEMS_MATYAS_FUNCTION_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace optimization {
namespace test {

/**
 * The Matyas function, defined by
 *
 * \f[
 * f(x) = 0.26 * (x_1^2 + x_2^2) - 0.48 * x_1 * x_2
 * \f]
 *
 * This should optimize to f(x) = 0, at x = [0, 0].
 *
 * For more information, please refer to:
 *
 * @code
 * @article{Jamil2013,
 *   title   = {A Literature Survey of Benchmark Functions For Global
 *              Optimization Problems},
 *   author  = {Momin Jamil and Xin{-}She Yang},
 *   journal = {CoRR},
 *   year    = {2013},
 *   url     = {http://arxiv.org/abs/1308.4008}
 * }
 * @endcode
 */
class MatyasFunction
{
 public:
  //! Initialize the MatyasFunction.
  MatyasFunction();

  /**
  * Shuffle the order of function visitation. This may be called by the
  * optimizer.
  */
  void Shuffle();

  //! Return 1 (the number of functions).
  size_t NumFunctions() const { return 1; }

  //! Get the starting point.
  arma::mat GetInitialPoint() const { return arma::mat("-3; 3"); }

  //! Evaluate a function for a particular batch-size
  double Evaluate(const arma::mat& coordinates,
                  const size_t begin,
                  const size_t batchSize) const;

  //! Evaluate the gradient of a function for a particular batch-size
  void Gradient(const arma::mat& coordinates,
                const size_t begin,
                arma::mat& gradient,
                const size_t batchSize) const;
};

} // namespace test
} // namespace optimization
} // namespace mlpack

#endif // MLPACK_CORE_OPTIMIZERS_PROBLEMS_MATYAS_FUNCTION_HPP
