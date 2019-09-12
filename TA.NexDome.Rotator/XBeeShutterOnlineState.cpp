// 
// 
// 

#include "XBeeShutterOnlineState.h"
#include "XBeeApiDetectShutterState.h"

void XBeeShutterOnlineState::OnEnter()
	{
	std::cout << "*** Final CCA " << std::hex << machine.CCA << std::dec << std::endl;
	static const std::string ack(XBEE_HELLO_ACK);
	machine.SendToRemoteXbee(ack);
	timer.SetDuration(XBEE_NO_HEARTBEAT_TIMEOUT);
	}

/*
Timer will expire when we haven't received anything from the shutter
for a while, which means the link may be down and we have to re-detect
the shutter. No shutter commands will be possible until the link is
re-established and confirmed.
*/
void XBeeShutterOnlineState::OnTimerExpired()
	{
	machine.ChangeState(new XBeeApiDetectShutterState(machine));
	}


void XBeeShutterOnlineState::OnApiRx64FrameReceived(const std::vector<byte>& payload)
	{
	static const std::string ShutterHelloMessage(XBEE_HELLO_MESSAGE);
	// Check whether the message is correct, otherwise ignore frame
	// Skip first 10 bytes of payload, look for string in bytes 11 onward.
	const auto msgStart = payload.begin() + 10;
	const auto msgEnd = payload.end();
	const std::string rxMessage(msgStart, msgEnd);
	if (rxMessage.compare(XBEE_HELLO_MESSAGE) == 0)
		{
		machine.SetDestinationAddress(payload);
		machine.ChangeState(new XBeeShutterOnlineState(machine));
		return;
		}

	// If no other interpretation seems appropriate, then send to the host.
	std::cout << rxMessage << std::endl;
	}

void XBeeShutterOnlineState::SendCommand(std::string& command)
	{
	machine.SendToRemoteXbee(command);
	}
