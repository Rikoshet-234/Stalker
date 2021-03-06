#pragma once
class XRayEnvironmentRender :public IEnvironmentRender
{
public:
	XRayEnvironmentRender();
	virtual void Copy(IEnvironmentRender &_in);
	virtual void OnFrame(EnvironmentRef *env);
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void RenderSky(EnvironmentRef *env);
	virtual void RenderClouds(EnvironmentRef *env);
	virtual void OnDeviceCreate();
	virtual void OnDeviceDestroy();
	virtual particles_systems::library_interface const& particles_systems_library();
};