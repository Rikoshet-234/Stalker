#include "stdafx.h"
#pragma hdrstop
#include "physicsshell.h"
#include "PHDynamicData.h"
#include "Physics.h"
#include "PHJoint.h"
#include "PHShell.h"
#include "PHJoint.h"
#include "PHJointDestroyInfo.h"
#include "PHSplitedShell.h"

//#include "gameobject.h"
#include "iphysicsshellholder.h"

//#include "objectdump.h"
#include "phvalide.h"

#include "xrRender/Kinematics.h"
#include "engine/xr_object.h"
#include "engine/bone.h"

extern CPHWorld			*ph_world;
CPhysicsShell::~CPhysicsShell()
{
	
	//if(ph_world)ph_world->NetRelcase(this);
}

CPhysicsElement*			P_create_Element		()
{
	CPHElement* element=xr_new<CPHElement>	();
	return element;
}

CPhysicsShell*				P_create_Shell			()
{
	CPhysicsShell* shell=xr_new<CPHShell>	();
	return shell;
}

CPhysicsShell*				P_create_splited_Shell	()
{
	CPhysicsShell* shell=xr_new<CPHSplitedShell>	();
	return shell;
}

CPhysicsJoint*				P_create_Joint			( CPhysicsJoint::enumType type, CPhysicsElement* first, CPhysicsElement* second )
{
	CPhysicsJoint* joint=xr_new<CPHJoint>	( type , first, second );
	return joint;
}


CPhysicsShell*	__stdcall P_build_Shell			( IPhysicsShellHolder* obj, bool not_active_state, BONE_P_MAP* bone_map, bool not_set_bone_callbacks )
{
	DEBUGFATALERROR1( obj );
	phys_shell_verify_object_model( *obj );
	//IRenderVisual*	V = obj->ObjectVisual();
	//IKinematics* pKinematics=smart_cast<IKinematics*>(V);
	//IKinematics* pKinematics	=  V->dcast_PKinematics			();
	IKinematics* pKinematics	= obj->ObjectKinematics();

	CPhysicsShell* pPhysicsShell		= P_create_Shell();
#ifdef DEBUG
	pPhysicsShell->dbg_obj=obj;
#endif
	pPhysicsShell->build_FromKinematics(pKinematics,bone_map);

	pPhysicsShell->set_PhysicsRefObject( obj );
	pPhysicsShell->mXFORM.set( obj->ObjectXFORM() );
	pPhysicsShell->Activate( not_active_state, not_set_bone_callbacks );//,
	//m_pPhysicsShell->SmoothElementsInertia(0.3f);
	pPhysicsShell->SetAirResistance();//0.0014f,1.5f

	return pPhysicsShell;
}

void	fix_bones( LPCSTR	fixed_bones, CPhysicsShell* shell )
{
		DEBUGFATALERROR1(fixed_bones);
		DEBUGFATALERROR1(shell);
		IKinematics	*pKinematics = shell->PKinematics();
		DEBUGFATALERROR1(pKinematics);
		int count =					XrTrims::GetItemCount(fixed_bones);
		for (int i=0 ;i<count; ++i) 
		{
			string64					fixed_bone							;
			XrTrims::GetItem					(fixed_bones,i,fixed_bone)			;
			u16 fixed_bone_id=pKinematics->LL_BoneID(fixed_bone)			;
			R_ASSERT2(BI_NONE!=fixed_bone_id,"wrong fixed bone")			;
			CPhysicsElement* E = shell->get_Element(fixed_bone_id)			;
			if(E)
				E->Fix();
		}
}
CPhysicsShell*	P_build_Shell( IPhysicsShellHolder* obj, bool not_active_state,BONE_P_MAP* p_bone_map, LPCSTR	fixed_bones )
{
	CPhysicsShell* pPhysicsShell = 0;
	//IKinematics* pKinematics=smart_cast<IKinematics*>(obj->ObjectVisual());
	IKinematics* pKinematics=obj->ObjectKinematics();
	if(fixed_bones)
	{


		int count =					XrTrims::GetItemCount(fixed_bones);
		for (int i=0 ;i<count; ++i) 
		{
			string64					fixed_bone							;
			XrTrims::GetItem					(fixed_bones,i,fixed_bone)			;
			u16 fixed_bone_id=pKinematics->LL_BoneID(fixed_bone)			;
			R_ASSERT2(BI_NONE!=fixed_bone_id,"wrong fixed bone")			;
			p_bone_map->insert(mk_pair(fixed_bone_id,physicsBone()))			;
		}

		pPhysicsShell=P_build_Shell(obj,not_active_state,p_bone_map);

		//m_pPhysicsShell->add_Joint(P_create_Joint(CPhysicsJoint::enumType::full_control,0,fixed_element));
	}
	else
		pPhysicsShell=P_build_Shell(obj,not_active_state);


	BONE_P_PAIR_IT i=p_bone_map->begin(),e=p_bone_map->end();
	if(i!=e) pPhysicsShell->SetPrefereExactIntegration();
	for(;i!=e;i++)
	{
		CPhysicsElement* fixed_element=i->second.element;
		R_ASSERT2(fixed_element,"fixed bone has no physics");
		//if(!fixed_element) continue;
		fixed_element->Fix();
	}
	return pPhysicsShell;
}

CPhysicsShell*	P_build_Shell( IPhysicsShellHolder* obj, bool not_active_state, LPCSTR	fixed_bones )
{
	U16Vec f_bones;
	if(fixed_bones){
		//IKinematics* K		= smart_cast<IKinematics*>(obj->ObjectVisual());
		IKinematics* K		=obj->ObjectKinematics();
		DEBUGFATALERROR1( K );
		int count =			XrTrims::GetItemCount(fixed_bones);
		for (int i=0 ;i<count; ++i){
			string64		fixed_bone;
			XrTrims::GetItem		(fixed_bones,i,fixed_bone);
			f_bones.push_back(K->LL_BoneID(fixed_bone));
			R_ASSERT2(BI_NONE!=f_bones.back(),"wrong fixed bone")			;
		}
	}
	return P_build_Shell	(obj,not_active_state,f_bones);
}

static BONE_P_MAP bone_map=BONE_P_MAP();
CPhysicsShell*	P_build_Shell	( IPhysicsShellHolder* obj, bool not_active_state, U16Vec& fixed_bones )
{
	bone_map.clear			();
	CPhysicsShell*			pPhysicsShell = 0;
	if(!fixed_bones.empty())
		for ( U16It it=fixed_bones.begin(); it!=fixed_bones.end(); it++ )
			bone_map.insert( mk_pair( *it, physicsBone() ) );
	pPhysicsShell=P_build_Shell( obj, not_active_state, &bone_map );

	// fix bones
	BONE_P_PAIR_IT i=bone_map.begin(), e=bone_map.end();
	if( i!=e ) 
		pPhysicsShell->SetPrefereExactIntegration();
	for(;i!=e;i++)
	{
		CPhysicsElement* fixed_element=i->second.element;
		//R_ASSERT2(fixed_element,"fixed bone has no physics");
		if(!fixed_element) continue;
		fixed_element->Fix();
	}
	return pPhysicsShell;
}

CPhysicsShell*	P_build_SimpleShell( IPhysicsShellHolder* obj, float mass, bool not_active_state )
{
	CPhysicsShell* pPhysicsShell		= P_create_Shell();
#ifdef DEBUG
	pPhysicsShell->dbg_obj=(obj);
#endif
	//Fobb obb; obj->ObjectVisual()->getVisData().box.get_CD( obb.m_translate, obb.m_halfsize );
	DEBUGFATALERROR1( obj );
	DEBUGFATALERROR1( obj->ObjectKinematics() );

	Fobb obb; obj->ObjectKinematics()->GetBox().get_CD( obb.m_translate, obb.m_halfsize );
	obb.m_rotate.identity();
	CPhysicsElement* E = P_create_Element(); R_ASSERT( E ); E->add_Box( obb );
	pPhysicsShell->add_Element( E );
	pPhysicsShell->setMass( mass );
	pPhysicsShell->set_PhysicsRefObject( obj );
	if( !obj->has_parent_object() )
		pPhysicsShell->Activate( obj->ObjectXFORM(), 0, obj->ObjectXFORM(), not_active_state );
	return pPhysicsShell;
}

void ApplySpawnIniToPhysicShell( CInifile const * ini, CPhysicsShell* physics_shell, bool fixed )
{
		if(!ini)
			return;
		if(ini->section_exist("physics_common"))
		{
			fixed = fixed || (ini->line_exist("physics_common","fixed_bones")) ;
#pragma todo("not ignore static if non realy fixed! ")
			fix_bones(ini->r_string("physics_common","fixed_bones"),physics_shell);
		}
		if(ini->section_exist("collide"))
		{

			if((ini->line_exist("collide","ignore_static")&&fixed)||(ini->line_exist("collide","ignore_static")&&ini->section_exist("animated_object")))
			{
				physics_shell->SetIgnoreStatic();
			}
			if(ini->line_exist("collide","small_object"))
			{
				physics_shell->SetSmall();
			}
			if(ini->line_exist("collide","ignore_small_objects"))
			{
				physics_shell->SetIgnoreSmall();
			}
			if(ini->line_exist("collide","ignore_ragdoll"))
			{
				physics_shell->SetIgnoreRagDoll();
			}


			//If need, then show here that it is needed to ignore collisions with "animated_object"
			if (ini->line_exist("collide","ignore_animated_objects"))
			{
				physics_shell->SetIgnoreAnimated();
			}

		}
		//If next section is available then given "PhysicShell" is classified
		//as animated and we read options for his animation
		
		if (ini->section_exist("animated_object"))
		{
			//Show that given "PhysicShell" animated
			physics_shell->CreateShellAnimator( ini, "animated_object" );
		}
	
}



void	get_box( const CPhysicsBase*	shell, const	Fmatrix& form,	Fvector&	sz, Fvector&	c )
{
	t_get_box( shell, form, sz, c );
}




void __stdcall	destroy_physics_shell( CPhysicsShell* &p )
{
	if (p)
		p->Deactivate();
	xr_delete(p);
}

bool bone_has_pysics( IKinematics& K, u16 bone_id )
{
	
	//CBoneData	* pBonedata1 = &K.LL_GetData( bone_id );
	//CBoneData	* pBonedata2 = K.LL_GetBoneData( bone_id );
	
	//u32	sz = sizeof(vecBones);
	//u32	sz1=  sizeof(pBonedata1->children);

	//	DEBUGFATALERROR1(pBonedata1 == pBonedata2);
	return K.LL_GetBoneVisible( bone_id ) && shape_is_physic( K.GetBoneData( bone_id ).get_shape() );
}

bool has_physics_collision_shapes( IKinematics& K )
{
	u16 nbb = K.LL_BoneCount();
	for(u16 i = 0; i < nbb; ++i )
		if( bone_has_pysics( K, i ) )
			return true;
	return false;
}

void	phys_shell_verify_model( IKinematics& K )
{
	//IRenderVisual* V = K.dcast_RenderVisual();
	//DEBUGFATALERROR1( V );
	DEBUGFATALERROR12( has_physics_collision_shapes( K ), make_string( "Can not create physics shell for model %s because it has no physics collision shapes set", K.getDebugName().c_str() ) );
}

void	phys_shell_verify_object_model( IPhysicsShellHolder& O )	
{
	//IRenderVisual	*V = O.ObjectVisual();

	//DEBUGFATALERROR12( V, make_string( "Can not create physics shell for object %s it has no model", O.ObjectName() )/*+ make_string("\n object dump: \n") + dbg_object_full_dump_string( &O )*/ );

	//IKinematics		*K = V->dcast_PKinematics();

	IKinematics* K		=O.ObjectKinematics();
		
	DEBUGFATALERROR12( K, make_string( "Can not create physics shell for object %s, model %s is not skeleton", O.ObjectName(), O.ObjectNameVisual() ) );

	DEBUGFATALERROR12( has_physics_collision_shapes( *K ), make_string( "Can not create physics shell for object %s, model %s has no physics collision shapes set", O.ObjectName(), O.ObjectNameVisual() )/*+ make_string("\n object dump: \n") + dbg_object_full_dump_string( &O )*/  );

	DEBUGFATALERROR12( _valid( O.ObjectXFORM() ), make_string( "create physics shell: object matrix is not valid" ) /*+ make_string("\n object dump: \n") + dbg_object_full_dump_string( &O )*/ );

	DEBUGFATALERROR12(valid_pos( O.ObjectXFORM().c ),  dbg_valide_pos_string( O.ObjectXFORM().c, &O, "create physics shell" ) );
}

bool __stdcall	can_create_phys_shell( string1024 &reason, IPhysicsShellHolder& O )
{
	xr_strcpy(reason, "ok" );
	bool result = true;
	IKinematics* K		=O.ObjectKinematics();
	if(!K)
	{
		xr_strcpy( reason,	make_string( "Can not create physics shell for object %s, model %s is not skeleton", O.ObjectName(), O.ObjectNameVisual() ).c_str() );
		return false;
	}
	if(!has_physics_collision_shapes( *K ))
	{
		xr_strcpy( reason,	make_string( "Can not create physics shell for object %s, model %s has no physics collision shapes set", O.ObjectName(), O.ObjectNameVisual() ).c_str() );
		return false;
	}
	if(!_valid( O.ObjectXFORM() ))
	{
		xr_strcpy( reason, make_string( "create physics shell: object matrix is not valid" ).c_str() );
		return false;
	}
	if(!valid_pos( O.ObjectXFORM().c ))
	{
#ifdef	DEBUG
		xr_strcpy( reason, dbg_valide_pos_string( O.ObjectXFORM().c, &O, "create physics shell" ).c_str() );
#else
		xr_strcpy( reason, make_string( "create physics shell: object position is not valid" ).c_str() );
#endif
		return false;
	}
	return result;
}





float NonElasticCollisionEnergy( CPhysicsElement *e1, CPhysicsElement *e2, const Fvector &norm)// norm - from 2 to 1
{
	DEBUGFATALERROR1( e1 );
	DEBUGFATALERROR1( e2 );
	dBodyID b1 = static_cast<CPHElement*> (e1)->get_body();
	DEBUGFATALERROR1(b1);
	dBodyID b2 = static_cast<CPHElement*> (e2)->get_body();
	DEBUGFATALERROR1(b2);
	return E_NL( b1, b2, cast_fp( norm ) );
}


void	StaticEnvironmentCB ( bool& do_colide, bool bo1, dContact& c, SGameMtl* material_1, SGameMtl* material_2 )
{
	dJointID contact_joint	= dJointCreateContact(0, ContactGroup, &c);

	if(bo1)
	{
		((CPHIsland*)(retrieveGeomUserData(c.geom.g1)->callback_data))->DActiveIsland()->ConnectJoint(contact_joint);
		dJointAttach			(contact_joint, dGeomGetBody(c.geom.g1), 0);
	}
	else
	{
		((CPHIsland*)(retrieveGeomUserData(c.geom.g2)->callback_data))->DActiveIsland()->ConnectJoint(contact_joint);
		dJointAttach			(contact_joint, 0, dGeomGetBody(c.geom.g2));
	}
	do_colide=false;
}