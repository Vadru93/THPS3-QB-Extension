# THPS3-QB-Extension


###BIG CHANGES###

LevelMod.flt is now deprecated. Now you only need d3d8.dll and you should remove all instances of levelmod.flt including copies. If game finds flt file in same folder as skate3.exe or data/MILES, it will try to load it and since it's already loaded inside d3d8.dll it will crash.

Also Skate3.exe is no longer needed, because d3d8.dll is loaded earlier so I change everything in memory instead.

1. Include Extension.h and add the d3d8.lib to your project.

2. If you use VS you need to set USE MULTI BYTE CHARSET.

3. Set Export target to .flt instead of .dll and add your flt file to data/MILES folder.

4. Remember to add the scripts and the latest built d3d8.dll to not make your game crash.




# List of All Functions that you can call from Script
* **SomeNotes** When you call a function that will add a value to your params that you can access with GLOBAL
* You need to have the value there in the params else it will not work
* **ChangeString** Example: `ChangeString string = StringToChange param = TherNewString` always returns true
*	**strstr** Example: `strstr s1 = "Here is a string" s2 = "string"` returns true if s1 contains s2
*	**GetZAngle** angle is stored in `GLOBAL.angle` and `GLOBAL.angle` need to exist before calling **GetZAngle** always returns true
*	**GetSpeed** same as above but stored in `GLOBAL.speed` always returns true
* **GetSkaterPos** same as above but stored in `GLOBAL.pos` always returns true
* **GetSkaterLook** same as above but stored in `GLOBAL.pos` always returns true
* **CreatePair** Example: `CreatePair Name = "NameToStore" x = 1.0 y = 2.0` acces it with `GLOBAL.NameToStore`
* **TestForAcid** depricated
*	**Wallplant** This is an unfinished wallplant script always returns true
* **PreWallplant** Related to unfinished wallplant always returns true
* **EnterObserveMode2** Enter custom observe mode always returns true
* **LeaveObserveMode2** Leave custom observe mode always returns true
* **ObserveNext** Observe next player always returns true
* **StoreSkaterPos** Stores skaters position always returns true
* **GotoStoredPos** Goto the stored position always returns true
* **Not** Example: `IF Not IsOptionOn` will call CFunction IsOptionOn and return !
* **SubToGlobal** used in thps4+ levels always returns true
* **AddToGlobal** used in thps4+ levels always returns true
* **FreezeCamera** Freezes the camera always returns true
* **UnfreezeCamera** Unfreeze the camera always returns true
* **GrafStarted** Called from scripts when Graffiti is started 
* so client can check if server has enabled unlimited tags
* **ChangeValues** Example: `ChangeValues Container = AStruct values = BStruct` 
* Sets values from BStruct to AStruct returns true if find params, else false
* **GetSliderValue** Example: `GetSliderValue id = element_id ||name = NameToStore||` 
* acces it with `GLOBAL.NameToStore` if no name is passed acces with `GLOBAL.value` returns true on sucess
* **InitLevelMod** Inits levelmod stuff
* **MoveObject** Move an EnvironmentObject
* **KillMovingObject** Stop Moving an EnvironmentObject
* **ChangeParamToUnnamed**
* **IsOptionOn**
* **AddOption**
* **ToggleOption**
* **LM_GotParam**
* **GetParam**
