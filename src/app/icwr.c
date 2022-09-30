#include "headfile.h"
typedef enum{
 tagnum=0,
 tag48=1,
 tag46,
 tag4d63,
}tag_eum;
/*static bool ic_write(tag_eum id ,void *tmp)
{
	if(tmp==NULL)
		return FALSE;
	 void *p=tmp;
  	if(id==tag48){
			p=(em4170_tag_t*)p;
		}
		  	if(id==tag46){
			p=(pcf7936_tag_t*)p;
		}
		  	if(id==tag4d63){
			p=(tms37145_tag_t*)p;
		}
	
}	*/
bool ic_copy(tag_eum tagid,uint8_t pgadr)
{
	uint8_t tmp=tagid,pgs=0,pgend=0;
	switch(tmp){
		case tag48:
			//if(!ic_write(tag48,&em4170_tag1))
				//return FALSE;
			break;
		case tag46:
			switch(pgadr){
				case(pages):{
				  pgs=0;
				  pgend=7;			  	 
			   }
			 break;
			 default:
				 return FALSE;
		   }
		if(!exc46_wr(&pcf7936_tag1,pgs,pgend,&pcf7936_tag1.a_page[0][0]))
				return FALSE;
			break;
		case tag4d63:
			//if(!ic_write(tag4d63,&tms37145_tag1))
				//return FALSE;
			break;
		default:
      return FALSE;			
	}
	return TRUE;
}

