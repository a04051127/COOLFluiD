#ifndef COOLFluiD_Physics_ICP_ICPNEQ2DPivtTvToCons_hh
#define COOLFluiD_Physics_ICP_ICPNEQ2DPivtTvToCons_hh

//////////////////////////////////////////////////////////////////////

#include "NEQ/Euler2DNEQPivtTvToCons.hh"

//////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace Physics {
    
    namespace ICP {

//////////////////////////////////////////////////////////////////////

/**
 * This class represents a transformer of variables from
 * [p_i u v T Tv] to conservative variables
 *
 * @author Andrea Lani
 */
class ICPNEQ2DPivtTvToCons : public NEQ::Euler2DNEQPivtTvToCons {
public:
  
  typedef Framework::MultiScalarTerm<Physics::NavierStokes::EulerTerm> PTERM;
  
  /**
   * Default constructor without arguments
   */
  ICPNEQ2DPivtTvToCons(Common::SafePtr<Framework::PhysicalModelImpl> model);

  /**
   * Default destructor
   */
  virtual ~ICPNEQ2DPivtTvToCons();

  /**
   * Transform a set of state vectors into another one
   */
  virtual void transform(const Framework::State& state, Framework::State& result);
  
  /**
   * Transform a state into another one from reference precomputed
   * values (physical data)associated to the given state
   */
  virtual void transformFromRef(const RealVector& data, Framework::State& result);
    
}; // end of class ICPNEQ2DPivtTvToCons

//////////////////////////////////////////////////////////////////////

    } // namespace ICP

  } // namespace Physics

} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////

#endif // COOLFluiD_Physics_ICP_ICPNEQ2DPivtTvToCons_hh
