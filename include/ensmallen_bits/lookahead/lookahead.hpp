/**
 * @file lookahead.hpp
 * @author Marcus Edel
 *
 * Lookahead is a stochastic gradient based optimization method which chooses a
 * search direction by looking ahead at the sequence of "fast weights" generated
 * by another optimizer.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef ENSMALLEN_LOOKAHEAD_LOOKAHEAD_HPP
#define ENSMALLEN_LOOKAHEAD_LOOKAHEAD_HPP

#include <ensmallen_bits/sgd/decay_policies/no_decay.hpp>

namespace ens {

/**
 * Lookahead is a stochastic gradient based optimization method which chooses a
 * search direction by looking ahead at the sequence of "fast weights" generated
 * by another optimizer.
 *
 * For more information, see the following.
 *
 * @code
 * @article{Zhang2019,
 *   author  = {Michael R. Zhang and James Lucas and Geoffrey E. Hinton and
 *              Jimmy Ba},
 *   title   = {Lookahead Optimizer: k steps forward, 1 step back},
 *   journal = {CoRR},
 *   year    = {2019},
 *   url     = {http://arxiv.org/abs/1907.08610}
 * }
 *
 * Lookahead can optimize differentiable separable functions.  For more details,
 * see the documentation on function types included with this distribution or on
 * the ensmallen website.
 *
 * @tparam BaseOptimizerType Optimizer type for the forward step.
 * @tparam DecayPolicyType Decay policy used during the iterative update
 *     process to adjust the step size. By default the step size isn't going to
 *     be adjusted (i.e. NoDecay is used).
 */
template<typename BaseOptimizerType, typename DecayPolicyType = NoDecay>
class Lookahead
{
 public:
  /**
   * Construct the Lookahead optimizer with the given function and parameters.
   * The defaults here are not necessarily good for the given problem, so it is
   * suggested that the values used be tailored to the task at hand.  The
   * maximum number of iterations refers to the maximum number of points that
   * are processed (i.e., one iteration equals one point; one iteration does not
   * equal one pass over the dataset).
   *
   * @param baseOptimizer Optimizer for the forward step.
   * @param stepSize Step size for each iteration.
   * @param k The synchronization period.
   * @param maxIterations Maximum number of iterations allowed (0 means no
   *        limit).
   * @param tolerance Maximum absolute tolerance to terminate algorithm.
   * @param decayPolicy Instantiated decay policy used to adjust the step size.
   * @param exactObjective Calculate the exact objective (Default: estimate the
   *        final objective obtained on the last pass over the data).
   */
  Lookahead(const BaseOptimizerType& baseOptimizer = BaseOptimizerType(),
            const double stepSize = 0.001,
            const size_t k = 5,
            const size_t maxIterations = 100000,
            const double tolerance = 1e-5,
            const DecayPolicyType& decayPolicy = DecayPolicyType(),
            const bool exactObjective = false);

  /**
   * Optimize the given function using Lookahead. The given starting point will
   * be modified to store the finishing point of the algorithm, and the final
   * objective value is returned.
   *
   * @tparam DecomposableFunctionType Type of the function to be optimized.
   * @tparam MatType Type of the parameters matrix.
   * @tparam GradType Type of the gradient matrix.
   * @tparam CallbackTypes Types of callback functions.
   * @param function Function to optimize.
   * @param iterate Starting point (will be modified).
   * @param callbacks Callback functions.
   * @return Objective value of the final point.
   */
  template<typename DecomposableFunctionType,
           typename MatType,
           typename GradType,
           typename... CallbackTypes>
  typename std::enable_if<IsArmaType<GradType>::value,
      typename MatType::elem_type>::type
  Optimize(DecomposableFunctionType& function,
           MatType& iterate,
           CallbackTypes&&... callbacks);

  //! Forward the MatType as GradType.
  template<typename DecomposableFunctionType,
           typename MatType,
           typename... CallbackTypes>
  typename MatType::elem_type Optimize(DecomposableFunctionType& function,
                                       MatType& iterate,
                                       CallbackTypes&&... callbacks)
  {
    return Optimize<DecomposableFunctionType, MatType, MatType,
        CallbackTypes...>(function, iterate,
        std::forward<CallbackTypes>(callbacks)...);
  }

  //! Get the base optimizer.
  BaseOptimizerType BaseOptimizer() const { return baseOptimizer; }
  //! Modify the base optimizer.
  BaseOptimizerType& BaseOptimizer() { return baseOptimizer; }

  //! Get the step size.
  double StepSize() const { return stepSize; }
  //! Modify the step size.
  double& StepSize() { return stepSize; }

  //! Get the synchronization period.
  double K() const { return k; }
  //! Modify the synchronization period.
  double& K() { return k; }

  //! Get the maximum number of iterations (0 indicates no limit).
  size_t MaxIterations() const { return maxIterations; }
  //! Modify the maximum number of iterations (0 indicates no limit).
  size_t& MaxIterations() { return maxIterations; }

  //! Get the tolerance for termination.
  double Tolerance() const { return tolerance; }
  //! Modify the tolerance for termination.
  double& Tolerance() { return tolerance; }

  //! Get the step size decay policy.
  const DecayPolicyType& DecayPolicy() const { return decayPolicy; }
  //! Modify the step size decay policy.
  DecayPolicyType& DecayPolicy() { return decayPolicy; }

  //! Get the instantiated decay policy type.  Be sure to check its type with
  //! Has() before using!
  const Any& InstDecayPolicy() const { return instDecayPolicy; }
  //! Modify the instantiated decay policy type.  Be sure to check its type with
  //! Has() before using!
  Any& InstDecayPolicy() { return instDecayPolicy; }

  //! Get whether or not the actual objective is calculated.
  bool ExactObjective() const { return exactObjective; }
  //! Modify whether or not the actual objective is calculated.
  bool& ExactObjective() { return exactObjective; }

 private:
  //! The base optimizer for the forward step.
  BaseOptimizerType baseOptimizer;

  //! The step size for each example.
  double stepSize;

  //! Synchronization period.
  size_t k;

  //! The maximum number of allowed iterations.
  size_t maxIterations;

  //! The tolerance for termination.
  double tolerance;

  //! The decay policy used to update the step size.
  DecayPolicyType decayPolicy;

  //! Controls whether or not the actual Objective value is calculated.
  bool exactObjective;

  //! Flag indicating whether the update policy
  //! parameters have been initialized.
  bool isInitialized;

  //! The initialized decay policy.
  Any instDecayPolicy;
};

} // namespace ens

// Include implementation.
#include "lookahead_impl.hpp"

#endif
