/*

		This is for AzerothCpre 3.3.5 (25-March-2019)

		What it does?
		Bassically, it loops through all active sessions and change display id of every online player.
		Type: .allmorph 999 (this will morph all players into a crab)

		I made it mostly for learning purposes.
*/

#include "ScriptMgr.h"
#include "Chat.h"
#include "Player.h"
#include "Pet.h"
#include "Language.h"
#include "Config.h"
#include "World.h"

class morph_all_command : public CommandScript
{
public:
	morph_all_command() : CommandScript("morph_all_command") { }

	std::vector<ChatCommand> GetCommands() const override
	{
		static std::vector<ChatCommand> CustomCommandTable =
		{
			{ "allmorph",     SEC_ADMINISTRATOR,      false,      &HandleMorphAllCommand,         "" },	//custom morph
			{ "alldemorph",   SEC_ADMINISTRATOR,      false,      &HandleDeMorphAllCommand,       "" }
		};
		return CustomCommandTable;
	}

	static bool HandleMorphAllCommand(ChatHandler * handler, const char * args)
	{
		Player * player = handler->GetSession()->GetPlayer();

        bool configSkipAdmin = sConfigMgr->GetBoolDefault("MorphAll.SkipAdmin", true);

		if (!*args)
			return false;
		
		uint16 display_id = (uint16)atoi((char*)args);

		if (!display_id)
			return false;

		SessionMap const& m_sessions = sWorld->GetAllSessions();

		for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
		{
            if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
                continue;

            if (configSkipAdmin && itr->second->GetPlayer()->GetSession()->GetSecurity() == SEC_ADMINISTRATOR)
                continue; // skip if player is Admin

			itr->second->GetPlayer()->SetDisplayId(display_id);
		}

		return true;
	}

	static bool HandleDeMorphAllCommand(ChatHandler * handler, const char * args)
	{
		Player * player = handler->GetSession()->GetPlayer();
		SessionMap const& m_sessions = sWorld->GetAllSessions();

        for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        {
            if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
                continue;

            itr->second->GetPlayer()->DeMorph();
        }

		return true;
	}
};

class morph_all_command_World : public WorldScript
{
public:
    morph_all_command_World() : WorldScript("morph_all_command_World") { }

    void OnStartup() override 
    {
        // Show this On worldserver startup
        sLog->outString("");
        sLog->outString("==============================");
        sLog->outString("== Loaded morph_all_command ==");
        sLog->outString("==============================");
        sLog->outString("");
    }

    void OnAfterConfigLoad(bool reload) override
    {
        if (reload) 
        {
            // Show this if ".reload config" command is used
            sLog->outString("");
            sLog->outString("=================================");
            sLog->outString("== Re-Loaded morph_all_command ==");
            sLog->outString("=================================");
            sLog->outString("");

            sWorld->SendGMText(40000);
        }
    }
};

void AddSC_morph_all_command()
{
	new morph_all_command();
    new morph_all_command_World();
}


