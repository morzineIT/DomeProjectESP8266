﻿#include "XBeeWaitForCommandModeState.h"
#include "XBeeStartupState.h"
#include "XBeeApiDetectShutterState.h"

void XBeeWaitForCommandModeState::OnTimerExpired()
	{
		if (timer.Expired())
		{
		machine.ChangeState(new XBeeStartupState(machine));
		}

	}

void XBeeWaitForCommandModeState::OnEnter()
	{
	timer.SetDuration(XBEE_AT_GUARD_TIME * 2);
	}

void XBeeWaitForCommandModeState::OnSerialLineReceived(String& rxData) 
	{
	if (rxData != "OK")
		return;
	// Set serial comms parameters and API mode 2
	machine.SendToXbee(XBEE_ROTATOR_INIT_STRING);	// Enter API mode.
	machine.ListenInApiMode();
	machine.ChangeState(new XBeeApiDetectShutterState(machine));
	}


