#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CMonsterStateManagerAbstract CMonsterStateManager<_Object>

TEMPLATE_SPECIALIZATION
void CMonsterStateManagerAbstract::reinit()
{
	inherited::reinit();
}

TEMPLATE_SPECIALIZATION
void CMonsterStateManagerAbstract::update()
{
	this->execute();
}
TEMPLATE_SPECIALIZATION
void CMonsterStateManagerAbstract::force_script_state(EMonsterState state)
{
	// установить текущее состояние
	this->select_state(state);
}
TEMPLATE_SPECIALIZATION
void CMonsterStateManagerAbstract::execute_script_state()
{
	// выполнить текущее состояние
	inherited::get_state_current()->execute();
}

TEMPLATE_SPECIALIZATION
bool CMonsterStateManagerAbstract::can_eat()
{
	if (!inherited::object->CorpseMan.get_corpse()) return false;

	return check_state(eStateEat);
}

TEMPLATE_SPECIALIZATION
bool CMonsterStateManagerAbstract::check_state(u32 state_id) 
{
	if (inherited::prev_substate == state_id) {
		if (!inherited::get_state_current()->check_completion())		return true;
	} else {
		if (inherited::get_state(state_id)->check_start_conditions())	return true;
	}

	return false;
}

TEMPLATE_SPECIALIZATION
void CMonsterStateManagerAbstract::critical_finalize()
{
	inherited::critical_finalize();
}

TEMPLATE_SPECIALIZATION
EMonsterState CMonsterStateManagerAbstract::get_state_type() 
{
	return inherited::get_state_type();
}


#undef CMonsterStateManagerAbstract 
#undef TEMPLATE_SPECIALIZATION