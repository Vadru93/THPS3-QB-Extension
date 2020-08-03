
//level numbers
LevelNum_Skateshop = 0
LevelNum_Foundry = 1
LevelNum_Canada = 2
LevelNum_Rio = 3
LevelNum_Suburbia = 4
LevelNum_Airport = 5
LevelNum_SkaterIsland = 6
LevelNum_LA = 7
LevelNum_Tokyo = 8
LevelNum_Ship = 9
LevelNum_Oil = 10
LevelNum_Tutorials = 11
LevelNum_Warehouse = 12
LevelNum_Burnside = 13
LevelNum_Roswell = 14
LevelNum_New = 15

//default values
NetSkyIsOn = 0
sky_path = "empty"


SCRIPT InitLevelLoadProcess
	printf "***********************InitLevelLoadProcess"
	UninitializeFrontEndSprites
	Cleanup
	StopMusic
	PauseMusic
	ClearMusicTrackList
	MouseShowCursor 0
ENDSCRIPT

SCRIPT PreFreezeScreen
	SetArenaSize 1
ENDSCRIPT

SCRIPT PostFreezeScreen
	SetArenaSize
ENDSCRIPT


SCRIPT SetupForClipPlane
	EnableFog
	SetClippingDistances near = 12 far = 20000
	SetBackgroundColor { r = 128 g = 128 b = 128 alpha = 0 }
	SetFogDistance distance = 1280
	SetFogColor r = 128 g = 128 b = 128
ENDSCRIPT

SCRIPT PreLevelLoad
	SetArenaSize 1
	SetUpBasicLights
	SetSfxReverb Instant
	ClearScoreGoals
	SetScoreGoal score = 10000 goalscript = DefaultHiScoreScript goal = GOAL_HIGHSCORE
	SetScoreGoal score = 50000 goalscript = DefaultProScoreScript goal = GOAL_PROSCORE
	ClearMusicTrackList
	IF GameModeEquals is_parkeditor
		IF InNetGame
			DisplayProperLoadingScreen net
		ELSE
			DisplayLoadingScreen "images\loadscrn_editor.png"
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT DisplayProperLoadingScreen
	printf "DisplayProperLoadingScreen"
	
	IF InMultiPlayerGame
	ELSE
		change FirstTimeFromSkateshop = 0
	ENDIF
	
	IF GotParam SkateShop
		DisplayLoadingScreen "images\loadscrn_ss.png"
	ELSE
		IF EnteringNetGame
			printf "Entering Net Game"
			IF OnServer
				DisplayLoadingScreen "images\loadscrn_Online.png"
			ELSE
				DisplayLoadingScreen "images\loadscrn_Online2.png"
			ENDIF
		ELSE
			IF IsTrue FirstTimeFromSkateshop
				DisplayLoadingScreen "images\loadscrn_2player.png"
			ELSE 
				LevelLoadingScreens <...>
			ENDIF
		ENDIF
	ENDIF
	
	IF GotParam Simulate
		wait 2 seconds
	ENDIF
	
ENDSCRIPT

SCRIPT LevelLoadingScreens
	IF GotParam Foundry
		DisplayLoadingScreen "images\loadscrn_foundry.png"
	ENDIF
	IF GotParam Canada
		DisplayLoadingScreen "images\loadscrn_can.png"
	ENDIF
	IF GotParam Suburbia
		DisplayLoadingScreen "images\loadscrn_sub.png"
	ENDIF
	IF GotParam SI
		IF IsCareerMode
			DisplayLoadingScreen "images\loadscrn_si_comp.png"
		ELSE
			DisplayLoadingScreen "images\loadscrn_si.png"
		ENDIF
	ENDIF
	IF GotParam Airport
		DisplayLoadingScreen "images\loadscrn_ap.png"
	ENDIF
	IF GotParam Rio
		IF IsCareerMode
			DisplayLoadingScreen "images\loadscrn_rio_comp.png"
		ELSE
			DisplayLoadingScreen "images\loadscrn_rio.png"
		ENDIF
	ENDIF
	IF GotParam LA
		DisplayLoadingScreen "images\loadscrn_la.png"
	ENDIF
	IF GotParam Tokyo
		IF IsCareerMode
			DisplayLoadingScreen "images\loadscrn_tok_comp.png"
		ELSE
			DisplayLoadingScreen "images\loadscrn_tok.png"
		ENDIF
	ENDIF
	IF GotParam Ship
		DisplayLoadingScreen "images\loadscrn_ship.png"
	ENDIF
	IF GotParam Oil
		DisplayLoadingScreen "images\loadscrn_oil.png"
	ENDIF
	IF GotParam Tutorials
		DisplayLoadingScreen "images\loadscrn_tutorials.png"
	ENDIF
	IF GotParam Rooftops
		DisplayLoadingScreen "images\loadscrn_paris.png"
	ENDIF
	IF GotParam Generic
		DisplayLoadingScreen "images\loadscrn_generic.png"
	ENDIF
	IF GotParam Hangar
		DisplayLoadingScreen "images\load\th2_han.png"
	ENDIF
	IF GotParam School2
		DisplayLoadingScreen "images\load\th2_sc2.png"
	ENDIF
	IF GotParam Marseille
		DisplayLoadingScreen "images\load\th2_mar.png"
	ENDIF
	IF GotParam NewYork
		DisplayLoadingScreen "images\load\th2_ny.png"
	ENDIF
	IF GotParam Venice
		DisplayLoadingScreen "images\load\th2_ven.png"
	ENDIF
	IF GotParam SkateStreet
		DisplayLoadingScreen "images\load\th2_ssv.png"
	ENDIF
	IF GotParam Philly
		DisplayLoadingScreen "images\load\th2_philly.png"
	ENDIF
	IF GotParam Bullring
		DisplayLoadingScreen "images\load\th2_bul.png"
	ENDIF
	IF GotParam Drop
		DisplayLoadingScreen "images\load\th2_drop.png"
	ENDIF
	IF GotParam Heaven
		DisplayLoadingScreen "images\load\th2_hvn.png"
	ENDIF
	IF GotParam Club
		DisplayLoadingScreen "images\load\th2_club.png"
	ENDIF
	IF GotParam Subway
		DisplayLoadingScreen "images\load\th2_sway.png"
	ENDIF
	IF GotParam Tampa
		DisplayLoadingScreen "images\load\th2_flor.png"
	ENDIF
	IF GotParam SkyLines
		DisplayLoadingScreen "images\load\th2_sky.png"
	ENDIF
	IF GotParam Cons
		DisplayLoadingScreen "images\load\th2_cons.png"
	ENDIF
	IF GotParam Warehouse
		DisplayLoadingScreen "images\loadscrn_warehouse.png"
	ENDIF
	IF GotParam School1
		DisplayLoadingScreen "images\load\th1_sc1.png"
	ENDIF
	IF GotParam Mall
		DisplayLoadingScreen "images\load\th1_mall.png"
	ENDIF
	IF GotParam Vans
		DisplayLoadingScreen "images\load\th1_vans.png"
	ENDIF
	IF GotParam Downtown
		DisplayLoadingScreen "images\load\th1_down.png"
	ENDIF
	IF GotParam Jam
		DisplayLoadingScreen "images\load\th1_jam.png"
	ENDIF
	IF GotParam Burnside
		DisplayLoadingScreen "images\loadscrn_burnside.png"
	ENDIF
	IF GotParam Streets
		DisplayLoadingScreen "images\load\th1_sf1.png"
	ENDIF
	IF GotParam Roswell
		DisplayLoadingScreen "images\loadscrn_roswell.png"
	ENDIF
	IF GotParam College
		DisplayLoadingScreen "images\load\th4_col.png"
	ENDIF
	IF GotParam Alcatraz
		DisplayLoadingScreen "images\load\th4_alc.png"
	ENDIF
	IF GotParam London
		DisplayLoadingScreen "images\load\th4_lon.png"
	ENDIF
	IF GotParam SanFrancisco
		DisplayLoadingScreen "images\load\th4_sf2.png"
	ENDIF
	IF GotParam Kona
		DisplayLoadingScreen "images\load\th4_kon.png"
	ENDIF
	IF GotParam Carnival
		DisplayLoadingScreen "images\load\th4_cnv.png"
	ENDIF
	IF GotParam Shipyard
		DisplayLoadingScreen "images\load\th4_jnk.png"
	ENDIF
	IF GotParam Zoo
		DisplayLoadingScreen "images\load\th4_zoo.png"
	ENDIF
	IF GotParam MotoX
		DisplayLoadingScreen "images\load\th4_mot.png"
	ENDIF
	IF GotParam Chicago
		DisplayLoadingScreen "images\load\th4_hof.png"
	ENDIF
	IF GotParam Jersey
		DisplayLoadingScreen "images\load\ug1_nj.png"
	ENDIF
	IF GotParam Manhattan
		DisplayLoadingScreen "images\load\ug1_ny.png"
	ENDIF
	IF GotParam Florida
		DisplayLoadingScreen "images\load\ug1_fl.png"
	ENDIF
	IF GotParam SanDiego
		DisplayLoadingScreen "images\load\ug1_sd.png"
	ENDIF
	IF GotParam Hawaii
		DisplayLoadingScreen "images\load\ug1_hi.png"
	ENDIF
	IF GotParam Vancouver
		DisplayLoadingScreen "images\load\ug1_vc.png"
	ENDIF
	IF GotParam SlamCityJam
		DisplayLoadingScreen "images\load\ug1_sj.png"
	ENDIF
	IF GotParam Moscow
		DisplayLoadingScreen "images\load\ug1_ru.png"
	ENDIF
	IF GotParam HotterThanHell
		DisplayLoadingScreen "images\load\ug1_se.png"
	ENDIF
	
	IF GotParam Australia
		DisplayLoadingScreen "images\load\ug2_au.png"
	ENDIF
	IF GotParam Barcelona
		DisplayLoadingScreen "images\load\ug2_ba.png"
	ENDIF
	IF GotParam Berlin
		DisplayLoadingScreen "images\load\ug2_be.png"
	ENDIF
	IF GotParam Boston
		DisplayLoadingScreen "images\load\ug2_bo.png"
	ENDIF
	IF GotParam NewOrleans
		DisplayLoadingScreen "images\load\ug2_no.png"
	ENDIF
	//is this safe to use?
	IF GotParam ProSkater
		DisplayLoadingScreen "images\load\ug2_se.png"
	ENDIF
	//is this safe to use too?
	IF GotParam Triangle
		DisplayLoadingScreen "images\load\ug2_se2.png"
	ENDIF
	IF GotParam Skatopia
		DisplayLoadingScreen "images\load\ug2_st.png"
	ENDIF
ENDSCRIPT


SCRIPT PostLevelLoad
	SetArenaSize
ENDSCRIPT


SCRIPT script_assert <...>
	printf "SCRIPT ASSERT!"
	printf <...>
	BEGIN
		wait 1 gameframe
	REPEAT
ENDSCRIPT

SCRIPT request_level
	printf "Running script: request_level"
	change wall_non_skatable_angle = 25
	IF GotParam level
		RequestLevel <level>
	ELSE
		script_assert "request_level needs a level param"
	ENDIF
ENDSCRIPT

SCRIPT LoadLevel
	printf "Running script: LoadLevel"
	change wall_non_skatable_angle = 25
	MemPushContext 0
	PreLevelLoad
	LaunchLevel <...>
	PostLevelLoad
	MemPopContext
ENDSCRIPT

SCRIPT FireUpGame
	ToggleSkater On
	SetGameState On
	LaunchMenuScreen screen = game_menu
ENDSCRIPT


SCRIPT DistinguishLevels
	SetCompetition LevelNum_Rio
	SetCompetition LevelNum_SkaterIsland
	SetCompetition LevelNum_Tokyo
ENDSCRIPT

SCRIPT Load_Ap
	AddMusicTrack "ambience\ap"
	SetUpAirportLights
	Load_Level_Func {
		levelnum = LevelNum_Airport 
		lev_bsp = "Levels\ap\ap.bsp" 
		lev_sky = "Levels\Ap_Sky\Ap_Sky.bsp" 
		lev_qb = "Levels\ap\ap.qb" 
		lev_scr = Airport
		r = 151 
		g = 180 
		b = 214 
		a = 0 
		cnear = 13 
		cfar = 20000
	}
	Ap_Startup
ENDSCRIPT

SCRIPT Load_La
	AddMusicTrack "ambience\la"
	SetUpLosAngelesLights
	Load_Level_Func levelnum = LevelNum_LA lev_bsp = "Levels\la\la.bsp" lev_sky = "Levels\la_Sky\la_Sky.bsp" lev_qb = "Levels\la\la.qb" lev_scr = LA
	PrepareLevelFog r = 158 g = 202 b = 220 a = 0 cnear = 13 cfar = 14000
	La_Startup
ENDSCRIPT

SCRIPT Load_Rio
	AddMusicTrack "ambience\rio"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels\rio\rio.bsp" lev_sky = "Levels\rio_Sky\rio_Sky.bsp" lev_qb = "Levels\rio\rio.qb" lev_scr = Rio
	CareerStartLevel level = LevelNum_Rio competition
	PrepareLevelFog r = 145 g = 201 b = 242 a = 0 cnear = 13 cfar = 30000
	SetMovementVelocity 2000
	SetRotateVelocity 120
	LoadTerrain
	Rio_Startup
ENDSCRIPT

SCRIPT Load_Shp
	AddMusicTrack "ambience\ship"
	SetUpShipLights
	Load_Level_Func lev_bsp = "Levels\shp\shp.bsp" lev_sky = "Levels\shp_Sky\shp_Sky.bsp" lev_qb = "Levels\shp\shp.qb" lev_scr = Ship
	CareerStartLevel level = LevelNum_Ship
	PrepareLevelFog r = 143 g = 205 b = 233 a = 0 cnear = 13 cfar = 32000
	SetMovementVelocity 1500
	SetRotateVelocity 120
	LoadTerrain
	SHP_Startup
ENDSCRIPT

SCRIPT Load_Si
	AddMusicTrack "ambience\si"
	SetUpSkatersIslandLights
	Load_Level_Func lev_bsp = "Levels\si\si.bsp" lev_sky = "Levels\si_Sky\si_Sky.bsp" lev_qb = "Levels\si\si.qb" lev_scr = SI
	CareerStartLevel level = LevelNum_SkaterIsland competition
	PrepareLevelFog r = 158 g = 211 b = 255 a = 0 cnear = 13 cfar = 12000
	LoadTerrain
	Si_Startup
ENDSCRIPT

SCRIPT Load_Sub
	AddMusicTrack "ambience\sub"
	SetUpSuburbiaLights
	Load_Level_Func lev_bsp = "Levels\sub\sub.bsp" lev_sky = "Levels\sub_Sky\sub_Sky.bsp" lev_qb = "Levels\sub\sub.qb" lev_scr = Suburbia
	CareerStartLevel level = LevelNum_Suburbia
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 13 cfar = 13500
	LoadTerrain
	Sub_Startup
ENDSCRIPT

SCRIPT Load_Tok
	AddMusicTrack "ambience\tok"
	SetUpTokyoLights
	Load_Level_Func lev_bsp = "Levels\tok\tok.bsp" lev_sky = "Levels\tok_Sky\tok_Sky.bsp" lev_qb = "Levels\tok\tok.qb" lev_scr = Tokyo
	CareerStartLevel level = LevelNum_Tokyo competition
	PrepareLevelFog r = 16 g = 17 b = 26 a = 0 cnear = 13 cfar = 17000
	SetMovementVelocity 1000
	SetRotateVelocity 120
	LoadTerrain
	Tok_Startup
ENDSCRIPT

SCRIPT Load_Oil
	AddMusicTrack "ambience\oil"
	SetUpOilLights
	Load_Level_Func lev_bsp = "Levels\oil\oil.bsp" lev_sky = "Levels\oil_Sky\oil_Sky.bsp" lev_qb = "Levels\oil\oil.qb" lev_scr = Oil
	CareerStartLevel level = LevelNum_Oil
	PrepareLevelFog r = 190 g = 205 b = 238 a = 0 cnear = 13 cfar = 30000
	SetMovementVelocity 1000
	SetRotateVelocity 120
	LoadTerrain
	SetRenderModeVU
	Oil_Startup
ENDSCRIPT

SCRIPT Load_Roof
	AddMusicTrack "ambience\burn"
	SetUpSuburbiaLights
	Load_Level_Func lev_bsp = "Levels\par\par.bsp" lev_sky = "Levels\par_Sky\par_Sky.bsp" lev_qb = "Levels\par\par.qb" lev_scr = Rooftops
	CareerStartLevel level = LevelNum_New
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 13 cfar = 20000
	LoadTerrain
ENDSCRIPT

SCRIPT Load_Burn
	AddMusicTrack "ambience\thps1\burn"
	SetUpBurnLights
	Load_Level_Func lev_bsp = "Levels\burn\burn.bsp" lev_sky = "Levels\burn_Sky\burn_Sky.bsp" lev_qb = "Levels\burn\burn.qb" lev_scr = Burnside
	CareerStartLevel level = LevelNum_Burnside
	PrepareLevelFog r = 159 g = 183 b = 214 a = 0 cnear = 13 cfar = 9000
	LoadTerrain
	Burn_Startup
ENDSCRIPT

SCRIPT Load_Ros
	AddMusicTrack "ambience\thps1\ros"
	SetUpRoswellLights
	Load_Level_Func lev_bsp = "Levels\ros\ros.bsp" lev_sky = "Levels\ros_Sky\ros_Sky.bsp" lev_qb = "Levels\ros\ros.qb" lev_scr = Roswell
	CareerStartLevel level = LevelNum_Roswell
	PrepareLevelFog r = 41 g = 34 b = 52 a = 0 cnear = 13 cfar = 9000
	LoadTerrain
ENDSCRIPT

SCRIPT Load_Tut
	DisplayProperLoadingScreen Tutorials
	CareerStartLevel level = LevelNum_Tutorials
	SetUpLosAngelesLights
	LoadLevelGeometry level = "Levels\tut\tut.bsp" Sky = "Levels\Sk3Ed_Bch_Sky\Sk3Ed_Bch_Sky.bsp" Pre_set = "Tut"
	LoadNodeArray "levels\tut\tut.qb"
	IF ClipPlaneEnabled
		SetupForClipPlane
	ELSE
		SetBackgroundColor { r = 128 g = 128 b = 128 alpha = 0 }
		SetClippingDistances near = 12 far = 20000
	ENDIF
	SetBSPAmbientColor { r = 84 g = 84 b = 84 alpha = 0 }
	LoadTerrain
	Tut_Startup
	UnloadPreFile "Tut.pre" dont_assert
ENDSCRIPT

SCRIPT Load_SkateShop
	DisplayProperLoadingScreen SkateShop
	CareerStartLevel level = LevelNum_Skateshop
	SetUpSkateShopLights
	LoadLevelGeometry level = "Levels\skateshop\skateshop.bsp" Sky = "" Pre_set = "SkateShop" KeepTexturesInMemory
	LoadNodeArray "levels\skateshop\skateshop.qb"
	LoadTerrain
	IF ClipPlaneEnabled
		SetupForClipPlane
	ELSE
		SetBackgroundColor { r = 0 g = 0 b = 32 alpha = 0 }
		SetClippingDistances near = 12 far = 12000
	ENDIF
	SkateShop_Startup
	UnloadPreFile "SkateShop.pre" dont_assert
ENDSCRIPT

SCRIPT Load_Foo
	SetUpFoundryLights
	LoadLevelGeometry level = "Levels\foo\foo.bsp" Sky = "" Pre_set = "Foo"
	LoadNodeArray "levels\foo\foo.qb"
	LoadTerrain
	SetRenderModeVU
	SetBackgroundColor { r = 16 g = 16 b = 32 alpha = 0 }
	SetClippingDistances near = 12 far = 22000
	Foo_Startup
	UnloadPreFile "Foo.pre" dont_assert
ENDSCRIPT

SCRIPT THPS1Levels
	LaunchPanelMessage "=========================================THPS1==="
ENDSCRIPT

SCRIPT Load_Jam
	AddMusicTrack "ambience\thps2\jam"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th1\jam\jam.bsp" lev_sky = "Levels_Th1\jam\jam_sky.bsp" lev_qb = "Levels_Th1\jam\jam.qb" lev_scr = Jam
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 12 cfar = 15000
ENDSCRIPT

SCRIPT Load_Sc1
	AddMusicTrack "ambience\thps2\sc1"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th1\sc1\sc1.bsp" lev_sky = "Levels_Th1\sc1\sc1_sky.bsp" lev_qb = "Levels_Th1\sc1\sc1.qb" lev_scr = School1
	PrepareLevelFog r = 145 g = 201 b = 242 a = 0 cnear = 12 cfar = 13000
ENDSCRIPT

SCRIPT Load_SF1
	AddMusicTrack "ambience\thps2\sf1"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th1\sf1\sf1.bsp" lev_sky = "Levels_Th1\sf1\sf1_sky.bsp" lev_qb = "Levels_Th1\sf1\sf1.qb" lev_scr = Streets
	PrepareLevelFog r = 165 g = 180 b = 16 a = 202 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Mal
	AddMusicTrack "ambience\thps2\mall"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th1\mall\mall.bsp" lev_sky = "Levels_Th1\mall\mall_sky.bsp" lev_qb = "Levels_Th1\mall\mall.qb" lev_scr = Mall
	PrepareLevelFog r = 0 g = 0 b = 0 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Down
	AddMusicTrack "ambience\thps2\down"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th1\down\down.bsp" lev_sky = "Levels_Th1\down\down_sky.bsp" lev_qb = "Levels_Th1\down\down.qb" lev_scr = Downtown
	PrepareLevelFog r = 7 g = 8 b = 16 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Vans
	AddMusicTrack "ambience\thps2\vans"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th1\vans\vans.bsp" lev_sky = "Levels_Th1\vans\vans_sky.bsp" lev_qb = "Levels_Th1\vans\vans.qb" lev_scr = Vans
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 12 cfar = 10000
ENDSCRIPT

SCRIPT THPS2Levels
	LaunchPanelMessage "=========================================THPS2==="
ENDSCRIPT


SCRIPT Load_Drop
	AddMusicTrack "ambience\thps2\drop"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\Drop\Drop.bsp" lev_sky = "Levels_Th2\Drop\Drop_sky.bsp" lev_qb = "Levels_Th2\Drop\Drop.qb" lev_scr = Drop
	PrepareLevelFog r = 12 g = 8 b = 9 a = 0 cnear = 12 cfar = 10000
ENDSCRIPT

SCRIPT Load_Sc2
	AddMusicTrack "ambience\thps2\sc2"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\sc2\sc2.bsp" lev_sky = "Levels_Th2\sc2\sc2_sky.bsp" lev_qb = "Levels_Th2\sc2\sc2.qb" lev_scr = School2
	PrepareLevelFog r = 145 g = 201 b = 242 a = 0 cnear = 12 cfar = 13000
ENDSCRIPT

SCRIPT Load_Ven
	AddMusicTrack "ambience\thps2\ven"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\ven\ven.bsp" lev_sky = "Levels_Th2\ven\ven_Sky.bsp" lev_qb = "Levels_Th2\ven\ven.qb" lev_scr = Venice
	PrepareLevelFog r = 145 g = 201 b = 242 a = 0 cnear = 12 cfar = 18000
ENDSCRIPT


SCRIPT Load_SSV
	AddMusicTrack "ambience\thps2\ssv"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\ssv\ssv.bsp" lev_sky = "Levels_Th2\ssv\ssv_Sky.bsp" lev_qb = "Levels_Th2\ssv\ssv.qb" lev_scr = SkateStreet
	PrepareLevelFog r = 145 g = 201 b = 242 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Ph
	AddMusicTrack "ambience\thps2\ph"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\Ph\Ph.bsp" lev_sky = "Levels_Th2\Ph\Ph_Sky.bsp" lev_qb = "Levels_Th2\Ph\Ph.qb" lev_scr = Philly
	PrepareLevelFog r = 145 g = 201 b = 242 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Bul
	AddMusicTrack "ambience\thps2\bul"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\Bul\Bul.bsp" lev_sky = "Levels_Th2\Bul\Bul_Sky.bsp" lev_qb = "Levels_Th2\Bul\Bul.qb" lev_scr = Bullring
	PrepareLevelFog r = 145 g = 201 b = 242 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Mar
	AddMusicTrack "ambience\thps2\mar"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\mar\mar.bsp" lev_sky = "Levels_Th2\mar\mar_sky.bsp" lev_qb = "Levels_Th2\mar\mar.qb" lev_scr = Marseille
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Hvn
	AddMusicTrack "ambience\thps2\hvn"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\hvn\hvn.bsp" lev_sky = "Levels_Th2\hvn\hvn_sky.bsp" lev_qb = "Levels_Th2\hvn\hvn.qb" lev_scr = Heaven
	PrepareLevelFog r = 0 g = 0 b = 0 a = 0 cnear = 12 cfar = 17000
ENDSCRIPT


SCRIPT Load_Trn
	AddMusicTrack "ambience\thps4\trn"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Trn\Trn.bsp" lev_sky = "Levels_Th4\Trn\Trn_Sky.bsp" lev_qb = "levels_TH4\Trn\Trn.qb" lev_scr = MotoX
	PrepareLevelFog r = 179 g = 184 b = 192 a = 0 cnear = 12 cfar = 15000
ENDSCRIPT

SCRIPT Load_Alc
	AddMusicTrack "ambience\thps4\alc"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Alc\Alc.bsp" lev_sky = "Levels_Th4\Alc\Alc_Sky.bsp" lev_qb = "levels_TH4\Alc\Alc.qb" lev_scr = Alcatraz
	PrepareLevelFog r = 179 g = 184 b = 192 a = 0 cnear = 12 cfar = 50000
ENDSCRIPT

SCRIPT Load_Sch
	AddMusicTrack "ambience\thps4\sch"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Sch\Sch.bsp" lev_sky = "Levels_Th4\Sch\Sch_Sky.bsp" lev_qb = "levels_TH4\Sch\Sch.qb" lev_scr = College
	PrepareLevelFog r = 179 g = 184 b = 192 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_SF2
	AddMusicTrack "ambience\thps4\sf2"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\SF2\SF2.bsp" lev_sky = "Levels_Th4\SF2\SF2_Sky.bsp" lev_qb = "levels_TH4\SF2\SF2.qb" lev_scr = SanFrancisco
	PrepareLevelFog r = 179 g = 184 b = 192 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Kon
	AddMusicTrack "ambience\thps4\Kon"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Kon\Kon.bsp" lev_sky = "Levels_Th4\Kon\Kon_Sky.bsp" lev_qb = "levels_TH4\Kon\Kon.qb" lev_scr = Kona
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 12 cfar = 80000
ENDSCRIPT

SCRIPT Load_Lon
	AddMusicTrack "ambience\thps4\Lon"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Lon\Lon.bsp" lev_sky = "Levels_Th4\Lon\Lon_Sky.bsp" lev_qb = "levels_TH4\Lon\Lon.qb" lev_scr = London
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 12 cfar = 80000
ENDSCRIPT

SCRIPT Load_Hof
	AddMusicTrack "ambience\thps4\Hof"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Hof\Hof.bsp" lev_sky = "Levels_Th4\Hof\Hof_Sky.bsp" lev_qb = "levels_TH4\Hof\Hof.qb" lev_scr = Chicago
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Mot
	AddMusicTrack "ambience\thps4\Mot"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Mot\Mot.bsp" lev_sky = "Levels_Th4\Mot\Mot_Sky.bsp" lev_qb = "levels_TH4\Mot\Mot.qb" lev_scr = MotoX
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Jnk
	AddMusicTrack "ambience\thps4\Jnk"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Jnk\Jnk.bsp" lev_sky = "Levels_Th4\Jnk\Jnk_Sky.bsp" lev_qb = "levels_TH4\Jnk\Jnk.qb" lev_scr = Shipyard
	PrepareLevelFog r = 179 g = 184 b = 192 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Zoo
	AddMusicTrack "ambience\thps4\zoo"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Zoo\Zoo.bsp" lev_sky = "Levels_Th4\Zoo\Zoo_Sky.bsp" lev_qb = "levels_TH4\Zoo\Zoo.qb" lev_scr = Zoo
	PrepareLevelFog r = 179 g = 184 b = 192 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT Load_Cnv
	AddMusicTrack "ambience\thps4\cnv"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "Levels_Th4\Cnv\Cnv.bsp" lev_sky = "Levels_Th4\Cnv\Cnv_Sky.bsp" lev_qb = "levels_TH4\Cnv\Cnv.qb" lev_scr = Carnival
	PrepareLevelFog r = 179 g = 184 b = 192 a = 0 cnear = 12 cfar = 20000
ENDSCRIPT

SCRIPT THUG1Levels
	LaunchPanelMessage "=========================================THUG1==="
ENDSCRIPT

SCRIPT Load_NJ
	AddMusicTrack "ambience\thug1\nj"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\nj\nj.bsp" lev_sky = "levels_ug1\nj\nj_sky.bsp" lev_qb = "levels_ug1\nj\nj.qb" lev_scr = Jersey
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_NY
	AddMusicTrack "ambience\thug1\ny"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\ny\ny.bsp" lev_sky = "levels_ug1\ny\ny_sky.bsp" lev_qb = "levels_ug1\ny\ny.qb" lev_scr = Manhattan
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_FL
	AddMusicTrack "ambience\thug1\fl"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\fl\fl.bsp" lev_sky = "levels_ug1\fl\fl_sky.bsp" lev_qb = "levels_ug1\fl\fl.qb" lev_scr = Tampa
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SD
	AddMusicTrack "ambience\thug1\sd"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\sd\sd.bsp" lev_sky = "levels_ug1\sd\sd_sky.bsp" lev_qb = "levels_ug1\sd\sd.qb" lev_scr = SanDiego
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_HI
	AddMusicTrack "ambience\thug1\hi"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\hi\hi.bsp" lev_sky = "levels_ug1\hi\hi_sky.bsp" lev_qb = "levels_ug1\hi\hi.qb" lev_scr = Hawaii
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_VC
	AddMusicTrack "ambience\thug1\vc"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\vc\vc.bsp" lev_sky = "levels_ug1\vc\vc_sky.bsp" lev_qb = "levels_ug1\vc\vc.qb" lev_scr = Vancouver
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SJ
	AddMusicTrack "ambience\thug1\sj"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\sj\sj.bsp" lev_sky = "levels_ug1\sj\sj_sky.bsp" lev_qb = "levels_ug1\sj\sj.qb" lev_scr = SlamCityJam
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_RU
	AddMusicTrack "ambience\thug1\ru"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\ru\ru.bsp" lev_sky = "levels_ug1\ru\ru_sky.bsp" lev_qb = "levels_ug1\ru\ru.qb" lev_scr = Moscow
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE
	AddMusicTrack "ambience\thug1\se"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\se\se.bsp" lev_sky = "levels_ug1\se\se_sky.bsp" lev_qb = "levels_ug1\se\se.qb" lev_scr = HotterThanHell
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT THUG2Levels
	LaunchPanelMessage "==========================Global Load Level Function==="
ENDSCRIPT


SCRIPT Load_AU_ug2
	AddMusicTrack "ambience\thug2\AU"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\AU\AU.bsp" lev_sky = "levels_ug2\AU\AU_sky.bsp" lev_qb = "levels_ug2\AU\AU.qb" lev_scr = Australia
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_NO_ug2
	AddMusicTrack "ambience\thug2\NO"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\NO\NO.bsp" lev_sky = "levels_ug2\NO\NO_sky.bsp" lev_qb = "levels_ug2\NO\NO.qb" lev_scr = NewOrleans
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BA_ug2
	AddMusicTrack "ambience\thug2\BA"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\BA\BA.bsp" lev_sky = "levels_ug2\BA\BA_sky.bsp" lev_qb = "levels_ug2\BA\BA.qb" lev_scr = Barcelona
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BE_ug2
	AddMusicTrack "ambience\thug2\BE"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\BE\BE.bsp" lev_sky = "levels_ug2\BE\BE_sky.bsp" lev_qb = "levels_ug2\BE\BE.qb" lev_scr = Berlin
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BO_ug2
	AddMusicTrack "ambience\thug2\BO"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\BO\BO.bsp" lev_sky = "levels_ug2\BO\BO_sky.bsp" lev_qb = "levels_ug2\BO\BO.qb" lev_scr = Boston
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE_ug2
	AddMusicTrack "ambience\thug2\se"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\se\se.bsp" lev_sky = "levels_ug2\se\se_sky.bsp" lev_qb = "levels_ug2\se\se.qb" lev_scr = ProSkater
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE2_ug2
	AddMusicTrack "ambience\thug2\SE2"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\SE2\SE2.bsp" lev_sky = "levels_ug2\SE2\SE2_sky.bsp" lev_qb = "levels_ug2\SE2\SE2.qb" lev_scr = Triangle
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_ST_ug2
	AddMusicTrack "ambience\thug2\ST"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\ST\ST.bsp" lev_sky = "levels_ug2\ST\ST_sky.bsp" lev_qb = "levels_ug2\ST\ST.qb" lev_scr = Skatopia
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT


//don't think default params are actually needed
//if it crashes, fix the struct. we don't want to like load foundry if something's wrong.
SCRIPT Load_Level_Func 
	DisplayProperLoadingScreen <lev_scr>

	IF GotParam lev_amb
		AddMusicTrack <lev_amb>
	ENDIF

	//this sets levelnum for career mode, so bails check doesnt work if skipped
	//make sure levelnum = LevelNum_New is set for levelmod levels
	IF GotParam levelnum
		IF GotParam competition
			CareerStartLevel { level = <levelnum> competition }
		ELSE
			CareerStartLevel { level = <levelnum> }
		ENDIF
	ENDIF

	sky_path change Sky
	printf "Loading Geometry..."
	LoadLevelGeometry { level = <lev_bsp> Sky = <lev_sky> }
	
	IF InNetGame
		IF IsTrue NetSkyIsOn
			printf "should load sky"
			LoadLevelGeometry { Sky = <lev_sky> }
		ELSE
			printf "should not load sky"
			LoadLevelGeometry { Sky = "" }
		ENDIF
	ENDIF
	
	printf "Loading NodeArray"
	LoadNodeArray <lev_qb>
	LoadTerrain
	
	SetBackgroundColor <...>
	
	//basically PrepareLevelFog function
	IF ClipPlaneEnabled
		EnableFog
		SetClippingDistances { near = 12 far = 2500 }
		SetFogDistance { distance = 1250 }
		SetFogColor <...>
	ELSE
		SetClippingDistances { near = <cnear> far = <cfar> }
	ENDIF

	printf "...finshed loading."
ENDSCRIPT


SCRIPT PrepareLevelFog r = 128 b = 128 b = 128 a = 128 cnear = 12 cfar = 10000
	IF ClipPlaneEnabled
		SetupForClipPlane
	ELSE
		SetBackgroundColor <...>
		SetClippingDistances { near = <cnear> far = <cfar> }
	ENDIF
ENDSCRIPT


OnlineModes = { 
	supports_ctf 
	supports_own 
	supports_bball 
}


//=============================Master Level List==============================


//THPS1 WAREHOUSE
Def_Ware = { 
	level_id = warehouse_id 
	levelnum = LevelNum_Warehouse
	
	level_name = "Warehouse" 
	location = "Woodland Hills"
	
	load_script = Load_Ware 

	regular_level NoCareer th1_level OnlineModes
	
	lev_bsp = "levels\ware\ware.bsp"
	lev_qb = "levels\ware\ware.qb"
	lev_amb = "ambience\thps1\ware"
	lev_scr = Warehouse
	
	unlock_flag = LEVEL_UNLOCKED_WAREHOUSE 

	r = 0 g = 0 b = 0 a = 0 
	cnear = 13 cfar = 20000
}

SCRIPT Load_Ware
	SetUpWareLights
	Load_Level_Func { Def_Ware }
	Ware_Startup
ENDSCRIPT


//THPS3 FOUNDRY
Def_Foun = {
	level_id = foundry_id 
	levelnum = LevelNum_Foundry 
	level_name = "Foundry" 
	location = "Woodland Hills" 
	
	regular_level th3_level OnlineModes
	
	once_on_startup = StartRunScript 
	once_on_exit = EndRunScript 
	load_script = Load_Foun 

	lev_bsp = "Levels\foun\foun.bsp" 
	lev_sky = "Levels\foun_Sky\foun_Sky.bsp" 
	lev_qb = "Levels\foun\foun.qb"
	lev_amb = "ambience\th3\foun"
	lev_scr = Foundry
	
	r = 0 g = 0 b = 0 a = 0 
	cnear = 13 cfar = 20000
}

SCRIPT Load_Foun
	SetUpFoundryLights
	Load_Level_Func { Def_Foun }
	Foun_Startup
ENDSCRIPT


//THPS3 CANADA
Def_Can = { 
	level_id = canada_id 
	levelnum = LevelNum_Canada 
	unlock_flag = LEVEL_UNLOCKED_CANADA 
	level_name = "Canada" 
	location = "Canada"
	
	regular_level th3_level OnlineModes
	
	once_on_startup = StartRunScript 
	once_on_exit = EndRunScript 
	load_script = Load_Can
	
	lev_bsp = "Levels\can\can.bsp" 
	lev_sky = "Levels\can_Sky\can_Sky.bsp" 
	lev_qb = "Levels\can\can.qb" 
	lev_amb = "ambience\th3\can"
	lev_scr = Canada
	
	r = 168 g = 176 b = 198 a = 0 
	cnear = 13 cfar = 16000

}

SCRIPT Load_Can
	SetUpCanadaLights
	Load_Level_Func { Def_Can }
	SetMovementVelocity 1500
	SetRotateVelocity 120
	Can_Startup
ENDSCRIPT


//THPS2 HANGAR
Def_Han = { 
	level_id = hangar_id 
	levelnum = LevelNum_New
	level_name = "Hangar" 
	localtion = "Mullet Falls MT"
	load_script = Load_han 
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "Levels_Th2\Han\han.bsp" 
	lev_sky = "Levels_Th2\han\han_sky.bsp" 
	lev_qb = "Levels_Th2\han\han.qb" 
	lev_amb = "ambience\thps2\han"
	lev_scr = Hangar
	
	r = 6 g = 8 b = 12 a = 0 
	cnear = 12 cfar = 10000
} 

SCRIPT Load_han
	Load_Level_Func { Def_Han }
ENDSCRIPT


//THPS2 NEW YORK CITY
Def_NY1 = { 
	level_name = "New York" 
	level_id = newyork_id 
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "Levels_Th2\ny\ny.bsp" 
	lev_sky = "Levels_Th2\ny\ny_Sky.bsp" 
	lev_qb = "Levels_Th2\ny\ny.qb" 
	lev_amb = "ambience\thps2\ny"
	lev_scr = NewYork
	
	r = 4 g = 8 b = 12 a = 0 
	cnear = 12 cfar = 20000
	
	load_script = Load_NY1 
}

SCRIPT Load_NY1
	Load_Level_Func { Def_NY1 }
ENDSCRIPT


//THPS2X CONSTRUCTION SITE
Def_Cons = { 
	level_id = cons_id
	levelnum = LevelNum_New
	level_name = "Construction Site"
	
	NoCareer th2x_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\cons\cons.bsp" 
	lev_sky = "levels_th2\cons\cons_sky.bsp" 
	lev_qb = "levels_th2\cons\cons.qb" 
	lev_amb = "ambience\thps2\cons"
	lev_scr = Cons
	
	r = 106 g = 97 b = 72 a = 0 
	cnear = 12 cfar = 20000
	
	load_script = Load_Cons 
}

SCRIPT Load_Cons
	Load_Level_Func { Def_Cons }
ENDSCRIPT


//THPS2X CLUB
Def_Club = { 
	level_id = club_id 
	levelnum = LevelNum_New
	
	level_name = "Club" 
	location = "London"

	lev_bsp = "Levels_Th2\Club\Club.bsp" 
	lev_qb = "Levels_Th2\Club\Club.qb" 
	lev_amb = "ambience\thps2\club"
	lev_scr = Club
	
	NoCareer th2x_level regular_level OnlineModes
	
	r = 0 g = 0 b = 0 a = 0 
	cnear = 12 cfar = 20000 
	
	load_script = Load_Club 
}

SCRIPT Load_Club
	Load_Level_Func { Def_Club }
ENDSCRIPT


//THPS2X SUBWAY
Def_Sway = { 
	level_name = "Subway" 
	load_script = Load_Sway 
	level_id = sway_id 
	
	lev_bsp = "Levels_Th2\Sway\Sway.bsp" 
	lev_qb = "levels_Th2\Sway\Sway.qb" 
	lev_amb = "ambience\thps2\sway"
	lev_scr = Subway
	
	NoCareer th2x_level regular_level OnlineModes
	
	r = 0 g = 0 b = 0 a = 0 
	cnear = 12 cfar = 20000 
}

SCRIPT Load_Sway
	Load_Level_Func { Def_Sway }
ENDSCRIPT


//THPS2X TAMPA
Def_Tampa = { 
	level_name = "Skate Park Tampa"
	load_script = Load_Tampa 
	level_id = tampa_id 
	
	unlock_flag = FIX_LEVELMOD_FLAG 
	
	lev_bsp = "Levels_Th2\flor\flor.bsp" 
	lev_sky = "Levels_Th2\flor\flor_sky.bsp" 
	lev_qb = "Levels_Th2\flor\flor.qb" 
	lev_amb = "ambience\thps2\flor"
	lev_scr = Tampa
	
	NoCareer th2x_level regular_level OnlineModes 
	
	r = 149 g = 192 b = 232 a = 0 
	cnear = 12 cfar = 20000 
}

SCRIPT Load_Tampa
	Load_Level_Func { Def_Tampa }
ENDSCRIPT


//THPS2X SKY LINES
Def_Sky = { 
	level_name = "Sky Lines" 
	load_script = Load_Sky 
	level_id = skylines_id 
	
	lev_bsp = "Levels_Th2\roof\roof.bsp" 
	lev_sky = "Levels_Th2\roof\roof_sky.bsp" 
	lev_qb = "levels_th2\roof\roof.qb" 
	lev_amb = "ambience\thps2\sky"
	lev_scr = SkyLines
	
	NoCareer th2x_level regular_level OnlineModes

	r = 32 g = 36 b = 63 a = 0 
	cnear = 12 cfar = 20000 
}

SCRIPT Load_Sky
	Load_Level_Func { Def_Sky }
ENDSCRIPT


master_level_list = [ 
	//THPS3
	{ Def_Foun }
	{ Def_Can }
	{ level_name = "Rio" load_script = Load_Rio level_id = rio_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_RIO supports_ctf supports_own supports_bball }
	{ level_name = "Suburbia" load_script = Load_Sub level_id = suburbia_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_SUBURBIA supports_ctf supports_own supports_bball }
	{ level_name = "Airport" load_script = Load_Ap level_id = airport_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_AIRPORT supports_ctf supports_own supports_bball }
	{ level_name = "Skater Island" load_script = Load_Si level_id = skatersisland_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_SKATERISLAND supports_ctf supports_own supports_bball }
	{ level_name = "Los Angeles" load_script = Load_La level_id = la_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_LOSANGELES supports_ctf supports_own supports_bball }
	{ level_name = "Tokyo" load_script = Load_Tok level_id = tokyo_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_TOKYO supports_ctf supports_own supports_bball }
	{ level_name = "Cruise Ship" load_script = Load_Shp level_id = ship_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_SHIP special_flag = SPECIAL_HAS_SEEN_SHIP supports_ctf supports_own supports_bball }
	{ level_name = "Oil Rig" load_script = Load_Oil level_id = oil_id NoCareer regular_level th3_level supports_ctf supports_own supports_bball }
	{ level_name = "Paris" load_script = Load_Roof level_id = rooftops_id NoCareer regular_level th3_level supports_ctf supports_own supports_bball }
	{ level_name = "Play custom park" load_script = custom_park level_id = custom_park debug_level }
	{ level_name = "Tutorials" load_script = Load_Tut level_id = tutorials_id NoCareer once_on_startup = CPF_Tut_LoadingScript }
	{ level_name = "Foo" load_script = Load_Foo level_id = foo_id debug_level }
	{ level_name = "Skate Shop" load_script = Load_SkateShop level_id = skateshop_id debug_level }
	
	//THPS1
	{ Def_Ware }
	{ level_name = "School" load_script = Load_Sc1 level_id = school1_id NoCareer th1_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Mall" load_script = Load_Mal level_id = Mall_id NoCareer th1_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Skate Park Chicago" load_script = Load_Vans level_id = vans_id NoCareer th1_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Downtown" load_script = Load_Down level_id = down_id NoCareer th1_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Downhill Jam" load_script = Load_Jam level_id = jam_id NoCareer th1_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Burnside" load_script = Load_Burn level_id = burnside_id regular_level NoCareer th1_level unlock_flag = LEVEL_UNLOCKED_BURNSIDE supports_ctf supports_own supports_bball }
	{ level_name = "Streets" load_script = Load_SF1 level_id = streets_id NoCareer th1_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Roswell" load_script = Load_Ros level_id = roswell_id NoCareer unlock_flag = LEVEL_UNLOCKED_ROSWELL regular_level th1_level supports_ctf supports_own supports_bball }
	
	//THPS2
	{ Def_Han }
	{ level_name = "School II" load_script = Load_Sc2 level_id = school2_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Marseille" load_script = Load_Mar level_id = marseilles_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ Def_NY1 }
	{ level_name = "Venice Beach" load_script = Load_Ven level_id = venice_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Skate Street" load_script = Load_SSV level_id = skatestreet_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Philadelphia" load_script = Load_Ph level_id = philly_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Bullring" load_script = Load_Bul level_id = bullring_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Chopper Drop" load_script = Load_Drop level_id = drop_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Skate Heaven" load_script = Load_Hvn level_id = heaven_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	
	//THPS2X
	{ Def_Club }
	{ Def_Cons }
	{ Def_Tampa }
	{ Def_Sway }
	{ Def_Sky }
	
	//THPS4	
	{ level_name = "Training" load_script = Load_Trn level_id = training_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "College" load_script = Load_Sch level_id = college_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "San Francisco" load_script = Load_SF2 level_id = sanfrancisco_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "Alcatraz" load_script = Load_Alc level_id = alcatraz_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "Kona" load_script = Load_Kon level_id = kona_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "Shipyard" load_script = Load_Jnk level_id = shipyard_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "London" load_script = Load_Lon level_id = london_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "Zoo" load_script = Load_Zoo level_id = zoo_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "Carnival" load_script = Load_Cnv level_id = carnival_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "Chicago" load_script = Load_Hof level_id = chicago_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	{ level_name = "MotoX" load_script = Load_Mot level_id = motox_id NoCareer regular_level th4_level supports_ctf supports_own supports_bball }
	
	//THUG
	{ level_name = "New Jersey" load_script = Load_NJ level_id = jersey_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Manhattan" load_script = Load_NY level_id = manhattan_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Tampa" load_script = Load_FL level_id = tampa_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "San Diego" load_script = Load_SD level_id = sandiego_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Hawaii" load_script = Load_HI level_id = hawaii_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Vancouver" load_script = Load_VC level_id = vancouver_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Slam City Jam" load_script = Load_SJ level_id = slam_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Moscow" load_script = Load_RU level_id = moscow_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Hotter Than Hell" load_script = Load_SE level_id = hotter_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	
	//THUG2
	{ level_name = "Boston" load_script = Load_BO_ug2 level_id = BO_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "Barcelona" load_script = Load_BA_ug2 level_id = BA_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "Berlin" load_script = Load_BE_ug2 level_id = BE_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "Australia" load_script = Load_AU_ug2 level_id = AU_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "New Orleans" load_script = Load_NO_ug2 level_id = NO_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }	
	{ level_name = "Skatopia" load_script = Load_ST_ug2 level_id = ST_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "Pro Skater" load_script = Load_SE_ug2 level_id = SE_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "The Triangle" load_script = Load_SE2_ug2 level_id = SE2_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
]
