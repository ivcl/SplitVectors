/*  svVectorField
 */

#ifndef __SV_VECTORFIELD_H
#define __SV_VECTORFIELD_H

#include "svType.h"
#include "svParticle.h"
#include "svArray.h"

using namespace std;

namespace __svl_lib {

typedef enum {
  SV_STEADY   = 0,
  SV_UNSTEADY = 1,
} svFlowType;

class svVectorField {
 public:
  svVectorField();
  svVectorField(svVector3 lb, svVector3 rb); // set the bounding box
  svVectorField(const svVectorField& vef) {*this=vef;}
  svVectorField & operator=(const svVectorField & vef);

  virtual ~svVectorField();
  
  virtual void CalculateRoughStrength(int num_seeds);
  virtual svVector3 GetVector(const svParticle & p)=0;
  virtual svVector3 GetVector(const svVector3& pos, svScalar t)=0;

  virtual void Update() = 0;
  virtual void Advect(svParticle& p, svInt direction, 
                      svScalar advection_time);
  virtual int Step(svParticle& p, svInt direction);
  virtual int Step(svParticle& p, svInt direction, svFloat max_advection_time);

  svBool pointOutOfBound(svVector3 p) const 
  {
    if((p[0] < lbbox[0] && p[0]> rbbox[0]) && 
       (p[1] < lbbox[1] && p[1]> rbbox[1]) && 
       (p[2] < lbbox[2] && p[2]> rbbox[2])) {
      //cerr << "svVectorField.h: pointOutOfBound true" << endl;
      //lbbox.print();
      //rbbox.print();
    return true;
    };
      //cerr << "svVectorField.h: pointOutOfBound false" << endl;
      //lbbox.print();
      //rbbox.print();
      return false;
  }

  virtual void Print()
  {
    cerr << "-------------- svVectorField Print() ------------------" << endl;
    cerr << "Bounding box:  (" << lbbox[0]<<", " << lbbox[1]<<", " << lbbox[2]<<"); ("<< rbbox[0] <<", "<<rbbox[1]<<", " << rbbox[2]<<")" << endl;
    cerr << "=-----------end of svVectorField Print() -------------=" << endl;
  }
  
  // for feature extraction
  virtual void jacobianMatrix(double x, double y, double z, svMatrix3 &jac){};

  // Get and Set
  // XXX: this is ambiguious
  virtual void GetPhysicalDimension(svScalar* sx, svScalar* sy, svScalar* sz) const
  {  
     *sx = rbbox[0]-lbbox[0]; *sy = rbbox[1]-lbbox[1]; 
     *sz = rbbox[2]-lbbox[2]; 
  };
  virtual void GetPhysicalDimension(svVector3 * lb, svVector3 * rb) const 
  {
      *lb= lbbox; *rb= rbbox;
  };

  virtual void SetPhysicalDimension(svVector3  _lbbox,
				    svVector3  _rbbox)
  {lbbox=_lbbox; rbbox = _rbbox;  }

  virtual svVector3 GetCenter() const
  { return ((lbbox+rbbox)*0.5); }

  virtual svVector3 GetBoundingBoxSize() const
  { return (rbbox - lbbox);};

  virtual void GetBoundingBox(svVector3 *lb, svVector3 *rb) const
  { 
    GetPhysicalDimension(lb, rb);
 // *lb = lbbox; *rb = rbbox; 
  };

  virtual void GetBoundingBox(svScalar *minx,svScalar *maxx, 
                      svScalar *miny,svScalar *maxy,
                      svScalar *minz,svScalar *maxz) const
  {*minx=lbbox[0];*maxx=rbbox[0];*miny=lbbox[1];*maxy=rbbox[1];
   *minz=lbbox[2];*maxz=rbbox[2];};

  svBool TypeOf(svFlowType ft) 
    {  return ft == flow_type; }

  virtual svUint GetDimNum() const
    {  return dimension;  }

  virtual void SetDimNum(svUint d)
    {  dimension = d;  }

  svUint GetTimeStepNum() const
    {  return time_step_num;  }

  virtual void SetTimeStepNum(svUint tsn)
    {  time_step_num = tsn;  }

  svUint GetTimeStepNumInCore() const
    {  return time_step_num_in_core;  }

  void SetTimeStepNumInCore(svUint tsn)
    {  time_step_num_in_core = tsn;  }

  svUint GetTimeStepStart() const
    {  return time_step_start;  }

  void SetTimeStepStart(svUint start)
    {  time_step_start = start;  }

  svScalar GetTimeStepSpan() const
    {  return time_step_span;  }

  void SetTimeStepSpan(svScalar span)
    {  time_step_span = span;  }

  svScalar GetFreezeTime() const
    {  return freeze_time;  }

  void SetFreezeTime(svScalar freeze)
    {  freeze_time = freeze;  }

  svScalar GetMaxVaringTime()
    {  return max_varing_time;  }

  void SetMaxVaringTime(svScalar t)
    {  max_varing_time = t;  }

  svScalar GetMaxStrength()
    {  return max_strength; }
  svScalar GetMinStrength()
    {  return min_strength; }

 protected:

  /* general info for the flow volume */

  svFlowType flow_type;            // the flow volume type: steady or unsteady
  svUint     dimension;            // the dimension of field, 2, 3, 4, ...
  svScalar longest_side;           // the longest side edge of field
  svScalar freeze_time;            // the frozen time stamp for the unstead flow
  svScalar max_strength; 
  svScalar min_strength;

  /* info for time varing field */

  svScalar max_varing_time;        // the max varing time of the field
  svUint time_step_num;            // the total time steps of the field
  svUint time_step_num_in_core;    // the time steps in the memory
  svUint time_step_start;          // the start time step in the memory
  svScalar time_step_span;         // the physical span between two time step
  svUint load_num;                 // the number of volumes for each loading

  svVector3 lbbox, rbbox; //bounding box of the field

 public:

  /* field advection information */

  struct svAdvect {
    svVectorField *field;
	svScalar delta_t;
	svScalar min_delta_t;
	svScalar max_delta_t;
	svScalar error_tolerance;

	inline svScalar GetMinDelta_t() const;
	inline void SetMinDelta_t(svScalar t);
	inline svScalar GetMaxDelta_t() const;
	inline void SetMaxDelta_t(svScalar t);
	inline svScalar GetDelta_t() const;
	inline void SetDelta_t(svScalar t);

	svAdvect(svVectorField *f);
	svScalar Step(svParticle & p, svInt direction, svScalar max_step_time=-1);
	void Advect(svParticle & p, svInt direction, svScalar advection_time);

   private:
	svVector3 RungeKutta4(const svVector3& pos, svScalar t, svInt dir, 
						  svScalar step_t) const;
  } *advect;
  friend struct svAdvect;
};

typedef svVectorField::svAdvect svAdvect;
}
#endif // __SV_VECTORFIELD_H
