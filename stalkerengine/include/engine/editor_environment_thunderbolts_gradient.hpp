////////////////////////////////////////////////////////////////////////////
// Module : editor_environment_thunderbolts_gradient.hpp
// Created : 04.01.2008
// Modified : 10.01.2008
// Author : Dmitriy Iassenev
// Description : editor environment thunderbolts gradient class
////////////////////////////////////////////////////////////////////////////

#ifndef EDITOR_WEATHER_THUNDERBOLTS_GRADIENT_HPP_INCLUDED
#define EDITOR_WEATHER_THUNDERBOLTS_GRADIENT_HPP_INCLUDED

#ifdef INGAME_EDITOR

#include <boost/noncopyable.hpp>
#include "../editor/property_holder.hpp"
#include "thunderbolt.h"

namespace editor
{

class property_holder;

namespace environment
{

class manager;

namespace thunderbolts
{

class gradient :
    public SThunderboltDesc::SFlare,
    private boost::noncopyable
{
public:
    gradient();
    ~gradient();
    void load(CInifile& config, shared_str const& section_id, LPCSTR prefix);
    void save(CInifile& config, shared_str const& section_id, LPCSTR prefix);
    void fill(
        ::editor::environment::manager& environment,
        LPCSTR name,
        LPCSTR description,
        editor::property_holder& holder
    );

private:
    LPCSTR  shader_getter() const;
    void  shader_setter(LPCSTR value);

    LPCSTR  texture_getter() const;
    void  texture_setter(LPCSTR value);

private:
    property_holder* m_property_holder;
}; // class gradient


} // namespace thunderbolts
} // namespace environment
} // namespace editor

#endif // #ifdef INGAME_EDITOR

#endif // ifndef EDITOR_WEATHER_THUNDERBOLTS_GRADIENT_HPP_INCLUDED