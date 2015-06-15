/**
 *  svTimeLine 
 */

#ifndef __SV_TIMELINE_H
#define __SV_TIMELINE_H

#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svArray.h"
using namespace __svl_lib;

#define DEFAULT_TIMELINE_MAX_NUM  6
#define DEFAULT_TIMELINE_INTERVAL 2.5f

class svTimeLine : public svPrimitive
{
  public:
	  svTimeLine(svVectorField* fld, svParticle* sd, svInt sd_num, svUint mode=SV_DISPLAYLIST);
	  virtual ~svTimeLine();

	  virtual void Render();
	  virtual void Generate();

	  svInt GetTimeLineNum() const
		{ return timeline_num; }

	  void SetTimeLineNum(svInt tl_num) 
		{ timeline_num = tl_num; }

	  svScalar GetTimeLineInterval() const
		{ return timeline_interval; }

	  void SetTimeLineInterval(svScalar tl_interval) 
		{ timeline_interval = tl_interval; }

  private:
	  void RenderStore() const;

  protected:
	svInt timeline_num;			// how many timelines to draw
	svScalar timeline_interval; // how much time interval to draw a timeline
	svVector3Array *timeline;	// store the particle position on the time lines
};

#endif // __SV_TIMELINE_H
