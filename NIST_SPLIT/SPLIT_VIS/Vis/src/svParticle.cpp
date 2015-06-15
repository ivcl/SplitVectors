/** 
 *  svParticle
 */

#include "svParticle.h"
#include "svVectorField.h"

namespace __svl_lib {
svParticle & svParticle::operator=(const svParticle& par)
{  
    if(this == &par) 
      return *this;

    position = par.position;
    time = par.time;
    field = par.field;

    return *this;
};

void svParticle::Advect(svInt direction, svScalar advection_time)
{  
  field->Advect(*this, direction, advection_time);  
}

void svParticle::Step(svInt direction)
{
  field->Step(*this, direction);  
}

// not needed
void svParticle::Step(svInt direction, svScalar max_advection_time)
{
  field->advect->Step(*this, direction, max_advection_time);  
}
}
