/*
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "svAnalyticField.h"


namespace __svl_lib {



svAnalyticField::svAnalyticField() 
{  
  max_varing_time = 0.0;  
  freeze_time = -1.0;
  flow_type = SV_STEADY;  
}

svAnalyticField::svAnalyticField(svVector3 _lbbox, svVector3 _rbbox, svScalar _mt)
{  
  lbbox = _lbbox;
  rbbox = _rbbox;
  max_varing_time = _mt;  
  freeze_time = -1.0;
  flow_type = _mt > 0.0 ? SV_STEADY : SV_UNSTEADY;
}


}
