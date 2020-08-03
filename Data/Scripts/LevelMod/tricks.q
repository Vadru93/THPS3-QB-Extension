
script SkaterInit
  NotInBail
  NotifyBailDone
  SetSkaterCamLerpReductionTimer time = 0
  ClearLipCombos
  AllowRailTricks
  ResetLandedFromVert
  BlendperiodOut 0
  Obj_Attachframe bone = "head"
  SetException Ex = RunHasEnded Scr = EndOfRun
  StopBalanceTrick
  SwitchOffAtomic special_item
  if InNetGame
  else
    ClearSkaterCamOverride
  endif
  ClearSkaterFlags
  printf "Clearing skaters flags =============="
  SetExceptionThatPreservesBalanceTrick MadeOtherSkaterBail
  if ProfileEquals is_named = demoness
    Obj_Attachframe bone = "left_wrist"
    Obj_Attachframe bone = "right_wrist"
    BloodParticlesOn name = "breath.png" start_col = 1426128895 end_col = 153 blend_mode = 72 num = 40 emit_w = 1.0 emit_h = 1.0 angle = 10 size = 20.0 bone = "left_wrist" growth = 0 time = 10000 speed = 50 grav = 0 life = 0.5
    BloodParticlesOn name = "breath.png" start_col = 1426128895 end_col = 153 blend_mode = 72 num = 40 emit_w = 1.0 emit_h = 1.0 angle = 10 size = 20.0 bone = "right_wrist" growth = 0 time = 10000 speed = 50 grav = 0 life = 0.5
  endif
  PlayAnim Anim = Idle
  SetBalanceTrickParams ManualParams
  goto OnGroundAI
endscript
script ClientSkaterInit
  SwitchOffAtomic special_item
endscript
script SkaterRetryScript
  SkaterInit
endscript
script ClearSkaterFlags
  Obj_ClearFlag FLAG_SKATER_KILLING
  Obj_ClearFlag FLAG_SKATER_REVERTFS
  Obj_ClearFlag FLAG_SKATER_REVERTBS
  Obj_ClearFlag FLAG_SKATER_INLOOP
  Obj_ClearFlag FLAG_SKATER_LIPCOMBO
  Obj_ClearFlag FLAG_SKATER_MANUALCHEESE
endscript
script Demoness_FlameOn
  BloodParticlesOn name = "breath.png" start_col = 1426128895 end_col = 153 blend_mode = 72 num = 40 emit_w = 1.0 emit_h = 1.0 angle = 10 size = 20.0 bone = "left_wrist" growth = 0 time = 10000 speed = 50 grav = 0 life = 0.5
  BloodParticlesOn name = "breath.png" start_col = 1426128895 end_col = 153 blend_mode = 72 num = 40 emit_w = 1.0 emit_h = 1.0 angle = 10 size = 20.0 bone = "right_wrist" growth = 0 time = 10000 speed = 50 grav = 0 life = 0.5
endscript
Exceptions =
[ RunHasEnded SkaterCollideBail MadeOtherSkaterBail Ollied GroundGone Landed OntoRail OffRail FlailHitWall FlailLeft FlailRight OffMeterTop OffMeterBottom WallRideLeft WallRideRight WallRideBail CarBail CarPlant WonGame LostGame AcidDrop SpineTransfer BankDrop ]
script OnGroundExceptions
  if GotParam NoClear
  else
    ClearExceptions
  endif
  if GotParam NoEndRun
  else
    SetException Ex = RunHasEnded Scr = EndOfRun
  endif
  SetException Ex = GroundGone Scr = GroundGone
  SetException Ex = Ollied Scr = Ollie
  SetException Ex = FlailHitWall Scr = FlailHitWall
  SetException Ex = FlailLeft Scr = FlailLeft
  SetException Ex = FlailRight Scr = FlailRight
  SetException Ex = CarPlant Scr = CarPlant
  SetException Ex = CarBail Scr = CarBail
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  SetException Ex = MadeOtherSkaterBail Scr = MadeOtherSkaterBail
  SetQueueTricks Jumptricks GroundTricks
  SetExtraGrindTricks special = SpecialGrindTricks GrindTricks
  SetManualTricks special = SpecialManualTricks GroundManualTricks
  VibrateOff
  SwitchOnBoard
  EnablePlayerInput
  BailOff
  BashOff
  SetRollingFriction #"default"
  CanSpin
  AllowRailTricks
  SetSkaterCamLerpReductionTimer time = 0
  Obj_ClearFlag FLAG_SKATER_MANUALCHEESE
  if GotParam NoEndRun
  else
    ResetLandedFromVert
  endif
endscript
script OnGroundExceptions_NoClear
  OnGroundExceptions NoClear
endscript
script OnGroundExceptions_NoEndRun
  OnGroundExceptions NoEndRun
endscript
script InAirExceptions
  ClearExceptions
  SetException Ex = Landed Scr = Land
  SetException Ex = WallRideLeft Scr = WallRide Params = { Left }
  SetException Ex = WallRideRight Scr = WallRide Params = { Right }
  SetException Ex = AcidDrop Scr = AcidDrop
  SetException Ex = SpineTransfer Scr = SpineTransfer
  SetException Ex = BankDrop Scr = BankDrop
  SetException Ex = WallRideBail Scr = WallRideBail
  SetException Ex = CarPlant Scr = CarPlant CallInsteadofGoto
  SetException Ex = CarBail Scr = CarBail
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  SetException Ex = MadeOtherSkaterBail Scr = MadeOtherSkaterBailAir CallInsteadofGoto
  SetQueueTricks special = SpecialTricks AirTricks
  SetExtraGrindTricks special = SpecialGrindTricks GrindTricks
  SetManualTricks special = SpecialManualTricks ManualTricks
  VibrateOff
  EnablePlayerInput
  BailOff
  BashOff
  SetRollingFriction #"default"
  SetState Air
  CanSpin
endscript
script ClearTrickQueues
  ClearTrickQueue
  ClearManualTrick
  ClearExtraGrindTrick
endscript
script AcidDrop
  SetTrickName 'Acid Drop'
  SetTrickScore 250
  Display NoDegrade
  if #"Not" DoingTrick
    SetState Air
    InAirExceptions
  endif
  
  begin
		if Released Circle
			break
		endif
		WaitOneGameFrame
	repeat

    Bailoff
	DoNextTrick
	Wait 2 frames
	KillExtraTricks
	WaitAnimWhilstChecking
	goto Airborne
endscript
script SpineTransfer
  SetTrickName 'Spine Transfer'
  SetTrickScore 250
  Display NoDegrade
  if #"Not" DoingTrick
    SetState Air
    InAirExceptions
  endif
  
  begin
		if Released Circle
			break
		endif
		WaitOneGameFrame
	repeat

    Bailoff
	DoNextTrick
	Wait 2 frames
	KillExtraTricks
	WaitAnimWhilstChecking
	goto Airborne
endscript
script ExitQP
TurnToFaceVelocity
goto Airborne
endscript
script BankDrop
  SetTrickName 'Bank Drop'
  SetTrickScore 250
  Display NoDegrade
  if #"Not" DoingTrick
    SetState Air
    InAirExceptions
  endif
  wait 2 frames
  KillExtraTricks
  WaitAnimWhilstChecking
  goto Airborne
endscript
script WallRide
  ClearExceptions
  SetException Ex = Landed Scr = Land
  SetException Ex = Ollied Scr = Wallie
  SetException Ex = GroundGone Scr = WallrideEnd
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  Vibrate Actuator = 1 Percent = 40
  if GotParam Left
    move x = 36
  else
    move x = -36
  endif
  SetQueueTricks WallrideTricks
  NollieOff
  SetTrickScore 200
  PlayCessSound
  if GotParam Left
    if Flipped
      SetTrickName 'BS Wallride'
      PlayAnim Anim = WallRideBackTrans BlendPeriod = 0.0
      WaitAnimFinished
      PlayAnim Anim = WallRideBackLoop BlendPeriod = 0.1 Cycle
    else
      SetTrickName 'FS Wallride'
      PlayAnim Anim = WallRideFrontTrans BlendPeriod = 0.0
      WaitAnimFinished
      PlayAnim Anim = WallRideFrontLoop BlendPeriod = 0.1 Cycle
    endif
  else
    if Flipped
      SetTrickName 'FS Wallride'
      PlayAnim Anim = WallRideFrontTrans BlendPeriod = 0.0
      WaitAnimFinished
      PlayAnim Anim = WallRideFrontLoop BlendPeriod = 0.1 Cycle
    else
      SetTrickName 'BS Wallride'
      PlayAnim Anim = WallRideBackTrans BlendPeriod = 0.0
      WaitAnimFinished
      PlayAnim Anim = WallRideBackLoop BlendPeriod = 0.1 Cycle
    endif
  endif
  Display
endscript

WallRideTricks =
[ { Trigger = { TapTwiceRelease , Up , x , 500 } Scr = Trick_WallPlant } ]
script Trick_WallPlant
  InAirExceptions
  Vibrate Actuator = 1 Percent = 50 Duration = 0.1
  PlayAnim Anim = Boneless BlendPeriod = 0.0
  SetTrickName "Wallplant"
  SetTrickScore 500
  Display
  Jump BonelessHeight
  WaitAnimWhilstChecking
  Goto Airborne StretchTime = 10 BlendPeriod = 0
endscript

script WallrideEnd
  BlendperiodOut 0
  ClearExceptions
  InAirExceptions
  SetState Air
  goto Airborne stretchtime = 10 BlendPeriod = 0
endscript
script Wallie
  InAirExceptions
  Vibrate Actuator = 1 Percent = 50 Duration = 0.1
  PlayAnim Anim = Ollie BlendPeriod = 0.0
  SetTrickName "Wallie"
  SetTrickScore 250
  Display
  Jump
  WaitAnimWhilstChecking
  goto Airborne stretchtime = 10 BlendPeriod = 0
endscript
script WallRideBail
endscript
script OnGroundAI
  SetState Ground
  OnGroundExceptions
  SetRollingFriction #"default"
  EnablePlayerInput
  NollieOff
  begin
    if LeftPressed
      if Crouched
        if Flipped
          if AnimEquals CrouchTurnRightIdle
          else
            PlayAnim Anim = CrouchTurnRight NoRestart
            if AnimFinished
              PlayAnim Anim = CrouchTurnRightIdle Cycle NoRestart
            endif
          endif
        else
          if AnimEquals CrouchTurnLeftIdle
          else
            PlayAnim Anim = CrouchTurnLeft NoRestart
            if AnimFinished
              PlayAnim Anim = CrouchTurnLeftIdle Cycle NoRestart
            endif
          endif
        endif
      else
        if Flipped
          if AnimEquals TurnRightIdle
          else
            PlayAnim Anim = TurnRight NoRestart
            if AnimFinished
              PlayAnim Anim = TurnRightIdle Cycle NoRestart
            endif
          endif
        else
          if AnimEquals TurnLeftIdle
          else
            PlayAnim Anim = TurnLeft NoRestart
            if AnimFinished
              PlayAnim Anim = TurnLeftIdle Cycle NoRestart
            endif
          endif
        endif
      endif
    else
      if RightPressed
        if Crouched
          if Flipped
            if AnimEquals CrouchTurnLeftIdle
            else
              PlayAnim Anim = CrouchTurnLeft NoRestart
              if AnimFinished
                PlayAnim Anim = CrouchTurnLeftIdle Cycle NoRestart
              endif
            endif
          else
            if AnimEquals CrouchTurnRightIdle
            else
              PlayAnim Anim = CrouchTurnRight NoRestart
              if AnimFinished
                PlayAnim Anim = CrouchTurnRightIdle Cycle NoRestart
              endif
            endif
          endif
        else
          if Flipped
            if AnimEquals TurnLeftIdle
            else
              PlayAnim Anim = TurnLeft NoRestart
              if AnimFinished
                PlayAnim Anim = TurnLeftIdle Cycle NoRestart
              endif
            endif
          else
            if AnimEquals TurnRightIdle
            else
              PlayAnim Anim = TurnRight NoRestart
              if AnimFinished
                PlayAnim Anim = TurnRightIdle Cycle NoRestart
              endif
            endif
          endif
        endif
      else
        if Crouched
          PlayAnim Anim = CrouchIdle Cycle NoRestart
        else
          if Braking
            if SpeedLessThan 1
              PlayAnim Anim = Idle NoRestart
              if HeldLongerThan Button = down 2 second
                goto HandBrake
              endif
            else
              if HeldLongerThan Button = down 0.2 second
                CessBrake
              else
                PlayAnim Anim = Idle NoRestart
              endif
            endif
          else
            if CanKick
              if AutoKickIsOff
                if ShouldMongo
                  PlayAnim Anim = MongoPush NoRestart
                  WaitAnimFinished
                  PlayAnim Anim = MongoPushCycle NoRestart
                  WaitAnimFinished
                  PlayAnim Anim = Idle Cycle NoRestart
                else
                  PlayAnim Anim = PushCycle NoRestart
                  WaitAnimFinished
                  PlayAnim Anim = Idle Cycle NoRestart
                endif
              else
                if ShouldMongo
                  if AnimEquals MongoPushCycle
                    PlayAnim Anim = MongoPushCycle Cycle NoRestart
                  else
                    PlayAnim Anim = MongoPush NoRestart
                    WaitAnimFinished
                    PlayAnim Anim = MongoPushCycle Cycle NoRestart
                  endif
                else
                  PlayAnim Anim = PushCycle Cycle NoRestart
                endif
              endif
            else
              if AutoKickIsOff
                if held square
                  if ShouldMongo
                    PlayAnim Anim = MongoPushCycle Cycle NoRestart
                  else
                    PlayAnim Anim = PushCycle Cycle NoRestart
                  endif
                else
                  if AnimEquals [ PushCycle MongoPushCycle MongoPush ]
                  else
                    PlayAnim Anim = Idle Cycle NoRestart
                  endif
                endif
              else
                if AnimFinished
                  PlayAnim Anim = Idle Cycle NoRestart
                endif
              endif
            endif
          endif
        endif
      endif
    endif
    DoNextTrick
    DoNextManualTrick
    WaitOneGameFrame
    if InNetGame
      CheckforNetBrake
    endif
  repeat
endscript
script CheckforNetBrake
  if InNetGame
    if MenuOnScreen
      goto NetBrake
    endif
  endif
endscript
script CessBrake
  ClearEventBuffer
  if SpeedGreaterThan 300
    PlayAnim Anim = CessSlide180_FS From = Start To = 28 speed = 1.2
    WaitAnim Frame 5
    PlayCessSound
    WaitAnim Frame 25
    PlayAnim Anim = CessSlide180_FS From = 27 To = Start speed = 1.2
    WaitAnimFinished
  endif
  PlayAnim Anim = Idle Cycle NoRestart
endscript
script NetBrake
  SetRollingFriction 20
  OnExceptionRun NetBrake_out
  CessBrake
  begin
    if SpeedLessThan 5
      if ShouldMongo
        PlayAnim Anim = MongoBrakeIdle Cycle NoRestart
      else
        PlayAnim Anim = BrakeIdle Cycle NoRestart
      endif
    endif
    if MenuOnScreen
      WaitOneGameFrame
    else
      break
    endif
  repeat
  SetRollingFriction #"default"
  if InNollie
    goto OnGroundNollieAI
  else
    goto OnGroundAI
  endif
endscript
script NetBrake_out
  SetRollingFriction #"default"
endscript
script HandBrake
  ClearEventBuffer
  OnExceptionRun BrakeDone
  SetRollingFriction 100
  begin
    if AutoKickIsOff
      if held square
        WaitOneGameFrame
        break
      endif
    else
      if held up
        break
      endif
      if Crouched
        break
      endif
    endif
    if RightPressed
      if Flipped
        PlayAnim Anim = TurnLeft NoRestart
        if AnimFinished
          PlayAnim Anim = TurnLeftIdle Cycle
        endif
      else
        PlayAnim Anim = TurnRight NoRestart
        if AnimFinished
          PlayAnim Anim = TurnRightIdle Cycle
        endif
      endif
    else
      if LeftPressed
        if Flipped
          PlayAnim Anim = TurnRight NoRestart
          if AnimFinished
            PlayAnim Anim = TurnRightIdle Cycle
          endif
        else
          PlayAnim Anim = TurnLeft NoRestart
          if AnimFinished
            PlayAnim Anim = TurnLeftIdle Cycle
          endif
        endif
      else
        if ShouldMongo
          PlayAnim Anim = MongoBrakeIdle Cycle NoRestart
        else
          PlayAnim Anim = BrakeIdle Cycle NoRestart
        endif
      endif
    endif
    WaitOneGameFrame
    DoNextTrick
    DoNextManualTrick
  repeat
  SetRollingFriction #"default"
  if InNollie
    goto OnGroundNollieAI
  else
    goto OnGroundAI
  endif
endscript
script BrakeDone
  SetRollingFriction #"default"
endscript
script OnGroundNollieAI
  SetState Ground
  EnablePlayerInput
  ClearExceptions
  OnGroundExceptions
  SetException Ex = Ollied Scr = Nollie
  SetQueueTricks Jumptricks GroundTricks
  SetManualTricks special = SpecialManualTricks GroundManualTricks
  NollieOn
  begin
    if LeftPressed
      if Crouched
        if Flipped
          PlayAnim Anim = NollieCrouchTurnRight NoRestart
        else
          PlayAnim Anim = NollieCrouchTurnLeft NoRestart
        endif
      else
        if Flipped
          PlayAnim Anim = NollieSkatingTurnRight NoRestart
        else
          PlayAnim Anim = NollieSkatingTurnLeft NoRestart
        endif
      endif
    else
      if RightPressed
        if Crouched
          if Flipped
            PlayAnim Anim = NollieCrouchTurnLeft NoRestart
          else
            PlayAnim Anim = NollieCrouchTurnRight NoRestart
          endif
        else
          if Flipped
            PlayAnim Anim = NollieSkatingTurnLeft NoRestart
          else
            PlayAnim Anim = NollieSkatingTurnRight NoRestart
          endif
        endif
      else
        if Crouched
          PlayAnim Anim = NollieCrouchIdle Cycle NoRestart
        else
          if Braking
            if SpeedLessThan 1
              PlayAnim Anim = NollieSkatingIdle NoRestart
              if HeldLongerThan Button = down 2 second
                goto HandBrake
              endif
            else
              if HeldLongerThan Button = down 0.2 second
                CessBrake
              else
                PlayAnim Anim = Idle NoRestart
              endif
            endif
          else
            if CanKick
              if AutoKickIsOff
                if ShouldMongo
                  if AnimEquals MongoPushCycle
                    PlayAnim Anim = MongoPushCycle NoRestart
                  else
                    PlayAnim Anim = MongoPush NoRestart
                    WaitAnimFinished
                    PlayAnim Anim = MongoPushCycle NoRestart
                  endif
                else
                  PlayAnim Anim = PushCycle NoRestart
                endif
              else
                if ShouldMongo
                  if AnimEquals MongoPushCycle
                    PlayAnim Anim = MongoPushCycle Cycle NoRestart
                  else
                    PlayAnim Anim = MongoPush NoRestart
                    WaitAnimFinished
                    PlayAnim Anim = MongoPushCycle Cycle NoRestart
                  endif
                else
                  PlayAnim Anim = PushCycle Cycle NoRestart
                endif
              endif
            else
              if AnimFinished
                PlayAnim Anim = NollieSkatingIdle Cycle NoRestart
              endif
            endif
          endif
        endif
      endif
    endif
    DoNextTrick
    DoNextManualTrick
    CheckforNetBrake
    WaitOneGameFrame
  repeat
endscript
script FlailExceptions
  NollieOff
  SetException Ex = CarBail Scr = CarBail
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  SetException Ex = RunHasEnded Scr = EndOfRun
  ClearTrickQueue
  ClearManualTrick
  ClearEventBuffer
  CheckGapTricks
  ClearGapTricks
  ClearPanel_Landed
endscript
script FlailHitWall
  FlailExceptions
  FlailVibrate
  PlayAnim Anim = FlailLeft
  WaitAnimFinished
  goto OnGroundAI
endscript
script FlailLeft
  FlailExceptions
  FlailVibrate
  PlayAnim Anim = FlailLeft
  WaitAnimFinished
  goto OnGroundAI
endscript
script FlailRight
  FlailExceptions
  FlailVibrate
  PlayAnim Anim = FlailRight
  WaitAnimFinished
  goto OnGroundAI
endscript
script FlailVibrate
  if SpeedGreaterThan 600
    Vibrate Actuator = 1 Percent = 80 Duration = 0.25
  else
    Vibrate Actuator = 1 Percent = 50 Duration = 0.15
  endif
endscript
script GroundGone
  InAirExceptions
  SetException Ex = Ollied Scr = Ollie
  ClearTricksFrom GroundTricks
  if GotParam NoBoneless
    SetQueueTricks special = SpecialTricks AirTricks
  else
    SetQueueTricks special = SpecialTricks AirTricks Jumptricks JumpTricks0
  endif
  if Crouched
    PlayAnim Anim = Crouch2InAir
  else
    PlayAnim Anim = Idle2InAir
  endif
  begin
    if AirTimeGreaterThan Skater_Late_Jump_Slop
      ClearException Ollied
      SetQueueTricks special = SpecialTricks AirTricks
    endif
    DoNextTrick
    if GotParam AndManuals
      DoNextManualTrick
    endif
    if AnimFinished
      break
    endif
    WaitOneGameFrame
  repeat
  goto Airborne
endscript
AirAnimParams = { Hold BlendPeriod = 0.3 NoRestart }
script Airborne stretchtime = 1 BlendPeriod = 0.3
  InAirExceptions
  begin
    DoNextTrick
    if LeftPressed
      if Flipped
        PlayAnim Anim = AirTurnRight AirAnimParams
      else
        PlayAnim Anim = AirTurnLeft AirAnimParams
      endif
    else
      if RightPressed
        if Flipped
          PlayAnim Anim = AirTurnLeft AirAnimParams
        else
          PlayAnim Anim = AirTurnRight AirAnimParams
        endif
      else
        if AirTimeGreaterThan <stretchtime> seconds
          PlayAnim Anim = Stretch BlendPeriod = <BlendPeriod> NoRestart
          break
        else
          PlayAnim Anim = AirIdle Cycle BlendPeriod = <BlendPeriod> NoRestart
        endif
      endif
    endif
    WaitOneGameFrame
  repeat
  WaitAnimWhilstChecking
  PlayAnim Anim = StretchIdle BlendPeriod = 0.1 Cycle
endscript
script Land
  SetSkaterCamLerpReductionTimer time = 0
  if ProfileEquals is_named = demoness
    Demoness_FlameOn
  endif
  if AbsolutePitchGreaterThan 60
    if PitchGreaterThan 60
      goto PitchBail
    endif
  endif
  if SpeedGreaterThan 500
    if YawBetween (60, 120)
      if InSplitScreenGame
      else
        LaunchPanelMessage "&C1Landed sideways" properties = panelcombo
      endif
      goto YawBail
    endif
  endif
  if RollGreaterThan 50
    goto DoingTrickBail
  endif
  if DoingTrick
    goto DoingTrickBail
  endif
  if GotParam ReturnBacktoLipLand
  else
    if GotParam IgnoreAirTime
      goto Land2
    else
      if AirTimeLessThan 0.2 seconds
        goto Land2 Params = { LittleAir }
      else
        GotoPreserveParams Land2
      endif
    endif
  endif
endscript
script Land2 RevertTime = 5
  AllowRailTricks
  NollieOff
  ClearLipCombos
  if LandedFromVert
    Obj_ClearFlag FLAG_SKATER_MANUALCHEESE
    SetExtraTricks tricks = Reverts Duration = <RevertTime>
  else
    DoNextManualTrick FromAir
  endif
  Vibrate Actuator = 1 Percent = 80 Duration = 0.1
  if Crouched
    if GotParam LittleAir
      PlayAnim Anim = CrouchBump
    else
      if Backwards
        FlipAndRotate
        PlayAnim Anim = CrouchedLandTurn BlendPeriod = 0
        BoardRotate
      else
        if YawBetween (45, 60)
          if AirTimeGreaterThan 0.75 second
            PlayAnim Anim = SketchyCrouchLand BlendPeriod = 0.1
            if InSplitScreenGame
            else
              LaunchPanelMessage "&C1Sketchy" properties = panelcombo
            endif
          else
            PlayAnim Anim = CrouchedLand BlendPeriod = 0.1
          endif
        else
          PlayAnim Anim = CrouchedLand BlendPeriod = 0.1
        endif
      endif
    endif
  else
    if GotParam LittleAir
      PlayAnim Anim = IdleBump
    else
      if Backwards
        FlipAndRotate
        PlayAnim Anim = LandTurn BlendPeriod = 0
        BoardRotate
      else
        if YawBetween (45, 60)
          if AirTimeGreaterThan 0.75 second
            PlayAnim Anim = SketchyLand BlendPeriod = 0.1
            if InSplitScreenGame
            else
              LaunchPanelMessage "&C1Sketchy" properties = panelcombo
            endif
          else
            PlayAnim Anim = Land BlendPeriod = 0.1
          endif
        else
          PlayAnim Anim = Land BlendPeriod = 0.1
        endif
      endif
    endif
  endif
  ClearTrickQueue
  ClearEventBuffer buttons = [ x ]
  OnGroundExceptions_NoEndRun
  OnExceptionRun Landout
  SetManualTricks special = SpecialManualTricks ManualTricks
  if GotParam NoReverts
  else
    if LandedFromVert
      ResetLandedFromVert
      begin
        wait 1
      repeat <RevertTime>
    else
      begin
        DoNextManualTrick FromAir
        wait 1
      repeat 10
    endif
  endif
  CheckGapTricks
  ClearGapTricks
  ClearPanel_Landed
  ResetSpin
  ClearManualTrick
  OnGroundExceptions
  CheckforNetBrake
  WaitAnimWhilstChecking AndManuals
  goto OnGroundAI
endscript
script Landout
  if ExceptionTriggered RunHasEnded
    SetSpeed 0
    SetState Ground
    ClearExceptions
    SetException Ex = RunHasEnded Scr = EndOfRun
  else
    OnGroundExceptions_NoClear
  endif
  CheckGapTricks
  ClearGapTricks
  ClearPanel_Landed
  ClearManualTrick
endscript
script WaitOnGround
  begin
    if OnGround
      break
    endif
    WaitOneGameFrame
  repeat
endscript
script VibrateOff
  Vibrate Actuator = 0 Percent = 0
  Vibrate Actuator = 1 Percent = 0
endscript
script EndOfRun
  SetState Ground
  CleanUpSpecialItems
  StopBalanceTrick
  CheckGapTricks
  ClearGapTricks
  ClearPanel_Landed
  ClearExceptions
  ClearTrickQueues
  ClearEventBuffer
  SetException Ex = LostGame Scr = LostGame
  DisablePlayerInput AllowCameraControl
  SetException Ex = SkaterCollideBail Scr = EndBail
  WaitOnGround
  SetRollingFriction 19
  wait 10 frames
  WaitOnGround
  if SpeedGreaterThan 400
    PlayCessSound
    PlayAnim Anim = CessSlide180_FS
    WaitAnim 50 Percent
    PlayAnim Anim = CessSlide180_FS From = Current To = 0
    WaitAnimFinished
  else
    PlayAnim Anim = brake
    WaitAnimFinished
  endif
  PlayAnim Anim = BrakeIdle BlendPeriod = 0.3 Cycle
  begin
    if SpeedLessThan 40
      if OnGround
        break
      endif
    endif
    WaitOneGameFrame
  repeat
  if InNetGame
    SetException Ex = WonGame Scr = WonGame
    SetException Ex = LostGame Scr = LostGame
  endif
  wait 1 seconds
  if IsCareerMode
    SetSkaterCamOverride heading = 3 tilt = 0.2 time = 2
  endif
  EndOfRunDone
  SetException Ex = LostGame Scr = LostGame
  wait 1 seconds
  if InNetGame
    if GameIsOver
    else
      EnterObserveMode2
      if GameModeEquals is_king
      else
        LaunchPanelMessage "Waiting for other players to finish their combos..." properties = netstatusprops
      endif
    endif
  endif
endscript
script ForceEndOfRun
  VibrateOff
  MakeSkaterGoto EndOfRun
endscript
script SkaterCollide_AtEndRun
endscript
script WonGame
  PlayAnim random( @Anim = Prop @Anim = Cheer1 ) BlendPeriod = 0.3
  EndOfRunDone
  WaitAnimFinished
  if ShouldMongo
    PlayAnim Anim = MongoBrakeIdle Cycle
  else
    PlayAnim Anim = BrakeIdle Cycle
  endif
endscript
script LostGame
  PlayAnim Anim = BrakeDefeat
  WaitAnimFinished
  EndOfRunDone
  if ShouldMongo
    PlayAnim Anim = MongoBrakeIdle Cycle
  else
    PlayAnim Anim = BrakeIdle Cycle
  endif
  WaitAnimFinished
endscript
script EndBail
  EndOfRunDone
  ClearExceptions
  InBail
  TurnToFaceVelocity
  VibrateOff
  PlaySound random( @BoardBail01 @BoardBail02) 
  PlayAnim Anim = SlipForwards NoRestart BlendPeriod = 0.3
  wait 10 frames
  SwitchOffBoard
  wait 10 Frame
  PlaySound random( @HitBody03 @HitBody04) 
  wait 10 frames
  SetRollingFriction 18
  Vibrate Actuator = 1 Percent = 100 Duration = 0.2
  Obj_Spawnscript BloodSmall
  WaitAnim 25 Percent fromend
  PlaySound FoleyMove01 vol = 50
  WaitAnimFinished
  Obj_Spawnscript BloodPool
  PlayAnim Anim = GetUpForwards BlendPeriod = 0.1
  SetRollingFriction 20
  wait 50 frames
  SwitchOnBoard
  BoardRotate normal
  WaitAnimFinished
  NotifyBailDone
  NotInBail
  goto EndOfRun
endscript
script EndOfRunTimeOut
  EndOfRunDone
endscript
script CarPlant
  ClearExceptions
  InAirExceptions
  ClearException CarPlant
  PlaySound GrindMetalOn random( @pitch = 80 @pitch = 90 @pitch = 85) 
  Obj_Spawnscript CarSparks
  DoCarPlantBoost
  SetTrickName "Car Plant"
  SetTrickScore 400
  Display
  if DoingTrick
  else
    PlayAnim Anim = Beanplant BlendPeriod = 0.3
    WaitAnimWhilstChecking
    goto Airborne
  endif
endscript
script CarSparks
  SparksOn
  wait 20 gameframes
  SetException Ex = CarPlant Scr = CarPlant
  wait 180 gameframes
  SparksOff
endscript
script FreezeSkater
  SwitchOnBoard
  ClearExceptions
  SetQueueTricks NoTricks
  DisablePlayerInput
  SetRollingFriction 10000
  PlayAnim Anim = StartIdle Cycle
endscript
script CarBail
  InBail
  Obj_Spawnscript BloodCar
  PlaySound hitdumpsterX
  PlaySound hitvehicle3
  goto NoseManualBail
endscript
script FlailingFall
  if Obj_FlagNotSet FLAG_SKATER_KILLING
    Obj_SetFlag FLAG_SKATER_KILLING
    InBail
    Obj_ClearFlag FLAG_SKATER_KILLING
  else
    InBail
  endif
  ClearExceptions
  SetQueueTricks NoTricks
  DisablePlayerInput AllowCameraControl
  PlayAnim Anim = StretchtoFlailingFall BlendPeriod = 0.3
  ClearGapTricks
  ClearPanel_Bailed
  WaitAnim 90 Percent
  SwitchOffBoard
  WaitAnimFinished
  PlayAnim Anim = FlailingFall Cycle
endscript
script RestartSkater
  RestartSkaterExceptions
  ClearGapTricks
  ClearPanel_Bailed
  EnablePlayerInput
  OnGroundExceptions
  WaitAnimFinished
  goto OnGroundAI
endscript
script DropIn DropInAnim = DropIn
  SetState Lip
  SetSkaterCamOverride heading = 0 tilt = 0.05 time = 0.0
  RestartSkaterExceptions
  SetRollingFriction 10000
  DisablePlayerInput
  PlayAnim Anim = <DropInAnim> BlendPeriod = 0.0
  WaitAnim 20 Percent
  SetSkaterCamOverride heading = 0 tilt = 0 time = 2
  WaitAnim 85 Percent
  SetState Ground
  SetRollingFriction 0
  WaitAnimFinished
  EnablePlayerInput
  ClearSkaterCamOverride
  OnGroundExceptions
  WaitOneGameFrame
  goto Land
endscript
script StartLevelSkating
  RestartSkaterExceptions
  if AutoKickIsOff
    if ShouldMongo
      PlayAnim Anim = MongoBrakeIdle
    else
      PlayAnim Anim = BrakeIdle
    endif
  else
    DisablePlayerInput
    SetRollingFriction 100000
    PlayAnim Anim = StartIdle
    wait 2 frames
    WaitAnimFinished
    SetRollingFriction #"default"
    PlayAnim Anim = StartSkating1 BlendPeriod = 0.0
    WaitAnim 75 Percent
  endif
  SetRollingFriction #"default"
  EnablePlayerInput
  ClearSkaterCamOverride
  OnGroundExceptions
  WaitAnimFinished
  goto OnGroundAI
endscript
script StartSkating1
  RestartSkaterExceptions
  SetRollingFriction #"default"
  DisablePlayerInput AllowCameraControl
  if ProfileEquals is_named = mullen
    SetRollingFriction 10000
    PlayAnim Anim = MullenStart BlendPeriod = 0.0
    WaitAnim 40 Percent
    PlaySound boneless09 pitch = 110
    PlayBonkSound
    BlendperiodOut 0
    WaitAnim 60 Percent
    SetRollingFriction #"default"
  else
    if AutoKickIsOff
      if ShouldMongo
        PlayAnim Anim = MongoBrakeIdle
      else
        PlayAnim Anim = BrakeIdle
      endif
    else
      PlayAnim Anim = StartSkating1 BlendPeriod = 0.0
      WaitAnim 75 Percent
    endif
  endif
  ClearSkaterCamOverride
  EnablePlayerInput
  OnGroundExceptions
  WaitAnimFinished
  goto OnGroundAI
endscript
script StartSkatingCanada
  Obj_Spawnscript CanadaBreath
  goto StartSkating11
endscript
script CanadaBreath
  Obj_Attachframe bone = "head"
  begin
    BloodParticlesOn name = "breath.png" start_col = -8355712 end_col = 1434484864 num = 1 emit_w = 1.0 emit_h = 1.0 angle = 90 size = 4 bone = "head" growth = 6 time = 2 speed = 20 grav = 0 life = 2
    wait 10 frames
  repeat
endscript
script PedProps name = "Ped Props" score = 500
  if InSplitScreenGame
  else
    LaunchPanelMessage "Spectator Bonus" properties = Panelcombo2
  endif
  PlaySound PedProps vol = 500
  SetTrickName <name>
  SetTrickScore <score>
  Display BlockSpin
  WaitOneGameFrame
  if OnGround
    if DoingTrick
    else
      CheckGapTricks
      ClearGapTricks
      ClearPanel_Landed
    endif
  endif
endscript
script RestartSkaterExceptions
  LeaveObserveMode2
  AllowRailTricks
  BoardRotate normal
  ClearExceptions
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  ClearTrickQueue
  SetQueueTricks NoTricks
  ClearManualTrick
  ClearEventBuffer
  SwitchOnBoard
  Obj_Attachframe bone = "head"
endscript
script NetPause
endscript
script Foobar
  if OnGround
    SetRollingFriction 100
    CessBrake
    HandBrake
  else
    WaitOnGround
    Land
    HandBrake
  endif
endscript
script LaunchSpecialMessage text = "Special Trick"
  PlaySound HUD_specialtrickAA vol = 100
  if InSplitScreenGame
  else
    LaunchPanelMessage <text> properties = panelcombo
  endif
endscript
script LaunchExtraMessage text = "Hidden Combo!"
  PlaySound HUD_specialtrickAA vol = 100
  if InSplitScreenGame
  else
    LaunchPanelMessage <text> properties = panelcombo
  endif
endscript
script MakeSkaterFly
  MakeSkaterGoto FlyAi
endscript
script FlyAi move = 8
  ClearExceptions
  SetQueueTricks NoTricks
  SetSpeed 0
  SetState Air
  SetRollingFriction 1000
  NoRailTricks
  begin
    if held R2
      Jump
      wait 8 frames
    endif
    if held L1
      break
    endif
    if held L2
      if held square
        move x = 18
      endif
      if held Circle
        move x = -18
      endif
      if held Triangle
        move z = 18
      endif
      if held x
        move z = -18
      endif
    else
      if held square
        move x = 6
      endif
      if held Circle
        move x = -6
      endif
      if held Triangle
        move z = 8
      endif
      if held x
        move z = -8
      endif
    endif
    WaitOneGameFrame
  repeat
  AllowRailTricks
  MakeSkaterGoto OnGroundAI
endscript
script PedKnockDown
  Obj_Spawnscript BloodCar
  SetRollingFriction 0
  goto NoseManualBail
endscript
script SkaterPushPed
  SetException Ex = CarBail Scr = CarBail
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  FlailVibrate
  NollieOff
  StopBalanceTrick
  PlaySound bitchslap2
  if AnimEquals [ CrouchIdle SkateIdle Land MongoPushCycle PushCycle ]
    goto FlailHitWall
  endif
  if AnimEquals [ runout runoutquick Smackwallupright ]
    goto Bailsmack Params = { SmackAnim = Smackwallupright }
  endif
  if InAir
    goto Airborne
  else
    if IsInBail
      if AnimEquals [ Smackwallfeet NutterFallBackward FiftyFiftyFallBackward BackwardsTest BackwardFaceSlam Smackwallfeet SlipBackwards ]
        goto Bailsmack Params = { SmackAnim = Smackwallfeet }
      else
        goto Bailsmack Params = { <...> }
      endif
    endif
    goto OnGroundAI
  endif
endscript
script SkaterBreakGlass
  SetException Ex = CarBail Scr = CarBail
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  FlailVibrate
  NollieOff
  StopBalanceTrick
  if AnimEquals [ CrouchIdle SkateIdle Land MongoPushCycle PushCycle ]
    goto FlailHitWall
  endif
  if AnimEquals [ runout runoutquick Smackwallupright ]
    goto Bailsmack Params = { SmackAnim = Smackwallupright }
  endif
  if InAir
    goto Airborne
  else
    if IsInBail
      if AnimEquals [ Smackwallfeet NutterFallBackward FiftyFiftyFallBackward BackwardsTest BackwardFaceSlam Smackwallfeet SlipBackwards ]
        goto Bailsmack Params = { SmackAnim = Smackwallfeet }
      else
        goto Bailsmack Params = { <...> }
      endif
    endif
    goto OnGroundAI
  endif
endscript
script SkaterCollideBall
  if InAir
    InAirExceptions
    Obj_Spawnscript CarSparks
    PlayAnim Anim = Boneless BlendPeriod = 0.3
    SetTrickName "Scratchin the Ball!"
    SetTrickScore 400
    Display
    WaitAnimWhilstChecking AndManuals
    goto Airborne
  else
    InBail
    Obj_Spawnscript BloodCar
    PlaySound bodysmackA
    goto NoseManualBail
    LaunchPanelMessage "Ball Busted!"
  endif
endscript
