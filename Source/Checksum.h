#pragma once
#ifndef CHECKSUM_H
#define CHECKSUM_H
#include "Defines.h"

EXTERN char* FindChecksumName(DWORD checksum);


inline unsigned long Checksum(const char* string)//THPS3 Function for CRC32
{
	unsigned long checksum;

	typedef unsigned long(__cdecl* pCRC32Address)(const char* string);
	pCRC32Address pCRC32 = (pCRC32Address)(0x004265F0);
	checksum = pCRC32(string);

	return checksum;
}


constexpr DWORD checksumTable[256] =
{
	  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
	  0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	  0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	  0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	  0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
	  0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
	  0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	  0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	  0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	  0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
	  0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
	  0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	  0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	  0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	  0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
	  0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
	  0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	  0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	  0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	  0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
	  0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
	  0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	  0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	  0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	  0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
	  0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
	  0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	  0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	  0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	  0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	  0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
	  0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	  0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	  0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	  0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
	  0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
	  0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	  0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	  0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	  0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
	  0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
	  0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	  0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	  0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
	  0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
	  0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	  0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	  0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	  0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
	  0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
	  0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	  0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	  0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

unsigned long crc32f(unsigned char* buf)
{
	unsigned long crc = 0xFFFFFFFF;
	unsigned char c = *buf++;
	while (c != 0x0)
	{
		if (c >= 'A' && c <= 'Z') c += 32;
		if (c == '/') c = '\\';

		crc = checksumTable[(unsigned char)((crc ^ c) & 0xFF)] ^ (crc >> 8);
		c = *buf++;
	}
	return crc;
}

template<size_t idx>
constexpr uint32_t crc32(const char* str)
{
	//Commented out the part to change to lowercase to make it compile faster
	//Just need to remember to only use lowercase strings now while generating checksums @ compiletime
	//Else they will not match
	/*char c = str[idx];
	if (c >= 'A' && c <= 'Z') c += 32;*/
	//if (c == '/') c = '\\';
	return (crc32<idx - 1>(str) >> 8) ^ checksumTable[(crc32<idx - 1>(str) ^ str[idx]) & 0x000000FF];
}

// This is the stop-recursion function
template<>
constexpr uint32_t crc32<size_t(-1)>(const char* str)
{
	return 0xFFFFFFFF;
}

// This doesn't take into account the nul char
#define COMPILE_CRC32(x) (crc32<sizeof(x) - 2>(x))


namespace Checksums
{
	enum CompileTimeChecksums//Generated at compile-time for improved performance and code readability
	{
		Class = COMPILE_CRC32("class"),
		Name = COMPILE_CRC32("name"),
		Position = COMPILE_CRC32("position"),
		Pos = COMPILE_CRC32("pos"),
		Angles = COMPILE_CRC32("angles"),
		Kill = COMPILE_CRC32("kill"),
		Invisible = COMPILE_CRC32("invisible"),
		Shatter = COMPILE_CRC32("shatter"),
		Create = COMPILE_CRC32("create"),
		ShatterAndDie = COMPILE_CRC32("shatteranddie"),
		Visible = COMPILE_CRC32("visisble"),
		GameObject = COMPILE_CRC32("gameobject"),
		LevelGeometry = COMPILE_CRC32("levelgeometry"),
		LevelObject = COMPILE_CRC32("levelobject"),
		InNetGame = COMPILE_CRC32("innetgame"),
		Cluster = COMPILE_CRC32("cluster"),
		TerrainType = COMPILE_CRC32("terraintype"),
		Type = COMPILE_CRC32("type"),
		TriggerScripts = COMPILE_CRC32("triggerscripts"),
		PedAI = COMPILE_CRC32("pedai"),
		DEFAULT = COMPILE_CRC32("default"),
		Value = COMPILE_CRC32("value"),
		TrickObject = COMPILE_CRC32("trickobject"),
		TrickOb = COMPILE_CRC32("trickob"),
		AbsentInNetGames = COMPILE_CRC32("absentinnetgames"),
		NetEnabled = COMPILE_CRC32("netenabled"),
		Permanent = COMPILE_CRC32("permanent"),
		CreatedAtStart = COMPILE_CRC32("createdatstart"),
		Links = COMPILE_CRC32("links"),
		Waypoint = COMPILE_CRC32("waypoint"),
		Restart = COMPILE_CRC32("restart"),
		Relpos = COMPILE_CRC32("relpos"),
		Normal_Lerp_Speed = COMPILE_CRC32("normal_lerp_speed"),
		OnServer = COMPILE_CRC32("onserver"),
		ANGULAR_VELOCITY = COMPILE_CRC32("angular_velocity"),
		MOVE_TO_POS = COMPILE_CRC32("move_to_pos"),
		ORIENT = COMPILE_CRC32("orient"),
		FUNCTION = COMPILE_CRC32("function"),
		param = COMPILE_CRC32("param"),
		text = COMPILE_CRC32("text"),
		//Physics_Air_Gravity = COMPILE_CRC32("physics_air_gravity"),
		CHEAT_MOON = COMPILE_CRC32("cheat_moon"),

		LM_Control_bRevert = COMPILE_CRC32("lm_control_brevert"),
		LM_Control_SpineButton = 0x2A135B22,//COMPILE_CRC32("lm_control_spinebutton"),
		LM_Control_bNewTricks = 0x0760404D, //COMPILE_CRC32("lm_control_bnewtricks"),
		LM_Control_AirTrickSpeed = 0x24A516D2, //COMPILE_CRC32("lm_control_airtrickspeed"),
		LM_Control_bXinput = COMPILE_CRC32("lm_control_bxinput"),
		LM_Control_bSpine = COMPILE_CRC32("lm_control_bspine"),
		LM_Control_bAcid = COMPILE_CRC32("lm_control_bacid"),
		LM_Control_bBank = COMPILE_CRC32("lm_control_bbank"),

		LM_GUI_bShowGrafCounter = 0x77823453, //COMPILE_CRC32("lm_gui_bshowgrafcounter"),
		LM_GUI_bShowHud = 0x71ACFD96, //COMPILE_CRC32("lm_gui_bshowhud"),
		LM_GUI_bNewMenu = COMPILE_CRC32("lm_gui_bnewmenu"),

		LM_BugFix_bTeleFix = COMPILE_CRC32("lm_bugfix_btelefix"),
		LM_BugFix_bSoundFix = 0x01D8084A, //COMPILE_CRC32("lm_bugfix_bsoundfix"),

		LM_GameOption_bLimitTags = 0xE3439B74, //COMPILE_CRC32("lm_gameoption_blimittags"),
		LM_GameOption_bGrass = 0x960BE926, //COMPILE_CRC32("lm_gameoption_bgrass"),
		LM_DebugOption_bDebugMode = 0x5EC80650, //COMPILE_CRC32("lm_debugoption_bdebugoption"),

		Key = COMPILE_CRC32("key"),
		Zone = COMPILE_CRC32("zone"),
		Flag = COMPILE_CRC32("flag"),
		Crown = COMPILE_CRC32("crown"),
		Spawn = COMPILE_CRC32("spawn"),

		Actuator = COMPILE_CRC32("actuator"),
		Percent = COMPILE_CRC32("percent"),
		Duration = COMPILE_CRC32("duration"),

		OrientToNormal = COMPILE_CRC32("orienttonormal"),

		Trick_The900 = COMPILE_CRC32("trick_the900"),
		Trick_KFBackflip = COMPILE_CRC32("trick_kfbackflip"),
		Trick_StaleFrontFlip = COMPILE_CRC32("trick_kffrontflip"),
		Trick_StaleBackFlip = COMPILE_CRC32("trick_kfbackflip"),
		Trick_MistyFlip = COMPILE_CRC32("trick_mistyflip"),
		Trick_FS540HeelFlip = 0x225961E2,/// COMPILE_CRC32("trick_fs540heelflip"),
		Trick_FS540 = COMPILE_CRC32("trick_fs540"),
		Trick_McTwist = COMPILE_CRC32("trick_mctwist"),
		Trick_Layout = COMPILE_CRC32("trick_layout"),
		Trick_MaulForceGrab = COMPILE_CRC32("maulforcegrab"),
		Trick_SackTap = COMPILE_CRC32("trick_sacktap"),
		Trick_2KickMadonnaFlip = 0xDC0D4F76,// COMPILE_CRC32("trick_2kickmadonnaflip"),
		Trick_AirCasperFlip = 0x885CD7EC,// COMPILE_CRC32("trick_aircasperflip"),
		Trick_PizzaGuy = COMPILE_CRC32("trick_pizzaguy"),
		Trick_540TailWhip = COMPILE_CRC32("trick_540tailwhip"),
		Trick_GhettoBird = COMPILE_CRC32("trick_ghettobird"),
		Trick_Berzerker = COMPILE_CRC32("trick_berzerker"),
		Trick_FingerFlipAirWalk = 0x1B6271D6,// COMPILE_CRC32("trick_fingerflipairwalk"),
		Trick_FingerFlip = COMPILE_CRC32("trick_fingerflip"),
		Trick_Jackass = COMPILE_CRC32("trick_jackass"),
		Trick_1234 = COMPILE_CRC32("trick_1234"),
		Trick_540Flip = COMPILE_CRC32("trick_540flip"),
		Trick_BetweenLegsSlam = 0x71C35A7E,// COMPILE_CRC32("trick_betweenlegsslam"),
		Trick_NollieFlipUnderflip = 0x27A1B702,// COMPILE_CRC32("trick_nollieflipunderflip"),
		Trick_HardFlipBackFootFlip = 0x30DBED8F,// COMPILE_CRC32("trick_hardflipbackfootflip"),
		Trick_KickFlipUnderFlip = 0x38FE15C9,// COMPILE_CRC32("trick_kickflipunderflip"),
		Trick_360Hardflip = COMPILE_CRC32("trick_360hardflip"),
		Trick_QuadrupleHeelFlip = 0xBCBA5274, //COMPILE_CRC32("trick_quadrupleheelflip"),
		Trick_OllieAirwalk = COMPILE_CRC32("trick_ollieairwalk"),
		CSOllieairwalkshoveit = 0x298D4F36,// COMPILE_CRC32("trick_csollieairwalkshoveit"),
		CSOllienorthbackfootflip = 0x3451C808,// COMPILE_CRC32("trick_csollienorthbackfootflip"),
		Trick_Trick_FFImpossible = COMPILE_CRC32("trick_ffimpossible"),
		CSdoublefrontfootimpossible = 0xF93FA905,// = COMPILE_CRC32("trick_csdoublefrontfootimpossible"),
		CSDoubleFingerFlip = 0x9221CBE2,// = COMPILE_CRC32("trick_csdoublefingerflip"),
		Trick_360Varial = COMPILE_CRC32("trick_360varial"),
		Trick_SalFlip = COMPILE_CRC32("trick_salflip"),
		Trick_180Varial = COMPILE_CRC32("trick_180varial"),
		Trick_OneFootJapan = COMPILE_CRC32("trick_onefootjapan"),
		Trick_Cannonball = COMPILE_CRC32("trick_cannonball"),
		CSFingerFlipCannonball = 0x6F6EE575,//  COMPILE_CRC32("trick_csfingerflipcannonball"),
		Trick_Method = COMPILE_CRC32("trick_method"),
		Trick_Judo = COMPILE_CRC32("trick_judo"),
		Speed = COMPILE_CRC32("speed"),
		option = COMPILE_CRC32("option"),

		//Trick_KFBackflip = 0xF7B4E0CB



	};
};
#endif