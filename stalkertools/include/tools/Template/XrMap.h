#pragma once
template <typename K, class V, class P = std::less<K>, typename allocator = xalloc<std::pair<K, V> > > class xr_map : public std::map < K, V, P, allocator > { public: u32 size() const { return (u32)__super::size(); } };
template	<class _Ty1, class _Ty2> inline	std::pair<_Ty1, _Ty2>		mk_pair(_Ty1 _Val1, _Ty2 _Val2) { return (std::pair<_Ty1, _Ty2>(_Val1, _Val2)); }
#define DEF_MAP(N,K,T) typedef xr_map< K, T > N; typedef N::iterator N##_it;
#define DEFINE_MAP(K,T,N,I) typedef xr_map< K , T > N; typedef N::iterator I;
