#include "stdafx.h"
#include "mathutils.h"
#include "ode/common.h"
XRPHYSICS_API const float	phInfinity = dInfinity;
/*
#include "MathUtils.h"
enum EBoxSideNearestPointCode
{
	box_inside		,
	side_invisible	,
	on_side			,
	on_edge			,
	on_vertex
};
EBoxSideNearestPointCode GetNearestPointOnOBBSide(const Fmatrix &xform,const Fvector	&center,const Fvector &sides,u16 side,const Fvector &p,Fvector &point)
{
	//to plane dist
	const Fvector	&norm=xform[side];
	u16 side1=(side+1)%3,side2=(side+2)%3;
	float h=sides[side],h1=sides[side1],h2=sides[side2];
	//Fvector vdiffc;vdiffc.sub(center,p);
	float c_prg=norm.dotproduct(center);
	float p_prg=norm.dotproduct(p);
	float diffc=c_prg-p_prg;norm.dotproduct(vdiffc);
	float diffs;
	if(diffc<0.f)
	{
		diffs=diffc+h;
		point.set(norm);
		point.mul(diffs);
		point.add(p);
		Fvector d;d.sub(center,point);
		bool inside1 =XrMath::abs(d[side1])<h1;
		bool inside2 =XrMath::abs(d[side2])<h2;
		if(diffs>0.f)
		{
			if(inside1&&inside2) return box_inside;
			else return side_invisible;
		}
		else
		{
			if(inside1&&inside2) return on_side;
			else if(inside1)
			{
				float dd=h2-XrMath::abs(d[side2]);
				Fvector s;s.set(xform[side2]);s.
			}
		}
	}
	float diffs=diffc<0.f ? diffc+h	:	diffc-h;
}
*/
IC bool RAYvsCYLINDER(const Fcylinder& c_cylinder, const Fvector &S, const Fvector &D, float &R, BOOL bCull)
{

	const float &r=c_cylinder.m_radius;
	float h=c_cylinder.m_height/2.f;
	const Fvector& p=S;
	const Fvector& dir=D;
	
	const Fvector &c=c_cylinder.m_center;
	const Fvector &ax=c_cylinder.m_direction;
	//c.set(-IM.c.dotproduct(IM.i),-IM.c.dotproduct(IM.j),-IM.c.dotproduct(IM.k));
	//Fvector ax;ax.set(IM.i.z,IM.j.z,IM.k.z);//??

//////////////////////////////////////////////////////////////
	Fvector	v;	v		.sub		(c,p)			;
	float	cs	=dir	.dotproduct	(ax)			;
	float	Lc	=v		.dotproduct	(ax)			;
	float	Lr	=v		.dotproduct	(dir)			;
	////////////////////////////////////////////////
	float sq_cos=cs*cs;
	float sq_sin=1-sq_cos;
	float v_smag=v.square_magnitude();
	const float sq_r=r*r;

	if(sq_sin<XrMath::EPS)//paralel
	{
		float tr1,tr2								;
		float sq_dist=v_smag-Lr*Lr;//
		if(sq_dist>sq_r) return false;
		float r_dist=XrMath::sqrt(sq_r-sq_dist)+h;
		tr1=Lr-r_dist;
		
		if(tr1>R) return false;//
		if(tr1<0.f)
		{
			if(bCull)return false;
			else{
				tr2=Lr+r_dist;
				if(tr2<0.f) return false;//
				if(tr2<R)
				{
					R=tr2;
					return true;
				}
				return false;
			}
		}
		R=tr1;
		return true;
	}

	if(sq_cos<XrMath::EPS)
	{
		float tr1,tr2								;	
		//perp//
		float abs_c_dist=XrMath::abs(Lc);
		if(abs_c_dist>h+r)return false;
		float sq_dist=v_smag-Lr*Lr-Lc*Lc;
		if(sq_dist>sq_r) return false;
		float lc_h=abs_c_dist-h;
		if(lc_h>0.f)
		{
			float sq_sphere_dist=lc_h*lc_h+sq_dist*sq_dist;
			if(sq_sphere_dist>sq_r)return false;
			float diff=XrMath::sqrt(sq_r-sq_sphere_dist);
			tr1=Lr-diff;
			if(tr1>R) return false;//
			if(tr1<0.f)
			{
				if(bCull)return false;
				else{
					tr2=Lr+diff;
					if(tr2<0.f) return false;//
					if(tr2<R)
					{
						R=tr2;
						return true;
					}
					return false;
				}
			}
		}
		float diff=XrMath::sqrt(sq_r-sq_dist);
		tr1=Lr-diff;
		
		if(tr1>R) return false;//
		if(tr1<0.f)
		{
			if(bCull)return false;
			else{
				tr2=Lr+diff;
				if(tr2<0.f) return false;//
				if(tr2<R)
				{
					R=tr2;
					return true;
				}
				return false;
			}
		}
		R=tr1;
		return true;
	}
//////////////////////////////////////////////////
	float tr1,tr2							;
	float r_sq_sin	=1.f/sq_sin				;
	float tr		=(Lr-cs*Lc)*r_sq_sin	;
	float tc		=(cs*Lr-Lc)*r_sq_sin	;

	//more frequent separation - axes dist> radius
	//v^2+tc^2+tr^2-2*(cos*tc*tr-Lc*tc+Lr*tr)

	float sq_nearest_dist=v_smag+tr*tr+tc*tc-2*(cs*tc*tr-Lc*tc+Lr*tr);

	if(sq_nearest_dist>sq_r) return false;
	//float max_c_diff=//;

	float sq_horde=(sq_r-sq_nearest_dist)		;

	//float horde=XrMath::sqrt(sq_horde)					;
	float sq_c_diff=sq_horde*sq_cos*r_sq_sin	;
	float c_diff=XrMath::sqrt(sq_c_diff)				;//ccc
	float cp1=tc-c_diff							;
	float cp2=tc+c_diff							;
	
	
	//cp1<cp2 
	if(cp1>h)
	{
		//sphere 
		float tc_h=tc-h;//!! hi					(=)/;
		float sq_sphere_dist=sq_sin*tc_h*tc_h;
		if(sq_sphere_dist>sq_horde)return false;
		float tr_c=tr-tc_h*cs;//
		float diff=XrMath::sqrt(sq_horde-sq_sphere_dist);
		tr1=tr_c-diff;
		if(tr1>R) return false;//
		if(tr1<0.f)
		{
			if(bCull)return false;
			else{
				tr2=tr_c+diff;
				if(tr2<0.f) return false;//
				if(tr2<R){R=tr2;return true;}
			}
		}
		R=tr1												;
		return true											;
	} 

	if(cp2<-h)
	{
		//sphere lo								/(=)
		float tc_h=tc+h;//!!
		float sq_sphere_dist=sq_sin*tc_h*tc_h;
		if(sq_sphere_dist>sq_horde)return false;
		float tr_c=tr-tc_h*cs;//!!
		float diff=XrMath::sqrt(sq_horde-sq_sphere_dist);
		tr1=tr_c-diff;
		if(tr1>R) return false;//
		if(tr1<0.f)
		{
			if(bCull)return false;
			else{
				tr2=tr_c+diff;
				if(tr2<0.f) return false;//
				if(tr2<R){R=tr2;return true;}
			}
		}
		R=tr1												;
		return true											;
	} 
////////////////////////////////////////////////////////////////
	if(cs>0.f)
	{
		if(cp1 >-h)
		{
			if(cp2<h)
			{
				//cylinder							(=/=)
				float diff=c_diff/cs;
				tr1=tr-diff;
				if(tr1>R) return false;//
				if(tr1<0.f)
				{
					if(bCull)return false;
					else{
						tr2=tr+diff;
						if(tr2<0.f) return false;//
						if(tr2<R){R=tr2;return true;}
					}
				}
				R=tr1												;
				return true											;
			}
			else{
				//mixed//cyl hi sphere					(=/)
				float diff=c_diff/cs					;
				tr1=tr-diff								;
				if(tr1>R) return false;//
				if(tr1<0.f)
				{
					if(bCull)return false;
					else{
						float tc_h=tc-h								;
						float sq_sphere_dist=sq_sin*tc_h*tc_h;
						//if(sq_sphere_dist>sq_horde)return false	;
						float tr_c=tr-tc_h*cs						;
						float diff1=XrMath::sqrt(sq_horde-sq_sphere_dist)	;
						tr2=tr_c+diff1								;
						if(tr2<0.f) return false					;//
						if(tr2<R){R=tr2;return true;}
					}
				}
				R=tr1												;
				return true											;
			}
		}else//cp1<=-h
		{
			if(cp2<h)
			{
				//mixed//lo sphere	cyl						(/=)
				
				float tc_h=tc+h								;//(tc-(-h))
				float sq_sphere_dist=sq_sin*tc_h*tc_h;
				//if(sq_sphere_dist>sq_horde)return false;
				float diff=XrMath::sqrt(sq_horde-sq_sphere_dist)	;
				float tr_c=tr-tc_h*cs						;
				tr1=tr_c-diff								;
				if(tr1>R) return false						;//
				if(tr1<0.f)
				{
					if(bCull)return false;
					else{
						float diff1=c_diff/cs				;
						tr2=tr+diff1							;
						if(tr2<0.f) return false			;//
						if(tr2<R){R=tr2;return true;}
					}
				}
				R=tr1												;
				return true											;
			}else
			{
				//-(--)-								//sphere lo&&hi

				/////////////////////////////////////////////
				float tc_h=tc+h								;
				float tr_c=tr-tc_h*cs						;
				float diff=XrMath::sqrt(sq_horde-sq_sin*tc_h*tc_h)	;
				tr1=tr_c-diff								;
				if(tr1>R) return false						;//
				if(tr1<0.f)
				{
					if(bCull)return false;
					else{
						float tc_h1=tc-h								;
						float tr_c1=tr-tc_h1*cs						;
						float diff1=XrMath::sqrt(sq_horde-sq_sin*tc_h1*tc_h1)	;
						tr2=tr_c1+diff1							;
						if(tr2<R){R=tr2;return true;}
					}
				}
				R=tr1												;
				return true											;
			}

		}
	}
	else
	{
		if(cp1 >-h) 
		{
			if(cp2<h)
			{
				//cylinder
				float diff=-c_diff/cs;
				tr1=tr-diff;
				if(tr1>R) return false;//
				if(tr1<0.f)
				{
					if(bCull)return false;
					else{
						tr2=tr+diff;
						if(tr2<0.f) return false;//
						if(tr2<R){R=tr2;return true;}
					}
				}
				R=tr1							;
				return true						;
			}
			else{//cp1>-h&&cp2>h


				float tc_h=tc-h;			//hi sphere/cyl
				float tr_c=tr-tc_h*cs;
				float diff=XrMath::sqrt(sq_horde-sq_sin*tc_h*tc_h);
				tr1=tr_c-diff;
				if(tr1>R) return false						;//
				if(tr1<0.f)
				{
					if(bCull)return false;
					else{
						diff=-c_diff/cs;
						tr2=tr+diff;
						if(tr2<0.f) return false;//
						if(tr2<R){R=tr2;return true;}
					}
				}
				R=tr1							;
				return true						;
			}
		}else//cp1<-h
		{
			if(cp2<h)
			{
				//cyl/lo sphere
				float diff=-c_diff/cs;
				tr1=tr-diff;
				if(tr1>R) return false						;//
				if(tr1<0.f)
				{
					if(bCull)return false;
					else{
						//mixed//lo 
						float tc_h=tc+h;			
						float tr_c=tr-tc_h*cs;
						diff=XrMath::sqrt(sq_horde-sq_sin*tc_h*tc_h);
						tr2=tr_c+diff;
						if(tr2<0.f) return false;//
						if(tr2<R){R=tr2;return true;}
					}
				}
				R=tr1							;
				return true						;
			}else//cp2>=h
			{
				//-(--)-								//sphere hi&&lo

				float tc_h=tc-h								;
				float tr_c=tr-tc_h*cs						;
				float diff=XrMath::sqrt(sq_horde-sq_sin*tc_h*tc_h)	;
				tr1=tr_c-diff								;
				if(tr1>R) return false						;//
				/////////////////////////////////////////////
				if(tr1<0.f)
				{
					if(bCull)return false;
					else{
						tc_h=tc+h								;
						tr_c=tr-tc_h*cs							;
						diff=XrMath::sqrt(sq_horde-sq_sin*tc_h*tc_h)	;
						tr2=tr_c+diff							;
						if(tr2<0.f) return false				;//
						if(tr2<R){R=tr2;return true;}
					}
				}
				R=tr1							;
				return true						;
			}
		}
	}
}


void capped_cylinder_ray_collision_test()
{
	Fcylinder c;
	c.m_center.set(0,0,0);
	c.m_direction.set(0,0,1);
	c.m_height=2;
	c.m_radius=1;
	//ray
	Fvector dir1,pos;float R;
	dir1.set(1,0,0);pos.set(0,0,0);R=3;
	
	//inside
	RAYvsCYLINDER(c,pos,dir1,R,FALSE);//true , 1
	RAYvsCYLINDER(c,pos,dir1,R,TRUE);//false ,
	dir1.set(0,0,1);
	RAYvsCYLINDER(c,pos,dir1,R,FALSE);//true , 2
	RAYvsCYLINDER(c,pos,dir1,R,TRUE);//false

	//outside
	pos.set(-3,0,0);dir1.set(1,0,0);R=4;
	RAYvsCYLINDER(c,pos,dir1,R,FALSE);//true , 2
	RAYvsCYLINDER(c,pos,dir1,R,TRUE);//true , 2
	R=1;
	RAYvsCYLINDER(c,pos,dir1,R,FALSE);//false
	pos.set(0,0,-3);dir1.set(0,0,1);R=4;
	RAYvsCYLINDER(c,pos,dir1,R,FALSE);//true , 1
	RAYvsCYLINDER(c,pos,dir1,R,TRUE);//true, 1

	pos.set(-3,-3,-3);dir1.set(1,1,1);dir1.normalize();R=10;
	RAYvsCYLINDER(c,pos,dir1,R,TRUE);//true, ?
	float ir[2];
	Fcylinder::ecode code[2];
	c.intersect(pos,dir1,ir,code);
	//
	pos.set(0,0,0);
	RAYvsCYLINDER(c,pos,dir1,R,FALSE);//true, ?
	//Fcylinder::ecode code[2];
	c.intersect(pos,dir1,ir,code);
	RAYvsCYLINDER(c,pos,dir1,R,TRUE);//false
	BearTimer t;
	t.restart();
	for(int i=0;i<1000000;i++)
	{
		Fcylinder c1;
		c1.m_center.random_point(Fvector().set(2,2,2));
		c1.m_direction.random_dir();
		c1.m_height=Random.randF(0.2f,2.f);
		c1.m_radius=Random.randF(0.1f,2.f);
		//ray
		Fvector dir11,pos1;float R1=Random.randF(0.1f,2.f);
		dir11.random_dir();pos1.random_point(Fvector().set(2,2,2));
		RAYvsCYLINDER(c1,pos1,dir11,R1,TRUE);
	}
	Msg("my RAYvsCYLINDE time %f ms",t.get_elapsed_time().asseconds()*1000.f);
	t.restart();
	for(int i=0;i<1000000;i++)
	{
		Fcylinder c1;
		c1.m_center.random_point(Fvector().set(2,2,2));
		c1.m_direction.random_dir();
		c1.m_height=Random.randF(0.2f,2.f);
		c1.m_radius=Random.randF(0.1f,2.f);
		//ray
		Fvector dir11,pos1;//float R=Random.randF(0.1f,2.f);
		dir11.random_dir();pos1.random_point(Fvector().set(2,2,2));
		c.intersect(pos1,dir11,ir,code);
	}
		Msg("current intersect time %f ms",t.get_elapsed_time().asseconds()*1000.f);

}