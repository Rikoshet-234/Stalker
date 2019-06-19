#include "stdafx.h"
#include "UI/UITalkWnd.h"

#include "UI/UITradeWnd.h"
#include "UI/UITalkDialogWnd.h"

#include "actor.h"
#include "trade.h"
#include "HUDManager.h"
#include "UIGameSP.h"
#include "PDA.h"
#include "character_info.h"
#include "level.h"

#include "PhraseDialog.h"
#include "PhraseDialogManager.h"

#include "game_cl_base.h"
#include "string_table.h"
#include "xr_level_controller.h"
#include "engine/cameraBase.h"
#include "UI/UIXmlInit.h"
#include "UI/UI3tButton.h"

CUITalkWnd::CUITalkWnd()
{
	m_pActor				= NULL;

	m_pOurInvOwner			= NULL;
	m_pOthersInvOwner		= NULL;

	m_pOurDialogManager		= NULL;
	m_pOthersDialogManager	= NULL;

	ToTopicMode				();

	InitTalkWnd				();
	Hide					();

	m_bNeedToUpdateQuestions = false;
	b_disable_break			= false;
}

CUITalkWnd::~CUITalkWnd()
{
}

void CUITalkWnd::InitTalkWnd()
{
	inherited::SetWndRect(Frect().set(0, 0, UI_BASE_WIDTH, UI_BASE_HEIGHT));

	UITalkDialogWnd = xr_new<CUITalkDialogWnd>();UITalkDialogWnd->SetAutoDelete(true);
	AttachChild(UITalkDialogWnd);
	UITalkDialogWnd->m_pParent = this;
	UITalkDialogWnd->InitTalkDialogWnd();
}

void CUITalkWnd::InitTalkDialog()
{
	m_pActor = Actor();
	if (m_pActor && !m_pActor->IsTalking()) return;

	m_pOurInvOwner = smart_cast<CInventoryOwner*>(m_pActor);
	m_pOthersInvOwner = m_pActor->GetTalkPartner();

	m_pOurDialogManager = smart_cast<CPhraseDialogManager*>(m_pOurInvOwner);
	m_pOthersDialogManager = smart_cast<CPhraseDialogManager*>(m_pOthersInvOwner);

	//����� ������������
	UITalkDialogWnd->UICharacterInfoLeft.InitCharacter		(m_pOurInvOwner->object_id());
	UITalkDialogWnd->UICharacterInfoRight.InitCharacter		(m_pOthersInvOwner->object_id());

//.	UITalkDialogWnd->UIDialogFrame.UITitleText.SetText		(m_pOthersInvOwner->Name());
//.	UITalkDialogWnd->UIOurPhrasesFrame.UITitleText.SetText	(m_pOurInvOwner->Name());
	
	//�������� ��� ���������
	UITalkDialogWnd->ClearAll();

	InitOthersStartDialog					();
	NeedUpdateQuestions						();
	Update									();

	UITalkDialogWnd->mechanic_mode			= m_pOthersInvOwner->SpecificCharacter().upgrade_mechanic();
	UITalkDialogWnd->SetOsoznanieMode		(m_pOthersInvOwner->NeedOsoznanieMode());
	UITalkDialogWnd->Show					();
	UITalkDialogWnd->UpdateButtonsLayout(b_disable_break, m_pOthersInvOwner->IsTradeEnabled());
}

void CUITalkWnd::InitOthersStartDialog()
{
	m_pOthersDialogManager->UpdateAvailableDialogs(m_pOurDialogManager);
	if(!m_pOthersDialogManager->AvailableDialogs().empty())
	{
		m_pCurrentDialog = m_pOthersDialogManager->AvailableDialogs().front();
		m_pOthersDialogManager->InitDialog(m_pOurDialogManager, m_pCurrentDialog);
		
		//������� �����
		CStringTable stbl;
		AddAnswer(m_pCurrentDialog->GetPhraseText("0"), m_pOthersInvOwner->Name());
		m_pOthersDialogManager->SayPhrase(m_pCurrentDialog, "0");

		//���� ������ ����������, ������� � ����� ������ ����
		if(!m_pCurrentDialog || m_pCurrentDialog->IsFinished()) ToTopicMode();
	}
}

void CUITalkWnd::NeedUpdateQuestions()
{
	m_bNeedToUpdateQuestions = true;
}

void CUITalkWnd::UpdateQuestions()
{
	UITalkDialogWnd->ClearQuestions();

	//���� ��� ��������� �������, ��
	//������ ������ ����
	if(!m_pCurrentDialog)
	{
		m_pOurDialogManager->UpdateAvailableDialogs(m_pOthersDialogManager);
		for(u32 i=0; i< m_pOurDialogManager->AvailableDialogs().size(); ++i)
		{
			const DIALOG_SHARED_PTR& phrase_dialog = m_pOurDialogManager->AvailableDialogs()[i];
			AddQuestion(phrase_dialog->DialogCaption(), phrase_dialog->GetDialogID());
		}
	}
	else
	{
		if(m_pCurrentDialog->IsWeSpeaking(m_pOurDialogManager))
		{
			//���� � ������ ���������� ���� ������ ���� ����� ��������, �� ������
			//������� (����� ��� �� ���������� ������� ��������)
			if( !m_pCurrentDialog->PhraseList().empty() && m_pCurrentDialog->allIsDummy() ){
				CPhrase* phrase = m_pCurrentDialog->PhraseList()[Random.randI(m_pCurrentDialog->PhraseList().size())];
				SayPhrase(phrase->GetID());
			};

			//����� ��������� ���� �� ��������� �������
			if( m_pCurrentDialog && !m_pCurrentDialog->allIsDummy() )
			{			
				for(PHRASE_VECTOR::const_iterator   it = m_pCurrentDialog->PhraseList().begin();
					it != m_pCurrentDialog->PhraseList().end();
					it++)
				{
					CPhrase* phrase = *it;
					AddQuestion( m_pCurrentDialog->GetPhraseText( phrase->GetID() ), phrase->GetID() );
				}
			}
			else
				UpdateQuestions();
		}
	}
	m_bNeedToUpdateQuestions = false;

}

//////////////////////////////////////////////////////////////////////////

void CUITalkWnd::SendMessage(CUIWindow* pWnd, s16 msg, void* pData)
{
	if(pWnd == UITalkDialogWnd && msg == TALK_DIALOG_TRADE_BUTTON_CLICKED)
	{
		SwitchToTrade();
	}
	else if(pWnd == UITalkDialogWnd && msg == TALK_DIALOG_UPGRADE_BUTTON_CLICKED)
	{
		SwitchToUpgrade();
	}
	else if(pWnd == UITalkDialogWnd && msg == TALK_DIALOG_QUESTION_CLICKED)
	{
		AskQuestion();
	}
	inherited::SendMessage(pWnd, msg, pData);
}

//////////////////////////////////////////////////////////////////////////
void UpdateCameraDirection(CGameObject* pTo)
{
	CCameraBase* cam = Actor()->cam_Active();

	Fvector des_dir; 
	Fvector des_pt;
	pTo->Center(des_pt);
	des_pt.y+=pTo->Radius()*0.5f;

	des_dir.sub(des_pt,cam->vPosition);

	float p,h;
	des_dir.getHP(h,p);


	if(XrMath::angle_difference(cam->yaw,-h)>0.2)
		cam->yaw		= XrMath::angle_inertion_var(cam->yaw,		-h,	0.15f,	0.2f,	XrMath::PI_DIV_6,	Device.fTimeDelta);

	if(XrMath::angle_difference(cam->pitch,-p)>0.2)
		cam->pitch		= XrMath::angle_inertion_var(cam->pitch,	-p,	0.15f,	0.2f,	XrMath::PI_DIV_6,	Device.fTimeDelta);

}

void CUITalkWnd::Update()
{
	//���������� ��������, ���� �����
	if (g_actor && m_pActor && !m_pActor->IsTalking() )
	{
		StopTalk();
	}else{
		CGameObject* pOurGO = smart_cast<CGameObject*>(m_pOurInvOwner);
		CGameObject* pOtherGO = smart_cast<CGameObject*>(m_pOthersInvOwner);
	
		if(	NULL==pOurGO || NULL==pOtherGO )
			Game().StartStopMenu(this,true);
	}

	if(m_bNeedToUpdateQuestions)
	{
		UpdateQuestions			();
	}
	inherited::Update			();
	UpdateCameraDirection		(smart_cast<CGameObject*>(m_pOthersInvOwner));

	UITalkDialogWnd->UpdateButtonsLayout(b_disable_break, m_pOthersInvOwner->IsTradeEnabled());

	if(playing_sound())
	{
		CGameObject* pOtherGO	= smart_cast<CGameObject*>(m_pOthersInvOwner);
		Fvector P				= pOtherGO->Position();
		P.y						+= 1.8f;
		m_sound.set_position	(P);
	}
}

void CUITalkWnd::Draw()
{
	inherited::Draw				();
}

void CUITalkWnd::Show()
{
	InitTalkDialog				();
	inherited::Show				();
}

void CUITalkWnd::Hide()
{
	StopSnd						();
	UITalkDialogWnd->Hide		();

	inherited::Hide				();
	if(!m_pActor)				return;
	
	ToTopicMode					();

	if (m_pActor->IsTalking()) m_pActor->StopTalk();
	m_pActor = NULL;
}

bool  CUITalkWnd::TopicMode			() 
{
	return NULL == m_pCurrentDialog.get();
}

void  CUITalkWnd::ToTopicMode		() 
{
	m_pCurrentDialog = DIALOG_SHARED_PTR((CPhraseDialog*)NULL);
}

void CUITalkWnd::AskQuestion()
{
	if(m_bNeedToUpdateQuestions) return;//quick dblclick:(
	shared_str					phrase_id;

	//����� ������ ���� ���������
	if(TopicMode())
	{
		if ( (UITalkDialogWnd->m_ClickedQuestionID =="") ||
			(!m_pOurDialogManager->HaveAvailableDialog(UITalkDialogWnd->m_ClickedQuestionID)) ) 
		{

			string128	s;
			sprintf_s		(s,"ID = [%s] of selected question is out of range of available dialogs ",UITalkDialogWnd->m_ClickedQuestionID);
			VERIFY2(FALSE, s);
		}

		m_pCurrentDialog = m_pOurDialogManager->GetDialogByID( UITalkDialogWnd->m_ClickedQuestionID);
		
		m_pOurDialogManager->InitDialog(m_pOthersDialogManager, m_pCurrentDialog);
		phrase_id = "0";
	}
	else
	{
		phrase_id = UITalkDialogWnd->m_ClickedQuestionID;
	}

	SayPhrase				(phrase_id);
	NeedUpdateQuestions		();
}

void CUITalkWnd::SayPhrase(const shared_str& phrase_id)
{

	AddAnswer(m_pCurrentDialog->GetPhraseText(phrase_id), m_pOurInvOwner->Name());
	m_pOurDialogManager->SayPhrase(m_pCurrentDialog, phrase_id);
	//���� ������ ����������, ������� � ����� ������ ����
	if(m_pCurrentDialog->IsFinished()) ToTopicMode();
}

void CUITalkWnd::AddQuestion(const shared_str& text, const shared_str& value)
{
	if(text.size() == 0)
	{
		return;
	}
	UITalkDialogWnd->AddQuestion(*CStringTable().translate(text),value.c_str());
}

void CUITalkWnd::AddAnswer(const shared_str& text, LPCSTR SpeakerName)
{
	//��� ������ ����� ������ ������ �� �������
	if(text.size() == 0)
	{
		return;
	}
	PlaySnd			(text.c_str());

	bool i_am = (0 == xr_strcmp(SpeakerName, m_pOurInvOwner->Name()));
	UITalkDialogWnd->AddAnswer(SpeakerName,*CStringTable().translate(text),i_am);
}

void CUITalkWnd::SwitchToTrade()
{
	if ( m_pOurInvOwner->IsTradeEnabled() && m_pOthersInvOwner->IsTradeEnabled() )
	{
		CUIGameSP* pGameSP = smart_cast<CUIGameSP*>( HUD().GetUI()->UIGame() );
		if ( pGameSP )
		{
			if ( pGameSP->MainInputReceiver() )
			{
				Game().StartStopMenu( pGameSP->MainInputReceiver(), true );
			}
			pGameSP->StartTrade	(m_pOurInvOwner, m_pOthersInvOwner);
		} // pGameSP
	}
}

void CUITalkWnd::SwitchToUpgrade()
{
	//if ( m_pOurInvOwner->IsInvUpgradeEnabled() && m_pOthersInvOwner->IsInvUpgradeEnabled() )
	{
		CUIGameSP* pGameSP = smart_cast<CUIGameSP*>(HUD().GetUI()->UIGame());
		if ( pGameSP )
		{
			if ( pGameSP->MainInputReceiver() )
			{
				Game().StartStopMenu(pGameSP->MainInputReceiver(),true);
			}
			pGameSP->StartUpgrade(m_pOurInvOwner, m_pOthersInvOwner);
		}
	}
}

bool CUITalkWnd::IR_OnKeyboardPress(int dik)
{
//.	StopSnd						();
	EGameActions cmd = get_binded_action(dik);
	if ( cmd==kUSE || cmd==kQUIT)
	{
		if(!b_disable_break)
			GetHolder()->StartStopMenu(this, true);
		return true;
	}
	if ( cmd == kSPRINT_TOGGLE )
	{
		if (m_pOthersInvOwner&&m_pOthersInvOwner->NeedOsoznanieMode())
		{
			return true;
		}
		UITalkDialogWnd->SetTradeMode();
		return true;
	}
	return inherited::IR_OnKeyboardPress(dik);
}

bool CUITalkWnd::OnKeyboard(int dik, EUIMessages keyboard_action)
{
	return inherited::OnKeyboard(dik,keyboard_action);
}

void CUITalkWnd::PlaySnd(LPCSTR text)
{
	u32 text_len = xr_strlen(text);
	if ( text_len == 0 )
	{
		return;
	}
	
	string_path	fn;
	
	LPCSTR path = "characters_voice\\dialogs\\";
	LPCSTR ext  = ".ogg";
	u32 tsize   = sizeof(fn) - xr_strlen(path) - xr_strlen(ext) - 1;
	if ( text_len > tsize )
	{
		text_len = tsize;
	}

	strncpy_s( fn, sizeof(fn), path, xr_strlen(path) );
	strncat_s( fn, sizeof(fn), text, text_len );
	strncat_s( fn, sizeof(fn), ext,  xr_strlen(ext) );

	//	strconcat( sizeof(fn), fn, "characters_voice\\dialogs\\", text2, ".ogg" );

	StopSnd();
	if ( FS.ExistFile( "%sounds%", fn ) )
	{
		VERIFY( m_pActor );
		if ( !m_pActor->OnDialogSoundHandlerStart(m_pOthersInvOwner, fn) )
		{
			CGameObject* pOtherGO = smart_cast<CGameObject*>(m_pOthersInvOwner);
			Fvector P = pOtherGO->Position();
			P.y			+= 1.8f;
			m_sound.create( fn, st_Effect, sg_SourceType );
			m_sound.play_at_pos( 0, P );
		}
	}
}

void CUITalkWnd::StopSnd()
{
	if (m_pActor && m_pActor->OnDialogSoundHandlerStop(m_pOthersInvOwner)) return;

	if(m_sound._feedback()) 
		m_sound.stop	();
}

void CUITalkWnd::AddIconedMessage(LPCSTR caption, LPCSTR text, LPCSTR texture_name, LPCSTR templ_name)
{
	UITalkDialogWnd->AddIconedAnswer(caption, text, texture_name, templ_name);
}

void CUITalkWnd::StopTalk()
{
	Game().StartStopMenu(this,true);
}

void CUITalkWnd::Stop()
{
}