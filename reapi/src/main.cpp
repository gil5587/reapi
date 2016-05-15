#include "precompiled.h"

edict_t* g_pEdicts;
int gmsgSendAudio;
int gmsgTeamScore;

void OnAmxxAttach()
{
	// initialize API
	api_cfg.Init();
}

bool OnMetaAttach()
{
	return true;
}

void OnMetaDetach()
{
	// clear all hooks?
	g_hookManager.clearHandlers();

	if (api_cfg.hasVTC()) {
		g_pVoiceTranscoderApi->ClientStartSpeak()->unregisterCallback(&ClientStartSpeak);
		g_pVoiceTranscoderApi->ClientStopSpeak()->unregisterCallback(&ClientStopSpeak);
	}

	if (api_cfg.hasReGameDLL()) {
		g_ReGameHookchains->InstallGameRules()->unregisterHook(&InstallGameRules);
	}
}

void ServerActivate_Post(edict_t *pEdictList, int edictCount, int clientMax)
{
	g_pEdicts = pEdictList;
	gmsgSendAudio = GET_USER_MSG_ID(PLID, "SendAudio", NULL);
	gmsgTeamScore = GET_USER_MSG_ID(PLID, "TeamScore", NULL);

	SET_META_RESULT(MRES_IGNORED);
}

void ServerDeactivate_Post()
{
	api_cfg.ServerDeactivate();
	g_hookManager.clearHandlers();

	SET_META_RESULT(MRES_IGNORED);
}

CGameRules *InstallGameRules(IReGameHook_InstallGameRules *chain)
{
	return g_pGameRules = chain->callNext();
}