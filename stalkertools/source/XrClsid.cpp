#include "stdafx.h"

void XrClsid::Clsid2String(CLASS_ID id, bchar * text)
{
	text[8] = 0;
	for (int i = 7; i >= 0; i--) { text[i] = char(id & 0xff); id >>= 8; }
}

CLASS_ID XrClsid::String2Clsid(const bchar * text)
{
	BEAR_ASSERT(BearString::GetSize(text) <= 8);
	char buf[9];
	buf[8] = 0;
	BearString::CopyWithSizeLimit(buf, text, 8);
	size_t need = 8 - BearString::GetSize(buf);
	while (need) { buf[8 - need] = ' '; need--; }
	return MK_CLSID(buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
}
