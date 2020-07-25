# THPS3-QB-Extension


###BIG CHANGES###

LevelMod.flt is now deprecated. Now you only need d3d8.dll and you should remove all instances of levelmod.flt including copies. If game finds flt file in same folder as skate3.exe or data/MILES, it will try to load it and since it's already loaded inside d3d8.dll it will crash.
Also Skate3.exe is no longer needed, because d3d8.dll is loaded earlier so I change everything in memory instead.

1. Include Extension.h and add the Levelmod.lib to your project.

2. If you use VS you need to set USE MULTI BYTE CHARSET.

3. Set Export target to .flt instead of .dll and add your flt file to data/MILES folder.

4. Remember to add the scripts and the latest built d3d8.dll to not make your game crash.
