//
// Copyright (c) 2019 Regents of the SIGNET lab, University of Padova.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the University of Padova (SIGNET lab) nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

/**
 * @file   uwdsync_ref.cpp
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Implementation of Uwpolling_SINK class
 *
 */

#include "uwdsync_ref.h"
#include "mmac.h"
#include "mac.h"
#include "uwcbr-module.h"
#include "mphy_pktheader.h"
#include "rng.h"

#include <algorithm>
#include <sstream>
#include <sys/time.h>


/**
 * Class that represents the binding with the tcl configuration script
 */
static class UwDSync_REF : public TclClass
{
public:
	/**
	 * Constructor of the class
	 */
	UwDSync_ref()
		: TclClass("Module/UW/DSYNC/REF")
	{
	}

	/**
	 * Creates the TCL object needed for the tcl language interpretation
	 * @return Pointer to an TclObject
	 */
	TclObject *
	create(int, const char *const *)
	{
		return (new UwDSync_ref());
	}
} class_module_uwdsync_ref;

UwDSync_ref::UwDSync_ref()
	: MMac()

{
	
}

UwDSync_ref::~UwDSync_ref()
{
}

int
UwDSync_ref::command(int argc, const char *const *argv)
{
}

int
UwDSync_ref::crLayCommand(ClMessage *m)
{
	switch (m->type()) {
		default:
			return MMac::crLayCommand(m);
	}
}


void 
UwDSync_ref::Phy2MacEndRx(Packet *p)
{
	// hdr_cmn *ch = HDR_CMN(p);
	// hdr_mac *mach = HDR_MAC(p);
	// int dest_mac = mach->macDA();
	// if (ch->error()) { //discard packet
	// 	//DISCARD PACKET AND INCREMENTS DEDICATED COUNTER
	// 	if (ch->ptype() == PT_TRIGGER) {
	// 		if (debug_)
	// 			std::cout << NOW << "Uwpolling_SINK(" << addr
	// 					  << ")::PHY2MACENDRX::DROP_TRIGGER" << std::endl;
	// 		if (sea_trial && print_stats)
	// 			out_file_stats << left << "[" << getEpoch() << "]::" << NOW
	// 						   << "::Uwpolling_SINK(" << addr
	// 						   << ")::PKT_TRIGGER_DROP_ERROR" << endl;
	// 		incrTriggerDropped();
	// 	} 
		// refreshReason(UWPOLLING_SINK_REASON_PKT_ERROR);
		// drop(p, 1, UWPOLLING_SINK_DROP_REASON_ERROR);
	// } else {
	// 	if (dest_mac == addr || dest_mac == (int) MAC_BROADCAST) { //Check MAC address
	// 		if (ch->ptype() == PT_TRIGGER) {
	// 			curr_trigger_pkt = p->copy();
	// 			Packet::free(p);
	// 			refreshReason(UWPOLLING_SINK_REASON_RX_TRIGGER);
	// 			stateRxTrigger();
	// 		} else if (ch->ptype() != PT_POLL && ch->ptype() != PT_PROBE &&
	// 				ch->ptype() != PT_ACK_SINK) { //data pkt
				
	// 			curr_data_pkt = p->copy();
	// 			Packet::free(p);
	// 			refreshReason(UWPOLLING_SINK_REASON_RX_DATA);
	// 			stateRxData();
	// 		}  else {
	// 			if (ch->ptype() == PT_POLL && RxDataEnabled && useAdaptiveTdata) {
	// 				hdr_POLL *pollh = HDR_POLL(p);
	// 				rx_data_timer.schedule(pollh->POLL_time());
	// 				if (debug_)
	// 					std::cout << NOW << "Uwpolling_SINK(" << addr
	// 							<< ")::Resched rx data timer, timeout=" 
	// 							<< pollh->POLL_time() << std::endl;
	// 			}
	// 			// PT_PROBE, PT_ACK_SINK and PT_POLL are not considerd by 
	// 			// the SINK
	// 			drop(p, 1, UWPOLLING_SINK_DROP_REASON_UNKNOWN_TYPE);
	// 		}
	// 	} else {
	// 		//PACKET NOT FOR ME, DISCARD IT
	// 		drop(p, 1, UWPOLLING_SINK_DROP_REASON_WRONG_RECEIVER);
	// 	}
	// }
}

void
UwDSync_ref::stateIdle()
{
	// /* Reset timer and move to STATE_IDLE*/
	// if (debug_) {
	// 	std::cout << NOW << "Uwpolling_SINK(" << addr << ")::IDLE_STATE"
	// 			  << std::endl;
	// }
	// refreshState(UWPOLLING_SINK_STATUS_IDLE);
	// Triggered = false;
	// RxDataEnabled = false;
	// send_ACK = false;
	// n_curr_rx_pkts = 0;
	// first_rx_pkt = true;
	// triggerEnabled = true;
	// backoff_timer.force_cancel();
	// rx_data_timer.force_cancel();
}



void 
UwDSync_ref::Mac2PhyStartTx(Packet *p)
{
	MMac::Mac2PhyStartTx(p); // Send down the packet to phy layer
}

void 
UwDSync_ref::Phy2MacEndTx(const Packet *p)
{
	// hdr_cmn* ch = HDR_CMN(p); 
	// if (ch->ptype() == PT_PROBE_SINK) {
	// 	refreshReason(UWPOLLING_SINK_REASON_TX_PROBE);
	// 	stateWaitData();
	// } else if (ch->ptype() == PT_ACK_SINK) {
	// 	refreshReason(UWPOLLING_SINK_REASON_TX_ACK);
	// stateIdle();
	// }
}

// void
// Uwpolling_SINK::stateTxAck()
// {
// 	if (send_ACK) {
// 		if(debug_)
// 			std::cout << NOW << "Uwpolling_SINK(" << addr
// 					<< ")stateTxAck()" << std::endl;
// 		refreshState(UWPOLLING_SINK_STATUS_TX_ACK);
// 		initPkt(UWPOLLING_ACK_PKT);
// 		txAck();
// 	}
// }

// void
// Uwpolling_SINK::txAck()
// {
// 	send_ACK = false;
// 	incrAckSent();
// 	incrCtrlPktsTx();
// 	Mac2PhyStartTx(curr_ack_pkt);
// }
