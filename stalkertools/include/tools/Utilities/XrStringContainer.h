#pragma once
struct XrStringContainerValue
{
	bsize dwReference;
	bsize dwLength;
	u32 dwCRC;
	XrStringContainerValue* next;
	char value[];
};
namespace Impl
{
	struct XrStringContainer;
}
 class XRCORE_API XrStringContainer
{
	 BEAR_CLASS_STATIC(XrStringContainer);

	static xrCriticalSection *cs;
	static Impl::XrStringContainer* impl;
	friend class XRCORE_API XrCore;
	static void Initialize();
	static void Destroy();
public:

	static XrStringContainerValue* dock(const bchar* value);
	static void clean();
	static void dump();
	static void dump(IWriter* W);
	static void verify();
	static u32 stat_economy();
};
class shared_str
{
private:
	XrStringContainerValue* p_;
protected:
	// ref-counting
	IC void _dec() { if (0 == p_) return; p_->dwReference--; if (0 == p_->dwReference) p_ = 0; }
public:
	IC void _set(const bchar*  rhs) { XrStringContainerValue* v = XrStringContainer::dock(rhs); if (0 != v) v->dwReference++; _dec(); p_ = v; }
	IC void _set(shared_str const& rhs) { XrStringContainerValue* v = rhs.p_; if (0 != v) v->dwReference++; _dec(); p_ = v; }
	// void _set (shared_str const &rhs) { XrStringContainerValue* v = g_pStringContainer->dock(rhs.c_str()); if (0!=v) v->dwReference++; _dec(); p_ = v; }



	IC const XrStringContainerValue* _get() const { return p_; }
public:
	// construction
	IC shared_str() { p_ = 0; }
	IC shared_str(const bchar*  rhs) { p_ = 0; _set(rhs); }
	IC shared_str(shared_str const& rhs) { p_ = 0; _set(rhs); }
	IC ~shared_str() { _dec(); }

	// assignment & accessors
	IC shared_str& operator= (const bchar*  rhs) { _set(rhs); return (shared_str&)*this; }
	IC shared_str& operator= (shared_str const& rhs) { _set(rhs); return (shared_str&)*this; }
	IC const bchar*  operator* () const { return p_ ? p_->value : 0; }
	IC bool operator! () const { return p_ == 0; }
	IC char operator[] (size_t id) { return p_->value[id]; }
	IC const bchar*  c_str() const { return p_ ? p_->value : 0; }

	// misc func
	IC u32 size() const { if (0 == p_) return 0; else return p_->dwLength; }
	IC void swap(shared_str& rhs) { XrStringContainerValue* tmp = p_; p_ = rhs.p_; rhs.p_ = tmp; }
	IC bool equal(const shared_str& rhs) const { return (p_ == rhs.p_); }
	IC shared_str& __cdecl printf(const char* format, ...)
	{
		string4096 buf;
		va_list p;
		va_start(p, format);
		int vs_sz = _vsnprintf_s(buf, sizeof(buf) - 1, format, p);
		buf[sizeof(buf) - 1] = 0;
		va_end(p);
		if (vs_sz) _set(buf);
		return (shared_str&)*this;
	}
};