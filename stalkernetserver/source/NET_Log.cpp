#include "stdafx.h"
#include "net_log.h"
//---------------------------------------------------------
string64 PacketName[] = {
	"M_UPDATE",	// DUAL: Update state
	"M_SPAWN",					// DUAL: Spawning, full state

	"M_SV_CONFIG_NEW_CLIENT",
	"M_SV_CONFIG_GAME",
	"M_SV_CONFIG_FINISHED",

	"M_MIGRATE_DEACTIVATE",		// TO:   Changing server, just deactivate
	"M_MIGRATE_ACTIVATE",			// TO:   Changing server", full state

	"M_CHAT",						// DUAL:

	"M_EVENT",					// Game Event
	"M_CL_INPUT",					// Client Input Data
	//----------- for E3 -----------------------------
	"M_CL_UPDATE",
	"M_UPDATE_OBJECTS",
	//-------------------------------------------------
	"M_CLIENTREADY",				// Client has finished to load level and are ready to play

	"M_CHANGE_LEVEL",				// changing level
	"M_LOAD_GAME",
	"M_RELOAD_GAME",
	"M_SAVE_GAME",
	"M_SAVE_PACKET",

	"M_SWITCH_DISTANCE",
	"M_GAMEMESSAGE",					// Game Message
	"M_EVENT_PACK",					// Pack of M_EVENT

	//-----------------------------------------------------
	"M_GAMESPY_CDKEY_VALIDATION_CHALLENGE",
	"M_GAMESPY_CDKEY_VALIDATION_CHALLENGE_RESPOND",
	"M_CLIENT_CONNECT_RESULT",
	"M_CLIENT_REQUEST_CONNECTION_DATA",

	"M_CHAT_MESSAGE",
	"M_CHANGE_LEVEL_GAME",
	//-----------------------------------------------------
	"M_CL_PING_CHALLENGE",
	"M_CL_PING_CHALLENGE_RESPOND",
	//-----------------------------------------------------
	"M_PAUSE_GAME",
	//-----------------------------------------------------
	"M_AUTH_CHALLENGE",
	"M_CL_AUTH",
	"M_BULLET_CHECK_RESPOND",
	//-----------------------------------------------------
	"M_STATISTIC_UPDATE",
	"M_STATISTIC_UPDATE_RESPOND",
	//-----------------------------------------------------
	"M_PLAYER_FIRE",
	//-----------------------------------------------------
	"M_MOVE_PLAYERS",
	"M_MOVE_PLAYERS_RESPOND",

	"MSG_FORCEDWORD"
};
//---------------------------------------------------------
INetLog::INetLog(LPCSTR sFileName, u32 dwStartTime)
#ifdef PROFILE_CRITICAL_SECTIONS
	:m_cs(MUTEX_PROFILE_ID(NET_Log))
#endif // PROFILE_CRITICAL_SECTIONS
{
	xr_strcpy(m_cFileName, sFileName);

	if(!m_pLogFile.Open(sFileName, m_pLogFile.M_Write))m_cFileName[0]=0;
	m_dwStartTime = 0;//dwStartTime;

}

INetLog::~INetLog() 
{
	FlushLog();
}

void	INetLog::FlushLog()
{
	if (m_cFileName[0])
	{
		for(xr_vector<SLogPacket>::iterator it = m_aLogPackets.begin(); it != m_aLogPackets.end(); it++)
		{
			SLogPacket* pLPacket = &(*it);
			BearString str;
			if (pLPacket->m_u16Type >= sizeof(PacketName)/sizeof(PacketName[0]))
				str.assign_printf( "%s %10u %10u %10u\n", pLPacket->m_bIsIn ? "In:" : "Out:", pLPacket->m_u32Time, pLPacket->m_u16Type, pLPacket->m_u32Size);
			else
				str.assign_printf("%s %10u %10s %10u\n", pLPacket->m_bIsIn ? "In:" : "Out:", pLPacket->m_u32Time, PacketName[pLPacket->m_u16Type], pLPacket->m_u32Size);
			m_pLogFile.WriteString(str, BearEncoding::UTF8, false);
		};

	};

	m_aLogPackets.clear();
}

void		INetLog::LogPacket(u32 Time, NET_Packet* pPacket, bool IsIn)
{
	if (!pPacket) return;

	m_cs.Enter();
	
	SLogPacket NewPacket;
	
	NewPacket.m_u16Type = *((u16*)&pPacket->B.data);
	NewPacket.m_u32Size = static_cast<uint32>(pPacket->B.count);
	NewPacket.m_u32Time = Time - m_dwStartTime;
	NewPacket.m_bIsIn = IsIn;

	m_aLogPackets.push_back(NewPacket);
	if (m_aLogPackets.size() > 100) FlushLog();

	m_cs.Leave();
};

void		INetLog::LogData(u32 Time, void* data, bsize size, bool IsIn)
{
	if (!data) return;

	m_cs.Enter();

	SLogPacket NewPacket;
	
	NewPacket.m_u16Type = *((u16*)data);
	NewPacket.m_u32Size =static_cast<uint32>( size);
	NewPacket.m_u32Time = Time - m_dwStartTime;
	NewPacket.m_bIsIn = IsIn;

	m_aLogPackets.push_back(NewPacket);
	if (m_aLogPackets.size() > 100) FlushLog();

	m_cs.Leave();
}