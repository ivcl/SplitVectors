/**
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "svTimeLine.h"
#include "svException.h"

//Constructor/Destructor
svTimeLine::svTimeLine(svVectorField* f, svParticle* sd, svInt sd_num, svUint mode)
  : svPrimitive(f, sd, sd_num, mode)
{
	timeline_num = DEFAULT_TIMELINE_MAX_NUM;
	timeline_interval = DEFAULT_TIMELINE_INTERVAL;	
	timeline = NULL;
}

svTimeLine::~svTimeLine()
{
  if (timeline != NULL) {
	for (int i=0; i<seed_num; i++)  
		timeline[i].free();
	delete [] timeline;
  }
}

//Methods

void svTimeLine::Generate()
{
  int i, j, k;
  svVector3 pos;

  svParticle *particle;
  particle = new svParticle[seed_num];

  // create working seed particles
  for(k=0; k<seed_num; k++){
    particle[k] = seed[k];
  }
  
  if (display_mode == SV_STORE) {
    timeline = new svVector3Array[timeline_num];
  }
  else if (display_mode == SV_DISPLAYLIST && !glIsList(display_list)){
	display_list = glGenLists(1);
	glNewList(display_list, GL_COMPILE);
  }
 
  //draw the timeline composed of the seeds
  glBegin(GL_LINE_STRIP);
  for(k=0; k<seed_num; k++)	{
	pos=particle[k].position;	
	if (display_mode == SV_IMMEDIATE || display_mode == SV_DISPLAYLIST)
	  glVertex3f(pos[0], pos[1], pos[2]);
    else  
	  timeline[0].add(pos);
  }
  glEnd();

  for (i=1; i<timeline_num; i++)
  {      
    glBegin(GL_LINE_STRIP);

    for(j=0; j<seed_num; j++)
	{
      try {
	    particle[j].Advect(direction, timeline_interval);
      }
      catch(int e){
		if (e == SV_EXP_STATIC_REGION	   || 
		    e == SV_EXP_OUT_OF_RANGE_SPACE || 
		    e == SV_EXP_OUT_OF_RANGE_TIME)  break;  else throw e;
      }
	  
	  pos=particle[j].position;	

	  if (display_mode == SV_IMMEDIATE || display_mode == SV_DISPLAYLIST)
		glVertex3f(pos[0], pos[1], pos[2]);
	  else  
		timeline[i].add(pos);

    } //loop of seeds
    
    glEnd();
      
  } //loop of timeline number
  
  if (display_mode == SV_DISPLAYLIST)  glEndList();

  delete [] particle;

}

void svTimeLine::Render()
{
  glColor3fv(render_property.color.getValue());
  
  if (display_mode == SV_IMMEDIATE)
    Generate();	
  else if(display_mode == SV_DISPLAYLIST && glIsList(display_list))
    glCallList(display_list);
  else if (display_mode == SV_STORE)
	RenderStore();
}

void svTimeLine::RenderStore() const
{
  for (int i=0; i<timeline_num; i++) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof (svVector3), &timeline[i][0]);
    glDrawArrays(GL_LINE_STRIP, 0, timeline[i].size());
	glDisableClientState(GL_VERTEX_ARRAY);
  }	
}
