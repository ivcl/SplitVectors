/**
 *  This file implements the class: svVectorField and svAdvect
 */

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "svException.h"
#include "svVectorField.h"
using namespace std;

//#define DEFAULT_DELTA_T      0.01
#define DEFAULT_DELTA_T      0.001
#define DEFAULT_MIN_DELTA_T  0.001
#define DEFAULT_MAX_DELTA_T  0.08
#define DEFAULT_ERROR_TOLERANCE  0.0001

#define DEFAULT_MIN_BBOX  999
#define DEFAULT_MAX_BBOX  -999
namespace __svl_lib {
//
// svAdvect Methods
//
svAdvect::svAdvect(svVectorField *f)
{
  field        = f;
  delta_t      = svScalar(DEFAULT_DELTA_T);
  min_delta_t  = svScalar(DEFAULT_MIN_DELTA_T);
  max_delta_t  = svScalar(DEFAULT_MAX_DELTA_T);
  error_tolerance = svScalar(DEFAULT_ERROR_TOLERANCE);
}

svScalar svAdvect::Step(svParticle& p, svInt direction, 
                        svScalar max_step_time)
{
  svVector3 full_step, half_step1, half_step2;
  svVector3 new_pos;
  svVector3 pos = p.position;
  svScalar  t = p.time;
  svInt     dir = direction;

  svScalar longest_side = field->longest_side;

  // calculate some fabsolute values
  svScalar fabs_error_tolerance = longest_side * error_tolerance;

  svScalar fabs_delta_t, fabs_min_delta_t, fabs_max_delta_t;

  if (field->TypeOf(SV_STEADY) || field->freeze_time>0.0) {
	fabs_delta_t         = longest_side * delta_t;
	fabs_min_delta_t     = longest_side * min_delta_t;
	fabs_max_delta_t     = longest_side * max_delta_t;
  }
  else{
	fabs_delta_t         = delta_t;
	fabs_min_delta_t     = min_delta_t;
	fabs_max_delta_t     = max_delta_t;
  }

  // calculate the working delta time
  svScalar work_delta_t = fabs_delta_t;  

  //cerr << "-->work_delta_t="<<work_delta_t<<endl;
  if (max_step_time >0){
	work_delta_t = fabs_delta_t>max_step_time ? max_step_time : fabs_delta_t;
  }
  
  while(true) {
    //take one full step
    full_step = RungeKutta4(pos, t, dir, work_delta_t);

    // take two half steps
    half_step1 = RungeKutta4(pos, t, dir, work_delta_t/2.0);
    half_step2 = RungeKutta4(half_step1, t+work_delta_t/2.0, dir, work_delta_t/2.0);

    svScalar error = (half_step2-full_step).length();

    if (error > fabs_error_tolerance){
      work_delta_t /= 2.1;
      if (fabs_delta_t<max_step_time)  delta_t /= 2.1;
      if (delta_t < min_delta_t)  delta_t = min_delta_t;
    }
    else{
      if (fabs_delta_t<max_step_time)  delta_t *= 2.0;
      break;
    }
  }

  p.SetPosition(half_step2);
  p.SetTime(t + work_delta_t);
  //cerr << "half_step2"; p.GetPosition().print();
  //cerr << "work_delta_t = " << work_delta_t << " " << t+work_delta_t << endl;

  return work_delta_t;
}

void svAdvect::Advect(svParticle& p, svInt direction, svScalar advection_time) 
{
  svScalar end_time = p.time + advection_time;
  while (p.time < end_time) {
     svScalar step_time;
     try {
        step_time = Step(p, direction, advection_time);
     } catch (svInt e) {
       if(e==SV_EXP_OUT_OF_RANGE_SPACE) {
        //cerr << "svAdvect: advect throw out of space.. " << endl;
        throw e;
       };
     };
     advection_time -= step_time; 
  }; // check p.time
}

svVector3 svAdvect::RungeKutta4(const svVector3& pos, svScalar t, 
                                svInt dir, svScalar step_t) const
{
  svVector3 k1, k2, k3, k4, out;
  svScalar work_step_t      = dir * step_t;
  svScalar work_half_step_t = work_step_t * 0.5;

  k1=field->GetVector(pos,t);
  if(k1 == svVector3(0.0)) {
     //cerr << "**** stagnation point" << endl;
     return svVector3(0,0,0);
     //throw SV_EXP_STATIC_REGION;
  }

  k2 = field->GetVector(pos + k1 * work_half_step_t, t+work_half_step_t);
 // cout << "k2= " << k2[0] << ", " << k2[1] << ", " << k2[2]<< endl;
  k3 = field->GetVector(pos + k2 * work_half_step_t, t+work_half_step_t);
 // cout << "k3= " << k3[0] << ", " << k3[1] << ", " << k3[2] << endl;
  k4 = field->GetVector(pos + k3 * work_step_t, t+work_step_t);
 // cout << "k4= " << k4[0] << ", " << k4[1] << ", " << k4[2]  << endl;

  out = pos + (k1/6.0 + k2/3.0 + k3/3.0 + k4/6.0) * work_step_t;
  //cout << "out= " << out[0] << ", " << out[1] << ", " << out[2]  << endl;
  //test if "out" is inside the field, otherwise execption is thrown
  //field->GetVector(out, t+step_t*dir);

  return out;
}


inline svScalar svAdvect::GetMinDelta_t() const
{
	return min_delta_t;
}

inline void svAdvect::SetMinDelta_t(svScalar t)
{
	min_delta_t = t;
}

inline svScalar svAdvect::GetMaxDelta_t() const
{
	return max_delta_t;
}

inline void svAdvect::SetMaxDelta_t(svScalar t)
{
	max_delta_t = t;
}

inline svScalar svAdvect::GetDelta_t() const
{
	return delta_t;
}

inline void svAdvect::SetDelta_t(svScalar t)
{
	delta_t = t;
}

//
// svVectorField Methods
//
svVectorField::svVectorField() 
{
  lbbox = svVector3(DEFAULT_MIN_BBOX,DEFAULT_MIN_BBOX,DEFAULT_MIN_BBOX);
  rbbox = svVector3(DEFAULT_MAX_BBOX,DEFAULT_MAX_BBOX,DEFAULT_MAX_BBOX);

  flow_type   = SV_STEADY; 
  freeze_time = -1.0;
  advect = new svAdvect(this); 

  max_strength = 0;
  min_strength = 9999;
}

svVectorField & svVectorField::operator=(const svVectorField & vef)
{
    if(this == &vef)
        return *this;

    flow_type = vef.flow_type;
    dimension = vef.dimension;
    lbbox = vef.lbbox;
    rbbox = vef.rbbox;
    flow_type = vef.flow_type;
    advect = vef.advect;
	return *this;
}

svVectorField::~svVectorField()
{
  if (advect != NULL)	delete advect;
}

void svVectorField::CalculateRoughStrength(int num_seeds)
{
    //int max_adv_time = 100;
    // random pick some points
    for(int i=0; i<num_seeds; i++)
    {
      svVector3 pos;
      pos[0] = float(rand())/float(RAND_MAX)*2.-1.f; 
      pos[1] = float(rand())/float(RAND_MAX)*2.-1.f; 
      pos[2] = float(rand())/float(RAND_MAX)*2.-1.f; 

      svParticle particle;

      particle.position = pos;
      particle.time = 0;

      svVector3 v = GetVector(particle);
      svScalar s = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
      if(s>svVectorField::max_strength) max_strength = s;
      if(s<svVectorField::min_strength) min_strength = s;
    }
}

void svVectorField::Advect(svParticle& p, svInt direction, 
                           svScalar advection_time)
{
  advect->Advect(p, direction, advection_time);  
}

int svVectorField::Step(svParticle& p, svInt direction )
{
    advect->Step(p, direction);  
	return 1;
}

// not needed
int svVectorField::Step(svParticle& p, svInt direction,
                        svFloat max_advection_time)
{
    advect->Step(p, direction, max_advection_time);  
	return 1;
}
}
