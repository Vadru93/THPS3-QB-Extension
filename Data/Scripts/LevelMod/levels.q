
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
NetSkyIsOn = 1
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

UnnamedString = ""

SCRIPT LevelLoadingScreens
	IF GotParam Foundry
		ChangeParamToUnnamed param = lev_scr function = "DisplayLoadingScreen"
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
	CareerStartLevel level = LevelNum_Rio
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
	CareerStartLevel level = LevelNum_Tokyo
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

SCRIPT Load_han
	AddMusicTrack "ambience\thps2\han"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\Han\han.bsp" lev_sky = "Levels_Th2\han\han_sky.bsp" lev_qb = "Levels_Th2\han\han.qb" lev_scr = Hangar
	PrepareLevelFog r = 6 g = 8 b = 12 a = 0 cnear = 12 cfar = 10000
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

SCRIPT Load_NY1
	AddMusicTrack "ambience\thps2\ny"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "Levels_Th2\ny\ny.bsp" lev_sky = "Levels_Th2\ny\ny_Sky.bsp" lev_qb = "Levels_Th2\ny\ny.qb" lev_scr = NewYork
	PrepareLevelFog r = 4 g = 8 b = 12 a = 0 cnear = 12 cfar = 20000
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
	Load_Level_Func lev_bsp = "levels_ug1\nj\nj.bsp" lev_sky = "levels_ug1\nj\nj_sky.bsp" lev_qb = "levels_ug1\nj\nj.qb" lev_scr = Jersey
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_NY
	AddMusicTrack "ambience\thug1\ny"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug1\ny\ny.bsp" lev_sky = "levels_ug1\ny\ny_sky.bsp" lev_qb = "levels_ug1\ny\ny.qb" lev_scr = Manhattan
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_FL
	AddMusicTrack "ambience\thug1\fl"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug1\fl\fl.bsp" lev_sky = "levels_ug1\fl\fl_sky.bsp" lev_qb = "levels_ug1\fl\fl.qb" lev_scr = Tampa
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SD
	AddMusicTrack "ambience\thug1\sd"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug1\sd\sd.bsp" lev_sky = "levels_ug1\sd\sd_sky.bsp" lev_qb = "levels_ug1\sd\sd.qb" lev_scr = SanDiego
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_HI
	AddMusicTrack "ambience\thug1\hi"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug1\hi\hi.bsp" lev_sky = "levels_ug1\hi\hi_sky.bsp" lev_qb = "levels_ug1\hi\hi.qb" lev_scr = Hawaii
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_VC
	AddMusicTrack "ambience\thug1\vc"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug1\vc\vc.bsp" lev_sky = "levels_ug1\vc\vc_sky.bsp" lev_qb = "levels_ug1\vc\vc.qb" lev_scr = Vancouver
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SJ
	AddMusicTrack "ambience\thug1\sj"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug1\sj\sj.bsp" lev_sky = "levels_ug1\sj\sj_sky.bsp" lev_qb = "levels_ug1\sj\sj.qb" lev_scr = SlamCityJam
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_RU
	AddMusicTrack "ambience\thug1\ru"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug1\ru\ru.bsp" lev_sky = "levels_ug1\ru\ru_sky.bsp" lev_qb = "levels_ug1\ru\ru.qb" lev_scr = Moscow
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE
	AddMusicTrack "ambience\thug1\se"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug1\se\se.bsp" lev_sky = "levels_ug1\se\se_sky.bsp" lev_qb = "levels_ug1\se\se.qb" lev_scr = HotterThanHell
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT THUG2Levels
	LaunchPanelMessage "==========================Global Load Level Function==="
ENDSCRIPT


SCRIPT Load_AU_ug2
	AddMusicTrack "ambience\thug2\AU"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug2\AU\AU.bsp" lev_sky = "levels_ug2\AU\AU_sky.bsp" lev_qb = "levels_ug2\AU\AU.qb" lev_scr = Australia
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_NO_ug2
	AddMusicTrack "ambience\thug2\NO"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug2\NO\NO.bsp" lev_sky = "levels_ug2\NO\NO_sky.bsp" lev_qb = "levels_ug2\NO\NO.qb" lev_scr = NewOrleans
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BA_ug2
	AddMusicTrack "ambience\thug2\BA"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug2\BA\BA.bsp" lev_sky = "levels_ug2\BA\BA_sky.bsp" lev_qb = "levels_ug2\BA\BA.qb" lev_scr = Barcelona
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BE_ug2
	AddMusicTrack "ambience\thug2\BE"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug2\BE\BE.bsp" lev_sky = "levels_ug2\BE\BE_sky.bsp" lev_qb = "levels_ug2\BE\BE.qb" lev_scr = Berlin
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BO_ug2
	AddMusicTrack "ambience\thug2\BO"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug2\BO\BO.bsp" lev_sky = "levels_ug2\BO\BO_sky.bsp" lev_qb = "levels_ug2\BO\BO.qb" lev_scr = Boston
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE_ug2
	AddMusicTrack "ambience\thug2\se"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug2\se\se.bsp" lev_sky = "levels_ug2\se\se_sky.bsp" lev_qb = "levels_ug2\se\se.qb" lev_scr = ProSkater
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE2_ug2
	AddMusicTrack "ambience\thug2\SE2"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug2\SE2\SE2.bsp" lev_sky = "levels_ug2\SE2\SE2_sky.bsp" lev_qb = "levels_ug2\SE2\SE2.qb" lev_scr = Triangle
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_ST_ug2
	AddMusicTrack "ambience\thug2\ST"
	SetUpRioDeJaneiroLights
	Load_Level_Func lev_bsp = "levels_ug2\ST\ST.bsp" lev_sky = "levels_ug2\ST\ST_sky.bsp" lev_qb = "levels_ug2\ST\ST.qb" lev_scr = Skatopia
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT



SCRIPT TestNoParam
	PlaySound <...>
ENDSCRIPT


SCRIPT Load_Level_Func { 
	levelnum = LevelNum_New 
	lev_bsp = "" 
	lev_sky = "" 
	lev_qb = "" 
	lev_scr = "images\load\foun.png"
	r = 128 b = 128 b = 128 a = 128 
	cnear = 12 cfar = 20000 
}
    //ChangeString string = UnnamedString param = lev_scr
	DisplayProperLoadingScreen <...>

	IF GotParam lev_amb
		AddMusicTrack { <lev_amb> }
	ENDIF

	//ChangeParamToUnnamed function = TestNoParam param = snd
	
	//wonder if need outside career mode
	
	IF IsCareerMode
		CareerStartLevel { level = <levelnum> }
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
	LoadNodeArray { <lev_qb> }
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
	Warehouse
	
	unlock_flag = LEVEL_UNLOCKED_WAREHOUSE 

	r = 0 g = 0 b = 0 a = 0 
	cnear = 13 cfar = 20000
	
	snd = HUD_specialtrickAA
}

SCRIPT Load_Ware
	SetUpWareLights
	Load_Level_Func { Def_Ware }
	//Ware_Startup
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
	Foundry
	//lev_scr = "images\loadscrn_oil.png" //"images\loadscrn_foundry.png"

	r = 0 g = 0 b = 0 a = 0 
	cnear = 13 cfar = 20000
}

SCRIPT Load_Foun
	SetUpFoundryLights
	Load_Level_Func { Def_Foun }
	//Foun_Startup
ENDSCRIPT


//THPS3 CANADA
Def_Can = { 
	level_id = canada_id 
	levelnum = LevelNum_Canada 
	unlock_flag = LEVEL_UNLOCKED_CANADA 
	
	level_name = "Canada" 
	location = "Canada"
	
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
	
	regular_level th3_level OnlineModes
}

SCRIPT Load_Can
	SetUpCanadaLights
	Load_Level_Func { Def_Can }
	SetMovementVelocity 1500
	SetRotateVelocity 120
	Can_Startup
ENDSCRIPT


//THPS2X CONSTRUCTION SITE
Def_Cons = { 
	level_name = "Construction Site" 
	load_script = Load_Cons 
	level_id = cons_id

	lev_bsp = "Levels_Th2\cons\cons.bsp" 
	lev_sky = "Levels_Th2\cons\cons_Sky.bsp" 
	lev_qb = "levels_Th2\cons\cons.qb" 
	lev_amb = "ambience\thps2\cons"
	lev_scr = Cons
	
	r = 106 g = 97 b = 72 a = 0 
	cnear = 12 cfar = 20000
	
	NoCareer th2x_level regular_level OnlineModes
}

SCRIPT Load_Cons
	Load_Level_Func { Def_Cons }
ENDSCRIPT


//THPS2X CLUB
Def_Club = { 
	level_name = "Club" 
	location = "London"
	load_script = Load_Club 
	level_id = club_id 
	
	lev_bsp = "Levels_Th2\Club\Club.bsp" 
	lev_qb = "Levels_Th2\Club\Club.qb" 
	lev_amb = "ambience\thps2\club"
	lev_scr = Club
	
	NoCareer th2x_level regular_level OnlineModes
	
	r = 0 g = 0 b = 0 a = 0 
	cnear = 12 cfar = 20000 
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
	{ level_name = "Hangar" load_script = Load_han level_id = hangar_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "School II" load_script = Load_Sc2 level_id = school2_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "Marseille" load_script = Load_Mar level_id = marseilles_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
	{ level_name = "New York" load_script = Load_NY1 level_id = newyork_id NoCareer th2_level regular_level supports_ctf supports_own supports_bball }
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
	
	
	

//====================================== Gamespy menu section ===============================================


// ==================================colors section=========================================

ui_colors = { 
	ui_white = [ 160 160 160 128 ] 
	ui_dark_white = [ 96 96 96 128 ] 
	ui_blue = [ 0 0 255 128 ] 
	ui_dark_blue = [ 0 0 96 128 ] 
	ui_red = [ 255 0 0 128 ] 
	ui_dark_red = [ 96 0 0 128 ] 
	ui_green = [ 0 255 0 128 ] 
	ui_dark_green = [ 0 96 0 128 ] 
	ui_yellow = [ 255 255 0 128 ] 
	ui_dark_yellow = [ 96 96 0 128 ] 
	ui_cyan = [ 32 255 255 128 ] 
	ui_dark_cyan = [ 8 96 96 128 ] 
	ui_invisible = [ 0 0 0 0 ] 
	ui_t50_dk_white = [ 96 96 96 64 ] 
	ui_t50_dk_red = [ 96 0 0 64 ] 
	ui_t50_dk_green = [ 0 96 0 64 ] 
	ui_t50_dk_blue = [ 0 0 96 64 ] 
}

indexed_ui_colors = [ 
	ui_white 
	ui_dark_white 
	ui_blue 
	ui_dark_blue 
	ui_red 
	ui_dark_red
	ui_green 
	ui_dark_green 
	ui_yellow 
	ui_dark_yellow 
	ui_cyan 
	ui_dark_cyan 
	ui_t50_dk_white 
	ui_t50_dk_red 
	ui_t50_dk_green 
	ui_t50_dk_blue 
	ui_invisible 
]

invisible_colors = { 
	normal = ui_invisible 
	highlight = ui_invisible 
	selected = ui_invisible 
	hot = ui_invisible 
	gray = ui_invisible 
}

element_colors = { 
	normal = [ 255 255 255 128 ] 
	highlight = [ 255 255 255 128 ] 
	selected = [ 255 255 255 128 ] 
	gray = [ 255 255 255 128 ] 
}

black_colors = { 
	normal = [ 0 0 0 128 ] 
	highlight = [ 0 0 0 128 ] 
	selected = [ 0 0 0 128 ] 
	gray = [ 0 0 0 128 ] 
}

image_button_highlight_colors = { 
	normal = [ 128 128 128 128 ] 
	highlight = [ 255 255 255 128 ] 
	selected = [ 192 192 192 128 ] 
	gray = [ 64 64 64 128 ] 
	hot = [ 192 192 192 128 ] 
}

on_off_colors = { 
	normal = [ 255 255 255 128 ] 
	highlight = [ 255 255 255 128 ] 
	selected = [ 255 255 255 128 ] 
	gray = [ 64 64 64 128 ] 
}

bright_colors = { 
	normal = [ 255 255 255 128 ]
	highlight = [ 255 255 255 128 ] 
	selected = [ 255 255 255 128 ] 
	gray = [ 255 255 255 128 ] 
}

gamespy_colors =
{ normal = [ 128 128 128 128 ] highlight = [ 128 128 128 128 ] selected = [ 128 128 128 128 ] gray = [ 128 128 128 128 ] }
border_colors_1 =
{ normal = [ 128 128 128 128 ] highlight = [ 255 255 255 128 ] selected = [ 192 192 192 128 ] gray = [ 64 64 64 128 ] hot = [ 192 192 192 128 ] }
img_fg_hilite_colors_1 =
{ normal = [ 128 128 128 128 ] highlight = [ 255 255 255 128 ] selected = [ 128 128 128 128 ] gray = [ 96 96 96 128 ] hot = [ 192 192 192 128 ] }
border_colors_2 =
{ normal = [ 128 128 128 64 ] highlight = [ 255 255 255 64 ] selected = [ 192 192 192 64 ] gray = [ 64 64 64 64 ] hot = [ 255 255 255 64 ] }
colors_scroll_background =
{ normal = [ 0 0 0 128 ] highlight = [ 0 0 192 128 ] selected = [ 0 0 128 128 ] gray = [ 0 0 0 128 ] }
bg_window_foreground_colors =
{ normal = [ 192 192 192 80 ] highlight = [ 192 192 192 80 ] selected = [ 192 192 192 80 ] gray = [ 192 192 192 80 ] }
client_area_colors =
{ normal = [ 16 16 16 64 ] highlight = [ 16 16 16 64 ] selected = [ 16 16 16 64 ] gray = [ 16 16 16 64 ] }
list_item_highlight_colors =
{ normal = [ 0 0 0 0 ] highlight = [ 0 0 192 128 ] selected = [ 0 0 128 128 ] gray = [ 0 0 0 0 ] hot = [ 0 0 128 128 ] }
button_bg_colors =
{ normal = [ 96 48 12 128 ] selected = [ 96 96 96 128 ] highlight = [ 0 0 128 128 ] hot = [ 135 65 15 128 ] gray = [ 32 16 4 128 ] }
orange_colors =
{ normal = [ 135 65 15 128 ] selected = [ 135 65 15 128 ] highlight = [ 135 65 15 128 ] hot = [ 135 65 15 128 ] gray = [ 66 32 8 128 ] }
image_button_orange_highlight_colors =
{ normal = [ 135 65 15 128 ] highlight = [ 255 130 30 128 ] selected = [ 192 97 23 128 ] gray = [ 64 32 7 128 ] hot = [ 192 97 23 128 ] }
button_fg_colors =
{ normal = [ 255 255 255 128 ] highlight = [ 255 255 255 128 ] selected = [ 255 255 255 128 ] gray = [ 64 64 64 128 ] }
playeritem_button_colors =
{ normal = [ 96 96 96 128 ] selected = [ 96 96 96 128 ] highlight = [ 192 192 0 128 ] hot = [ 128 128 128 128 ] gray = [ 64 64 64 128 ] }
cap_icon_colors =
{ normal = ui_white highlight = ui_white selected = ui_white hot = ui_white gray = [ 64 64 64 128 ] }
mode_icon_bg_colors =
{ normal = [ 135 65 15 128 ] selected = [ 135 65 15 128 ] highlight = [ 135 65 15 128 ] hot = [ 135 65 15 128 ] gray = [ 135 65 15 128 ] }

slider_thumb_properties = { 
	checksum = slider_thumb_properties 
	x = 0.0 y = 0.0 
	type = imageelement noselect stretch draggable background 
	foreground_colors = image_button_highlight_colors 
	style = pushbutton 
	texture_list = [ "GSpyMenu\scroll.png" "GSpyMenu\scroll.png" "GSpyMenu\scroll.png" ] 
}

slider_button_properties =
{ checksum = slider_button_properties x = 0.0 y = 0.0 type = imageelement noselect stretch repeattime = 100 foreground_colors = image_button_highlight_colors style = pushbutton texture_list = [ "GSpyMenu\arrow.png" ] }
slider_element_properties =
{ checksum = slider_element_properties type = sliderelement style = scrollbar x = 0.0 y = 0.0 size = 14 background_colors = element_colors border_colors = element_colors vertical noselect repeattime = 100 thumb_style = slider_thumb_properties button_style = slider_button_properties forceclip drawfront }
background_window_properties =
{ checksum = background_window_properties type = imageelement style = background scalex = 0.5 scaley = 0.5 windowclip static dont_gray texture_list = [ "GSpyMenu\stretchmenu1.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\lowercorner4.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\lowercorner.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\stretchmenu1.png" ] foreground_colors = bg_window_foreground_colors drawbehind align_x = { parent full } align_y = { parent full } }
background_full_round_window_properties =
{ checksum = background_full_round_window_properties type = imageelement style = background scalex = 0.5 scaley = 0.5 windowclip static dont_gray texture_list = [ "GSpyMenu\lowercorner2.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\lowercorner3.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\lowercorner4.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\lowercorner.png" "GSpyMenu\stretchmenu1.png" "GSpyMenu\stretchmenu1.png" ] foreground_colors = bg_window_foreground_colors align_x = { parent full } align_y = { parent full } }
background_title_window_properties =
{ checksum = background_title_window_properties type = imageelement style = background windowclip static dont_gray texture_list = [ "GSpyMenu\topper2.png" "GSpyMenu\topperstretch.png" "GSpyMenu\topper1.png" "GSpyMenu\topperstretch.png" "GSpyMenu\topperstretch.png" "GSpyMenu\topperstretch.png" "GSpyMenu\topperstretch.png" "GSpyMenu\topperstretch.png" "GSpyMenu\topperstretch.png" ] foreground_colors = bg_window_foreground_colors align_x = { parent full } align_y = { parent full } }
client_area_properties =
{ checksum = client_area_properties type = uielement x = 0 y = 0 w = 1 h = 1 align_x = { parent client full } align_y = { parent client full } noselect static dont_gray background drawbehind background_colors = client_area_colors }
game_modeicon_properties =
{ checksum = game_modeicon_properties type = imageelement w = 32.0 h = 32.0 noselect static dont_gray stretch foreground_colors = bright_colors background_colors = mode_icon_bg_colors background style = imagelist activetexture = 0 texture_list = [ "GSpyMenu\icon-versus.png" "GSpyMenu\icon-versus.png" "GSpyMenu\icon-spray.png" "GSpyMenu\icon-slappers.png" "GSpyMenu\icon-koth.png" "GSpyMenu\icon-ball.png" "GSpyMenu\icon-otz.png" "GSpyMenu\icon-ctf.png" ] }
game_status_properties =
{ checksum = game_status_properties x = 40 y = 2 w = 16.0 h = 16.0 type = imageelement noselect static dont_gray stretch foreground_colors = bright_colors style = imagelist activetexture = 0 texture_list = [ "GSpyMenu\no.png" "GSpyMenu\play.png" ] }
game_lock_properties =
{ checksum = game_lock_properties type = imageelement x = 14 y = 5 w = 8.0 h = 8.0 noselect static dont_gray stretch foreground_colors = bright_colors style = imagelist activetexture = 0 texture_list = [ "GSpyMenu\locked.png" ] }
game_players_properties =
{ checksum = game_players_properties type = textelement y = 50 w = 64.0 h = 12.0 texheight = 9 spacing = 1 align_x = { parent client left_left right_right } just_top_y noselect static dont_gray foreground_colors = bright_colors truncate drawer = keyboard_property max_text = 1 }
game_name_properties =
{ checksum = game_name_properties type = textelement y = 38 w = 64.0 h = 12.0 texheight = 9 spacing = 1 align_x = { parent client left_left right_right } just_top_y noselect static dont_gray foreground_colors = bright_colors truncate drawer = keyboard_property max_text = 1 }

game_item_properties = { 
	checksum = game_item_properties 
	type = uielement 
	w = 80.0 h = 64.0 
	parent_aligned 
	border = 1.0 
	padding_h = 0.0 padding_v = 2.0 
	border_colors = bright_colors 
	border_when_selected 
	children = [ 
		{ checksum = game_name_properties }
		{ checksum = game_players_properties }
		{ checksum = game_lock_properties }
		{ checksum = game_ping_properties }
		{ checksum = game_status_properties }
		{ 
			checksum = game_modeicon_properties 
			parms = { 
				align_x = { parent client center } 
				align_y = { parent client top_top } 
			} 
		} 
	] 
}

game_ping_properties = { 
	checksum = game_ping_properties 
	type = pingelement 
	x = 58 y = 2 w = 12.0 h = 22.0 
	parent_aligned noselect static dont_gray 
	value_table = [ 
		{ dots = 8 val = 20 col = 1 }
		{ dots = 7 val = 50 col = 1 }
		{ dots = 6 val = 100 col = 1 }
		{ dots = 5 val = 150 col = 2 }
		{ dots = 4 val = 200 col = 2 }
		{ dots = 3 val = 300 col = 2 }
		{ dots = 2 val = 400 col = 3 }
		{ dots = 1 val = 500 col = 3 } 
	] 
	foreground_colors = { 
		normal = [ 128 128 128 128 ] 
		highlight = [ 0 255 0 128 ] 
		selected = [ 255 128 0 128 ] 
		gray = [ 196 0 0 128 ]
	} 
}

window_title_properties =
{ checksum = window_title_properties type = textelement w = 128.0 h = 32.0 texheight = 20 just_center_y just_left_x padding_h = 20 spacing = 0 parent_aligned noselect static dont_gray foreground_colors = bright_colors background = background_title_window_properties truncate drawer = title max_text = 1 }
game_server_list_scroller =
{ checksum = game_server_list_scroller type = sliderelement style = scrollbar x = 0.0 y = 0.0 size = 14 background_colors = element_colors vertical noselect ticks = (64, 192) repeattime = 100 thumb_style = slider_thumb_properties button_style = slider_button_properties forceclip drawfront }
game_server_list =
{ checksum = game_server_list id = gs_server_list type = scrollwindow style = scroll x = 0.0 y = 0.0 w = 400.0 h = 220.0 spacing_h = 8.0 spacing_v = 4.0 parent_aligned parent_resized border_colors = border_colors_2 border = 1.0 noselect scrollv = game_server_list_scroller children = [ { checksum = client_area_properties } ] }
game_create_window =
{ checksum = game_create_window id = gs_create_window type = uielement x = 0.0 y = 0.0 w = 400.0 h = 220.0 parent_aligned parent_resized border_colors = border_colors_2 border = 1.0 noselect children = [ { checksum = player_name_entry parms = { id = gs_server_name x = 10 y = 25 text = "http://levelmod4.tk" maxentrychars = 255 tip = "Enter a name for your server." caption = { checksum = text_caption parms = { text = "Server Name" } } } }
		{ checksum = player_name_entry parms = { id = gs_cr_password text = "" maxentrychars = 8 tip = "Enter a password for your server." x = 10 y = 70 caption = { checksum = text_caption parms = { text = "Password" } } } }
		{ checksum = full_combo_box_def parms = { id = gs_max_players x = 180 y = 25 w = 170 choices = choices_num_players tip = "Set the maximum number of players in your game." caption = { checksum = text_caption parms = { text = "Max Players" } } } }
		{ checksum = client_area_properties } ] }
game_staging_window =
{ checksum = game_staging_window id = gs_staging_window type = uielement x = 0.0 y = 0.0 w = 400.0 h = 220.0 parent_aligned parent_resized border_colors = border_colors_2 border = 1.0 noselect children = [ { checksum = full_combo_box_def_2 parms = { id = gs_staging_ready x = 10 y = 25 style = checkbox prompt = "Ready" } }
		{ checksum = room_players_list parms = { id = gs_staging_players_list x = 180 y = 25 w = 190 h = 140 style = tiledvertical caption = { checksum = text_caption parms = { text = "Joined Players" } } } }
		{ checksum = client_area_properties } ] }
game_options_window =
{ checksum = game_options_window id = gs_options_window type = uielement x = 0.0 y = 0.0 w = 400.0 h = 220.0 parent_aligned parent_resized border_colors = border_colors_2 border = 1.0 noselect children = [ { checksum = player_name_entry parms = { id = gs_nick_entry x = 10 y = 25 text = "http://levelmod4.tk" tip = "Enter a nick for GameSpy." caption = { checksum = text_caption parms = { text = "GameSpy Nick" } } } }
		{ checksum = full_combo_box_def parms = { id = gs_update_speed style = dropdown choices = choices_update_speed x = 10 y = 70 h = 100 tip = "Choose a higher setting for higher bandwidth connections." caption = { checksum = text_caption parms = { text = "Server Update Speed" } } } }
		{ checksum = client_area_properties } ] }
game_filters_window =
{ checksum = game_filters_window id = gs_filters_window type = uielement x = 0.0 y = 0.0 w = 400.0 h = 220.0 parent_aligned parent_resized border_colors = border_colors_2 border = 1.0 noselect children = [ { checksum = full_combo_box_def parms = { id = gs_filter_ping x = 5 y = 25 h = 100 style = dropdown choices = choices_connect_speed caption = { checksum = text_caption parms = { text = "Connection speed" } } } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_empty x = 5 y = 70 style = checkbox prompt = "Show empty servers" caption = { checksum = text_caption parms = { text = "Server filters" } } } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_full x = 5 y = 90 style = checkbox prompt = "Show full servers" } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_started x = 5 y = 110 style = checkbox prompt = "Show started games" } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_private x = 5 y = 130 style = checkbox prompt = "Show password games" } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_ta x = 170 y = 25 style = checkbox prompt = "Trick Attack" caption = { checksum = text_caption parms = { text = "Show game types" } } } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_graffiti x = 170 y = 45 style = checkbox prompt = "Graffiti" } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_slap x = 170 y = 65 style = checkbox prompt = "Slap!" } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_koth x = 170 y = 85 style = checkbox prompt = "King of the Hill" } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_ball x = 170 y = 105 style = checkbox prompt = "Keep-Away" } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_zone x = 170 y = 125 style = checkbox prompt = "Control Zone" } }
		{ checksum = full_combo_box_def_2 parms = { id = gs_filter_ctf x = 170 y = 145 style = checkbox prompt = "Capture the Flag" } }
		{ checksum = client_area_properties } ] }
game_serverinfo_window =
{ checksum = game_serverinfo_window id = gs_serverinfo_window type = uielement x = 0.0 y = 0.0 w = 400.0 h = 220.0 parent_aligned parent_resized border_colors = border_colors_2 border = 1.0 noselect children = [ { checksum = static_text parms = { id = gs_si_name x = 5 y = 5 h = 20 just_center_x align_x = { parent client full } texheight = 16 text = "Server Name" truncate } }
		{ checksum = static_text parms = { id = gs_si_game_name x = 5 y = 75 h = 16 just_left_x texheight = 12 text = "Trick Attack" } }
		{ checksum = static_text parms = { id = gs_si_game_open x = 5 y = 95 h = 16 just_left_x texheight = 12 text = "Staging" } }
		{ checksum = player_name_entry parms = { id = gs_join_password maxentrychars = 8 text = "" tip = "This server requires a password to join." x = 10 y = 150 caption = { checksum = text_caption parms = { text = "Password" } } } }
		{ checksum = game_ping_properties parms = { id = gs_si_ping x = 5 y = 35 } }
		{ checksum = game_status_properties parms = { id = gs_si_status x = 40 y = 35 } }
		{ checksum = game_modeicon_properties parms = { id = gs_si_mode x = 24 y = 35 } }
		{ checksum = text_players_list parms = { id = gs_si_players h = 120 caption = { checksum = text_caption parms = { text = "Players" } } } }
		{ checksum = client_area_properties } ] }
text_players_list =
{ checksum = text_players_list type = textelement x = 240 y = 50 w = 120 h = 140 drawer = keyboard_property texheight = 14 truncate max_text = 8 spacing = 2.0 just_left_x just_top_y padding_h = 2.0 padding_v = 2.0 foreground_colors = bright_colors background_colors = black_colors border_colors = border_colors_2 border = 1.0 noselect }
gs_sb_text =
{ checksum = gs_sb_text type = textelement w = 30.0 texheight = 10 padding_h = 1.0 parent_aligned just_center_y static dont_gray foreground_colors = bright_colors background_colors = invisible_colors border_colors = border_colors_2 border = 1.0 noselect truncate drawer = keyboard_property max_text = 1 }
gs_sb_progress =
{ checksum = gs_sb_progress type = sliderelement style = progress parent_aligned w = 48 padding_h = 2.0 padding_v = 1.0 static dont_gray background_colors = element_colors border_colors = border_colors_2 border = 1.0 noselect thumb_style = slider_thumb_properties forceclip }
server_statusbar =
{ checksum = server_statusbar type = scrollwindow style = tiledhorizontal parent_aligned x = 0.0 y = 0.0 w = 400.0 h = 18.0 noselect background_colors = client_area_colors border_colors = border_colors_2 noselect children = [ { checksum = gs_sb_text parms = { id = gs_tooltip parent_resized just_left_x text = "This is the help window" } }
		{ checksum = gs_sb_progress parms = { id = gs_progress } }
		{ checksum = gs_sb_text parms = { id = gs_server_count w = 64 just_center_x text = "0" } }
		{ checksum = client_area_properties } ] }
game_server_list_window =
{ checksum = game_server_list_window type = scrollwindow style = tiledvertical parent_aligned parent_resized x = 0.0 y = 270.0 w = 400.0 h = 80.0 noselect background = background_window_properties padding_h = 8.0 padding_v = 4.0 children = [ { checksum = game_options_window }
		{ checksum = game_serverinfo_window }
		{ checksum = game_filters_window }
		{ checksum = game_staging_window }
		{ checksum = game_create_window }
		{ checksum = game_server_list }
		{ checksum = server_statusbar } ] }
server_info_window =
{ checksum = server_info_window type = scrollwindow style = tiledvertical x = 10.0 y = 0.0 w = 400.0 h = 265.0 noselect border_colors = border_colors_1 children = [ { checksum = window_title_properties parms = { id = gs_server_window_title text = "Server List" } }
		{ checksum = server_toolbar }
		{ checksum = game_server_list_window } ] }
text_button =
{ checksum = text_button type = textelement x = 0.0 y = 0.0 w = 32.0 h = 32.0 drawer = keyboard_property texheight = 12 just_center_y hot parent_aligned border = 1.0 background foreground_colors = button_fg_colors background_colors = button_bg_colors border_colors = border_colors_1 }
simple_image_button =
{ checksum = simple_image_button type = imageelement style = pushbutton stretch x = 0.0 y = 0.0 w = 32.0 h = 32.0 parent_aligned hot foreground_colors = img_fg_hilite_colors_1 background_colors = button_bg_colors border_colors = border_colors_1 }
text_caption =
{ checksum = text_caption type = textelement x = 0.0 y = 0.0 w = 32.0 h = 16.0 align_y = { parent bottom_top } align_x = { parent full } nocliptoparent static dont_gray drawer = keyboard_property texheight = 10 just_center_y just_left_x foreground_colors = bright_colors background_colors = black_colors border_colors = border_colors_1 }
static_text =
{ checksum = static_text type = textelement x = 0.0 y = 0.0 w = 32.0 h = 16.0 static dont_gray drawer = keyboard_property texheight = 10 foreground_colors = bright_colors background_colors = black_colors border_colors = border_colors_1 }
server_toolbar =
{ checksum = server_toolbar type = scrollwindow style = tiledhorizontal x = 0.0 y = 0.0 w = 400.0 h = 32.0 parent_aligned spacing_h = 5.0 padding_h = 8.0 noselect background_colors = black_colors background children = [ { checksum = simple_image_button parms = { id = gs_connect texture_list = [ "GSpyMenu\join.png" ] tip = "Connect to GameSpy!" } }
		{ checksum = simple_image_button parms = { id = gs_create_game texture_list = [ "GSpyMenu\start.png" ] tip = "Create a new staging room." } }
		{ checksum = simple_image_button parms = { id = gs_start_game texture_list = [ "GSpyMenu\start.png" ] tip = "Start the staging room or game." } }
		{ checksum = simple_image_button parms = { id = gs_join_game texture_list = [ "GSpyMenu\join.png" ] tip = "Join this game." } }
		{ checksum = simple_image_button parms = { id = gs_filters texture_list = [ "GSpyMenu\filter.png" ] tip = "Customize server list filters." } }
		{ checksum = simple_image_button parms = { id = gs_refresh texture_list = [ "GSpyMenu\refresh.png" ] tip = "Refresh the server list." } }
		{ checksum = simple_image_button parms = { id = gs_stop texture_list = [ "GSpyMenu\stop.png" ] tip = "Stop refreshing the server list." } }
		{ checksum = simple_image_button parms = { id = gs_back texture_list = [ "GSpyMenu\back.png" ] tip = "Go back to previous screen." } }
		{ checksum = simple_image_button parms = { id = gs_options texture_list = [ "GSpyMenu\settings.png" ] tip = "Configure GameSpy." } }
		{ checksum = simple_image_button parms = { id = gs_done texture_list = [ "GSpyMenu\quit.png" ] tip = "Return to the main menu." } }
		{ checksum = full_combo_box_def parms = { id = gs_game_sort style = chooser choices = choices_sorting x = 170 y = 13 h = 120 align_x = { parent client right_right } caption = { checksum = text_caption parms = { text = "Sort" } } } } ] }
text_entry_bar =
{ checksum = text_entry_bar type = textelement x = 0.0 y = 0.0 w = 128.0 h = 18.0 parent_aligned hot drawer = keyboard_string texheight = 12 border = 1.0 background just_left_x foreground_colors = gamespy_colors background_colors = black_colors border_colors = border_colors_2 max_text = 1 padding_h = 2.0 padding_v = 1.0 maxentrychars = 200 textentry truncate }
text_screen =
{ checksum = text_screen type = textelement x = 0.0 y = 0.0 w = 400.0 h = 100.0 parent_aligned parent_resized drawer = keyboard_string texheight = 10 just_left_x just_bottom_y foreground_colors = bright_colors background_colors = black_colors border_colors = border_colors_2 border = 1.0 noselect max_text = 200 spacing = 2.0 scrollv = slider_element_properties wrap children = [ { checksum = client_area_properties } ] }
chat_window =
{ checksum = chat_window type = scrollwindow style = tiledvertical x = 10.0 y = 270.0 w = 400.0 h = 175.0 noselect children = [ { checksum = window_title_properties parms = { text = "Chat" } }
		{ checksum = chat_text_window parms = { padding_h = 8.0 padding_v = 4.0 background = background_window_properties } } ] }
gs_logo_properties =
{ checksum = game_lock_properties type = imageelement w = 213.0 h = 50.0 x = 418 y = 388 noselect static dont_gray stretch foreground_colors = gamespy_colors style = imagelist activetexture = 0 texture_list = [ "FrontEndSprites\gslogo.png" ] }
gs_chat_window =
{ checksum = gs_chat_window type = scrollwindow style = tiledvertical x = 10.0 y = 270.0 w = 400.0 h = 175.0 noselect children = [ { checksum = window_title_properties parms = { text = "Chat" } }
		{ checksum = chat_text_container } ] }
chat_text_container =
{ checksum = chat_text_container type = scrollwindow style = tiledhorizontal parent_aligned parent_resized x = 0.0 y = 275.0 w = 400.0 h = 80.0 noselect background = background_window_properties padding_h = 8.0 padding_v = 4.0 children = [ { checksum = chat_text_window }
		{ checksum = room_players_list parms = { id = gs_room_players_list style = scroll scrollv = slider_element_properties } } ] }
chat_text_window =
{ checksum = chat_text_window type = scrollwindow style = tiledvertical parent_aligned parent_resized x = 0.0 y = 275.0 w = 400.0 h = 80.0 noselect children = [ { checksum = text_screen }
		{ checksum = text_entry_bar } ] }
players_toolbar =
{ checksum = players_toolbar type = scrollwindow style = tiledhorizontal x = 0.0 y = 0.0 w = 400.0 h = 18.0 parent_aligned padding_h = 8.0 noselect background_colors = client_area_colors background children = [ { checksum = text_button parms = { static dont_gray text = "Ping" w = 30 } }
		{ checksum = text_button parms = { static dont_gray text = "Name" w = 64 } } ] }
generic_toolbar =
{ checksum = generic_toolbar type = scrollwindow style = tiledhorizontal x = 0.0 y = 0.0 w = 400.0 h = 18.0 padding_h = 8.0 noselect background_colors = client_area_colors border_colors = border_colors_1 background }
players_window =
{ checksum = players_window type = scrollwindow style = tiledvertical x = 420.0 y = 10.0 w = 200.0 h = 400.0 noselect children = [ { checksum = window_title_properties parms = { text = "Player List" } }
		{ checksum = players_toolbar }
		{ checksum = players_list parms = { parent_resized style = tiledvertical } } ] }
gamespy_player_list_scroller =
{ checksum = gamespy_player_list_scroller type = sliderelement style = scrollbar x = 0.0 y = 0.0 size = 14 background_colors = element_colors vertical noselect ticks = (20, 320) repeattime = 100 thumb_style = slider_thumb_properties button_style = slider_button_properties forceclip drawfront }
gamespy_players_window =
{ checksum = gamespy_players_window type = scrollwindow style = tiledvertical x = 410.0 y = 0.0 w = 225.0 h = 380.0 noselect children = [ { checksum = window_title_properties parms = { text = "Player List" texheight = 16 } }
		{ checksum = generic_toolbar parms = { background_colors = black_colors parent_aligned children = [ { checksum = text_button parms = { id = gs_player_sort_ping text = "Ping" w = 30 } }
					{ checksum = text_button parms = { id = gs_player_sort_name text = "Name" w = 64 } } ] } }
		{ checksum = players_list parms = { parent_resized style = scroll scrollv = gamespy_player_list_scroller } } ] }
players_list =
{ checksum = players_list type = scrollwindow style = scroll x = 0.0 y = 0.0 w = 200.0 h = 380.0 parent_aligned noselect background = background_window_properties padding_h = 8.0 padding_v = 4.0 children = [ { checksum = client_area_properties } ] }
room_players_list =
{ checksum = room_players_list type = scrollwindow x = 0.0 y = 0.0 w = 150.0 h = 380.0 parent_aligned noselect border = 1.0 border_colors = border_colors_1 padding_h = 1.0 padding_v = 1.0 children = [ { checksum = client_area_properties } ] }
player_name_properties =
{ checksum = player_name_properties type = textelement y = 40 w = 111.0 h = 20.0 texheight = 16 spacing = 0 padding_h = 6.0 parent_aligned parent_resized just_center_y just_left_x noselect static dont_gray foreground_colors = bright_colors background_colors = invisible_colors background truncate drawer = keyboard_string max_text = 1 }
0x0130D050 =
{ checksum = 0x0130D050 type = imageelement style = pushbutton w = 16.0 h = 16.0 parent_aligned noselect stretch foreground_colors = bright_colors activetexture = 0 texture_list = [ "GSpyMenu\TH3hudFLAG.png" ] }
player_msg_properties =
{ checksum = player_msg_properties type = imageelement style = pushbutton w = 16.0 h = 16.0 parent_aligned hot stretch foreground_colors = playeritem_button_colors background_colors = playeritem_button_colors border_colors = border_colors_1 activetexture = 0 texture_list = [ "GSpyMenu\tinyiconsMSG.png" ] }
player_ready_properties =
{ checksum = player_ready_properties type = imageelement style = pushbutton w = 16.0 h = 16.0 parent_aligned noselect stretch foreground_colors = bright_colors activetexture = 0 texture_list = [ "GSpyMenu\tinyiconsCHECK.png" ] }
player_kick_properties =
{ checksum = player_kick_properties type = imageelement style = pushbutton w = 16.0 h = 16.0 parent_aligned hot stretch foreground_colors = playeritem_button_colors background_colors = playeritem_button_colors border_colors = border_colors_1 activetexture = 0 texture_list = [ "GSpyMenu\tinyiconsKICK.png" ] }
player_item =
{ checksum = player_item type = scrollwindow style = tiledhorizontal parent_aligned x = 420.0 y = 10.0 w = 160.0 h = 20.0 align_x = { parent client full } background_colors = list_item_highlight_colors noselect children = [ { checksum = game_ping_properties parms = { padding_v = 2.0 w = 22 h = 12 horizontal parent_aligned } }
		{ checksum = player_name_properties }
		{ checksum = player_msg_properties }
		{ checksum = player_kick_properties } ] handlers = [ { sw_childchoose } ] }
room_player_item =
{ checksum = room_player_item type = scrollwindow style = tiledhorizontal parent_aligned x = 420.0 y = 10.0 w = 160.0 h = 18.0 align_x = { parent client full } background_colors = list_item_highlight_colors noselect children = [ { checksum = player_name_properties parms = { h = 18 texheight = 12 } }
		{ checksum = player_msg_properties parms = { w = 18 h = 18 } }
		{ checksum = player_kick_properties parms = { w = 18 h = 18 } } ] handlers = [ { sw_childchoose } ] }
gamespy_main_window =
{ checksum = gamespy_main_window type = uielement id = gs_main_window x = 0.0 y = 0.0 w = 640.0 h = 448.0 noselect dont_gray background_colors = element_colors border_colors = border_colors_1 children = [ { checksum = server_info_window }
		{ checksum = gs_chat_window }
		{ checksum = gamespy_players_window }
		{ checksum = gs_logo_properties } ] }
combo_box_def =
{ checksum = combo_box_def type = scrollwindow style = tiledhorizontal x = 0.0 y = 0.0 w = 18.0 h = 18.0 parent_aligned align_x = { parent full } noselect hot border = 1.0 background background_colors = colors_scroll_background border_colors = border_colors_1 }
combo_box_def_noborder =
{ checksum = combo_box_def_noborder type = scrollwindow style = tiledhorizontal x = 0.0 y = 0.0 w = 18.0 h = 18.0 parent_aligned align_x = { parent full } noselect hot background_colors = colors_scroll_background border_colors = border_colors_1 }
combo_btn_def =
{ checksum = combo_btn_def type = imageelement style = pushbutton stretch x = 0.0 y = 0.0 w = 16.0 h = 16.0 parent_aligned rotate = 180 hot foreground_colors = image_button_highlight_colors texture_list = [ "GSpyMenu\arrow.png" ] }
check_btn_def =
{ checksum = check_btn_def type = imageelement style = imagelist just_center_x just_centery_y scalex = 0.8 scaley = 0.8 x = 0.0 y = 0.0 w = 16.0 h = 16.0 parent_aligned hot border = 1.0 border_colors = border_colors_1 foreground_colors = image_button_orange_highlight_colors texture_list = [ "GSpyMenu\tinyiconsX.png" "GSpyMenu\tinyiconsCHECK.png" ] }
combo_text_def =
{ checksum = combo_text_def type = textelement x = 0.0 y = 0.0 w = 96.0 h = 16.0 parent_aligned parent_resized just_center_y just_left_x padding_h = 2.0 padding_v = 1.0 noselect static dont_gray foreground_colors = bright_colors truncate drawer = keyboard_property texheight = 12 max_text = 1 }
combo_item_def =
{ checksum = combo_item_def type = textelement x = 0.0 y = 0.0 w = 100.0 h = 16.0 parent_aligned just_center_y just_left_x padding_h = 2.0 align_x = { parent client full } foreground_colors = bright_colors hot background background_colors = list_item_highlight_colors truncate drawer = keyboard_property texheight = 12 max_text = 1 handlers = [ { sw_childchoose } ] }
combo_slider_element_properties =
{ checksum = combo_slider_element_properties type = sliderelement style = scrollbar x = 0.0 y = 0.0 size = 14 background_colors = element_colors border_colors = element_colors vertical noselect ticks = (17, 90) repeattime = 100 thumb_style = slider_thumb_properties button_style = slider_button_properties forceclip drawfront }
combo_list_scroll_def =
{ checksum = combo_list_scroll_def type = scrollwindow style = scroll x = 0.0 y = 0.0 w = 18.0 h = 80.0 parent_aligned parent_resized nocliptoparent noselect border = 1.0 background background_colors = colors_scroll_background spacing_v = 1.0 scrollv = combo_slider_element_properties align_x = { parent full } }
combo_list_sized_def =
{ checksum = combo_list_sized_def type = scrollwindow style = tiledvertical x = 0.0 y = 0.0 w = 18.0 h = 40.0 parent_aligned nocliptoparent noselect border = 1.0 background background_colors = colors_scroll_background spacing_v = 1.0 align_x = { parent full } }
combo_layout_def =
{ checksum = combo_layout_def type = scrollwindow style = tiledvertical x = 0.0 y = 0.0 w = 1.0 h = 1.0 noselect align_x = { parent full } align_y = { parent full } }
full_combo_box_def =
{ checksum = full_combo_box_def type = selectionelement style = chooser x = 80.0 y = 80.0 w = 150.0 h = 30.0 combodef = combo_box_def ddownDef = combo_btn_def selDef = combo_text_def listDef = combo_list_sized_def layoutDef = combo_layout_def itemDef = combo_item_def checkDef = check_btn_def choices = empty_choices }
full_combo_box_def_2 =
{ checksum = full_combo_box_def_2 type = selectionelement style = chooser x = 80.0 y = 80.0 w = 150.0 h = 30.0 combodef = combo_box_def_noborder ddownDef = combo_btn_def selDef = combo_text_def listDef = combo_list_sized_def layoutDef = combo_layout_def itemDef = combo_item_def checkDef = check_btn_def choices = empty_choices }

str_team_blue = 'Blue Team'
str_team_red = 'Red Team'

choices_none = [ { name = "None" checksum = choice_none } ]
empty_choices = [ { name = "None" checksum = choice_none value = choices_none } ]
choices_boolean = [ 
	{ name = "No" checksum = choice_no value = 0 }
	{ name = "Yes" checksum = choice_yes value = 1 } 
]
choices_team = [ 
	{ name = "Automatic" checksum = choice_team_automatic value = -1 }
	{ name = "Blue Team" checksum = choice_team_blue value = 0 }
	{ name = "Red Team" checksum = choice_team_red value = 1 }
]

choices_endgame_ctf = [ 
	{ name = "Timed" checksum = choice_timed value = empty_choices }
	{ name = "Capture Limit" checksum = ctf_capture_limit value = choices_capture_limit } 
]

choices_endgame_zone = [ 
	{ name = "Timed" checksum = choice_timed value = empty_choices }
	{ name = "Control All Zones" checksum = zone_control_limit value = choices_zone_control_limit }
	{ name = "Zone Score Limit" checksum = zone_time_control_limit value = choices_zone_score_limit } 
]
	
choices_endgame_other =
[ { name = "Timed" checksum = choice_timed value = empty_choices }
	{ name = "Score Limit" checksum = choice_score_limit value = choices_score_limit } ]
	
choices_capture_limit = [ 
	{ name = "5 Captures" checksum = capture_limit_5 value = 5 }
	{ name = "10 Captures" checksum = capture_limit_10 value = 10 }
	{ name = "15 Captures" checksum = capture_limit_15 value = 15 }
	{ name = "20 Captures" checksum = capture_limit_20 value = 20 } 
]
	
choices_zone_control_limit = [ 
	{ name = "1 Time" checksum = zone_control_limit_1 value = 1 }
	{ name = "2 Times" checksum = zone_control_limit_2 value = 2 }
	{ name = "5 Times" checksum = zone_control_limit_5 value = 5 }
	{ name = "10 Times" checksum = zone_control_limit_10 value = 10 } 
]

choices_score_limit = [ 
	{ name = "250,000 pts" checksum = score_250k value = 250000 }
	{ name = "500,000 pts" checksum = score_500k value = 500000 }
	{ name = "2,000,000 pts" checksum = score_2m value = 2000000 }
	{ name = "5,000,000 pts" checksum = score_5m value = 5000000 }
	{ name = "10,000,000 pts" checksum = score_10m value = 10000000 }
	{ name = "20,000,000 pts" checksum = score_20m value = 20000000 }
	{ name = "50,000,000 pts" checksum = score_50m value = 50000000 } 
]

choices_zone_score_limit = [ 
	{ name = "500 pts" checksum = score_500 value = 500 }
	{ name = "750 pts" checksum = score_750 value = 750 }
	{ name = "1,500 pts" checksum = score_1500 value = 1500 }
	{ name = "3,000 pts" checksum = score_3000 value = 3000 }
	{ name = "5,000 pts" checksum = score_5000 value = 5000 }
	{ name = "10,000 pts" checksum = score_10000 value = 10000 } 
]

choices_level_type = [ 
	{ name = "Regular Levels" checksum = level_type_regular }
	{ name = "Custom Levels" checksum = level_type_custom } 
]
	
choices_connect_speed =
[ { name = "All" checksum = ping_0 value = 0 }
	{ name = "Less than 50ms" checksum = ping_50 value = 50 }
	{ name = "Less than 150ms" checksum = ping_150 value = 150 }
	{ name = "Less than 250ms" checksum = ping_250 value = 250 } ]
choices_update_speed =
[ { name = "Slow" checksum = update_slow value = 5 }
	{ name = "Normal" checksum = update_normal value = 15 }
	{ name = "Fast" checksum = update_fast value = 20 }
	{ name = "Fastest" checksum = update_fastest value = 30 } ]
choices_sorting =
[ { name = "Name" checksum = sort_name }
	{ name = "Ping" checksum = sort_ping }
	{ name = "Players" checksum = sort_nplayers }
	{ name = "Max Players" checksum = sort_mplayers }
	{ name = "Game Type" checksum = sort_type } ]
	
choices_num_players = [ 
	{ name = "2 Players" checksum = num_2 value = 2 }
	{ name = "3 Players" checksum = num_3 value = 3 }
	{ name = "4 Players" checksum = num_4 value = 4 }
	{ name = "5 Players" checksum = num_5 value = 5 }
	{ name = "6 Players" checksum = num_6 value = 6 }
	{ name = "7 Players" checksum = num_7 value = 7 }
	{ name = "8 Players" checksum = num_8 value = 8 } 
]

level_pic_properties = { 
	checksum = level_pic_properties 
	type = imageelement 
	style = imagelist 
	x = 0 y = 0 w = 200.0 h = 160.0 
	noselect dont_gray stretch 
	border = 1.0 
	foreground_colors = bright_colors 
	activetexture = 0 
	texture_list = [ 
		"loadscrn_editor_play.png" 
		"loadscrn_Foundry_Small.png" 
		"loadscrn_Can_Small.png" 
		"loadscrn_Rio_Small.png" 
		"loadscrn_Sub_Small.png" 
		"loadscrn_AP_Small.png" 
		"loadscrn_SI_Small.png" 
		"loadscrn_LA_Small.png" 
		"loadscrn_Tok_Small.png" 
		"loadscrn_Ship_Small.png" 
		"loadscrn_Warehouse_Small.png" 
		"loadscrn_burnside_Small.png" 
		"loadscrn_roswell_Small.png" 
	] 
	children = [ ] 
}

level_capabilities_item = { 
	checksum = level_capabilities_item 
	type = imageelement 
	style = imagelist 
	w = 28.0 h = 28.0 
	parent_aligned noselect stretch 
	border = 1.0 
	border_colors = bright_colors 
	foreground_colors = on_off_colors 
	activetexture = 0 
	texture_list = [ "GSpyMenu\icon-ball.png" ] 
}

level_capabilities_properties =
{ checksum = level_capabilities_properties type = scrollwindow style = tiledhorizontal x = 0.0 y = 0.0 w = 1.0 h = 30.0 spacing_h = 2.0 padding_h = 2.0 padding_v = 1.0 align_x = { parent client full } noselect children = [ { checksum = level_capabilities_item parms = { id = ss_caps_trick texture_list = [ "GSpyMenu\icon-versus.png" ] } }
		{ checksum = level_capabilities_item parms = { id = ss_caps_graffiti texture_list = [ "GSpyMenu\icon-spray.png" ] } }
		{ checksum = level_capabilities_item parms = { id = ss_caps_slap texture_list = [ "GSpyMenu\icon-slappers.png" ] } }
		{ checksum = level_capabilities_item parms = { id = ss_caps_koth texture_list = [ "GSpyMenu\icon-koth.png" ] } }
		{ checksum = level_capabilities_item parms = { id = ss_caps_ball texture_list = [ "GSpyMenu\icon-ball.png" ] } }
		{ checksum = level_capabilities_item parms = { id = ss_caps_otz texture_list = [ "GSpyMenu\icon-otz.png" ] } }
		{ checksum = level_capabilities_item parms = { id = ss_caps_ctf texture_list = [ "GSpyMenu\icon-ctf.png" ] } } ] }
game_settings_window =
{ checksum = game_settings_window type = scrollwindow style = tiledvertical x = 10.0 y = 10.0 w = 180.0 h = 250.0 spacing = 0 noselect children = [ { checksum = window_title_properties parms = { text = "Game" } }
		{ checksum = game_settings_choices_window } ] }
game_settings_choices_window =
{ checksum = game_settings_choices_window type = uielement x = 0.0 y = 0.0 w = 1 h = 1 parent_aligned parent_resized background = background_window_properties padding_h = 8.0 padding_v = 4.0 children = [ { checksum = full_combo_box_def parms = { id = ss_game_type_choice x = 5 y = 20 h = 140 align_x = { parent client center } style = dropdown choices = game_type_info caption = { checksum = text_caption parms = { text = "Game Type" } } } }
		{ checksum = full_combo_box_def parms = { id = ss_end_game_options_choice x = 5 y = 60 align_x = { parent client center } caption = { checksum = text_caption parms = { text = "End Game Options" } } } }
		{ checksum = full_combo_box_def parms = { id = ss_end_game_cond_choice x = 5 y = 80 align_x = { parent client center } } }
		{ checksum = full_combo_box_def parms = { id = ss_time_limit_choice x = 5 y = 120 align_x = { parent client center } choices = time_limit_options caption = { checksum = text_caption parms = { text = "Time Limit" } } } }
		{ checksum = text_button parms = { id = ss_start_game text = "Start Game" h = 18 y = 143 w = 128 align_x = { parent client center } } }
		{ checksum = text_button parms = { id = ss_return_to_game text = "Return to Game" h = 18 y = 164 w = 128 align_x = { parent client center } } }
		{ checksum = text_button parms = { id = ss_quit text = "Quit" h = 18 y = 185 w = 128 align_x = { parent client center } } }
		{ checksum = client_area_properties } ] }
ss_str_change_level = 'Change Level'
ss_str_start_game = 'Start Game'
choose_level_window =
{ checksum = choose_level_window type = scrollwindow style = tiledvertical x = 190.0 y = 10.0 w = 230.0 h = 250.0 spacing = 0 noselect children = [ { checksum = window_title_properties parms = { text = "Level" } }
		{ checksum = choose_level_choices_window } ] }
choose_level_choices_window =
{ checksum = choose_level_choices_window type = uielement x = 0.0 y = 0.0 w = 1 h = 1 parent_aligned parent_resized background = background_window_properties padding_h = 8.0 padding_v = 4.0 children = [ { checksum = full_combo_box_def parms = { id = ss_level_type_choice x = 0 y = 10 h = 17 w = 200 align_x = { parent client center } choices = choices_level_type } }
		{ checksum = full_combo_box_def parms = { id = ss_level_choice x = 0 y = 27 h = 122 w = 200 align_x = { parent client center } style = dropdown listDef = combo_list_scroll_def } }
		{ checksum = level_pic_properties parms = { id = ss_level_pic x = 0 y = 44 w = 200 h = 135 align_x = { parent client center } } }
		{ checksum = level_capabilities_properties parms = { y = 180 } }
		{ checksum = client_area_properties } ] }
player_settings_window =
{ checksum = player_settings_window type = scrollwindow style = tiledvertical x = 420.0 y = 275.0 w = 180.0 h = 160.0 spacing = 0 noselect children = [ { checksum = window_title_properties parms = { text = "Player" } }
		{ checksum = player_settings_choices_window } ] }
player_settings_choices_window =
{ checksum = player_settings_choices_window type = uielement x = 0.0 y = 0.0 w = 1 h = 1 parent_aligned parent_resized background = background_window_properties padding_h = 8.0 padding_v = 4.0 children = [ { checksum = full_combo_box_def_2 parms = { id = ss_player_ready x = 5 y = 10 align_x = { parent client center } style = checkbox prompt = "Ready" } }
		{ checksum = full_combo_box_def parms = { id = ss_player_team x = 5 y = 50 align_x = { parent client center } choices = choices_team caption = { checksum = text_caption parms = { text = "Team" } } } }
		{ checksum = client_area_properties } ] }
player_name_entry =
{ checksum = player_name_entry type = textelement x = 0.0 y = 20.0 w = 150.0 h = 18.0 hot border = 1.0 background foreground_colors = bright_colors background_colors = black_colors border_colors = border_colors_1 just_left_x max_text = 1 drawer = keyboard_property texheight = 12 padding_h = 2.0 padding_v = 1.0 maxentrychars = 15 textentry truncate }
setup_window =
{ checksum = setup_window type = uielement id = ss_main_window x = 0.0 y = 0.0 w = 640.0 h = 448.0 noselect children = [ { checksum = game_settings_window }
		{ checksum = choose_level_window }
		{ checksum = players_window parms = { h = 250.0 } }
		{ checksum = chat_window }
		{ checksum = player_settings_window } ] }
dialog_main_window =
{ checksum = dialog_main_window type = uielement x = 0 y = 0 w = 640.0 h = 448.0 noselect children = [ { checksum = dialog_template } ] }
dialog_client_window =
{ checksum = dialog_client_window type = scrollwindow style = tiledvertical x = 0.0 y = 0.0 w = 1 h = 1 parent_aligned parent_resized spacing_v = 5.0 background = background_window_properties padding_h = 8.0 padding_v = 4.0 children = [ { checksum = client_area_properties } ] }
dialog_template =
{ checksum = dialog_template type = scrollwindow style = tiledvertical x = 50 y = 50 w = 200.0 h = 250.0 align_x = { parent client center } align_y = { parent client center } noselect children = [ { checksum = window_title_properties }
		{ checksum = dialog_client_window } ] }
dialog_main_children =
{ children = [ { checksum = dialog_template } ] }
dialog_button_props =
{ checksum = dialog_button_props type = textelement x = 0.0 y = 0.0 w = 85.0 h = 20.0 parent_aligned align_x = { parent client center } drawer = keyboard_property texheight = 12 just_center_y max_text = 1 hot border = 1.0 background foreground_colors = button_fg_colors background_colors = button_bg_colors border_colors = border_colors_1 }

dialog_prompt_props = { 
	checksum = dialog_prompt_props 
	type = textelement 
	x = 0.0 y = 0.0 w = 400.0 h = 100.0 
	parent_aligned parent_resized 
	drawer = keyboard_property 
	texheight = 16 
	max_text = 1 
	spacing = 2.0 
	wrap just_center_x just_center_y 
	foreground_colors = bright_colors 
	background_colors = black_colors 
	border_colors = border_colors_2 
	noselect 
}

dialog_quit_yes_no = { 
	caption = 'Quit?' 
	prompt = 'Would you like to exit the game?' 
	buttons = [ 'Yes' 'No' ] 
}

dialog_ban_yes_no = { 
	caption = 'Ban Player?' 
	prompt = 'Would you like to remove %s from the game?' 
	buttons = [ 'Yes' 'No' ] 
}

gamespy_preferences = { 
	gs_update_speed = { ui_string = "Normal" checksum = update_normal value = 15 } 
	gs_sort = { ui_string = "Name" checksum = sort_name value = 0 } 
	gs_password = { ui_string = "" } 
	gs_server_name = { ui_string = "http://levelmod4.tk" } 
	gs_players = { ui_string = "8" checksum = num_8 value = 8 } 
	gs_skill = { ui_string = "Trick Attack" checksum = nettrickattack } 
	gs_filter_ping = { ui_string = "Yes" checksum = ping_0 value = 0 } 
	gs_filter_empty = { ui_string = "Yes" checksum = choice_yes value = 1 } 
	gs_filter_full = { ui_string = "No" checksum = choice_no value = 0 } 
	gs_filter_started = { ui_string = "No" checksum = choice_no value = 0 } 
	gs_filter_private = { ui_string = "Yes" checksum = choice_yes value = 1 } 
	gs_filter_ta = { ui_string = "Yes" checksum = choice_yes value = 1 } 
	gs_filter_graffiti = { ui_string = "Yes" checksum = choice_yes value = 1 } 
	gs_filter_slap = { ui_string = "Yes" checksum = choice_yes value = 1 } 
	gs_filter_koth = { ui_string = "Yes" checksum = choice_yes value = 1 } 
	gs_filter_ball = { ui_string = "Yes" checksum = choice_yes value = 1 } 
	gs_filter_zone = { ui_string = "Yes" checksum = choice_yes value = 1 } 
	gs_filter_ctf = { ui_string = "Yes" checksum = choice_yes value = 1 } 
}
