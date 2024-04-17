//
// Copyright (c) 2017 Regents of the SIGNET lab, University of Padova.
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
 * @file   uwdsync_node.cpp
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Implementation of Uwpolling_NODE class
 *
 */

#include "uwdsync_node.h"
#include "uwpolling_cmn_hdr.h"
#include "mac.h"
#include "mmac.h"

#include "uwcbr-module.h"
#include "rng.h"

#include <sstream>
#include <sys/time.h>

/**
 * Class that represents the binding with the tcl configuration script
 */
static class UwDSync_NODE : public TclClass
{
public:
	/**
	 * Constructor of the class
	 */
	UwDSync_NODE()
		: TclClass("Module/UW/DSYNC/NODE")
	{
	}

	/**
	 * Creates the TCL object needed for the tcl language interpretation
	 * @return Pointer to an TclObject
	 */
	TclObject *
	create(int, const char *const *)
	{
		return (new UwDSync_Node());
	}
} class_module_uwdsync_node;

UwDSync_node::UwDSync_node()
	: MMac()
{
}

UwDSync_node::~UwDSync_node()
{
}

int
UwDSync_Node::command(int argc, const char *const *argv)
{

}

int
UwDSync_Node::crLayCommand(ClMessage *m)
{
	switch (m->type()) {
		default:
			return MMac::crLayCommand(m);
	}
}



void
UwDSync_Node::Mac2PhyStartTx(Packet *p)
{
	MMac::Mac2PhyStartTx(p);
}

void
Uwpolling_NODE::Phy2MacEndTx(const Packet *p)
{
	// switch (curr_state) {
	// 	case (UWPOLLING_NODE_STATUS_TX_PROBE): {
	// 		refreshReason(UWPOLLING_NODE_REASON_TX_PROBE);
	// 		stateWaitPoll();
	// 		break;
	// 	}
	// 	case (UWPOLLING_NODE_STATUS_TX_DATA): {
	// 		refreshReason(UWPOLLING_NODE_REASON_TX_DATA);
	// 		if (LastPacket) {
	// 			stateIdle();
	// 		} else {
	// 			Packet *dummy = Packet::alloc();
	// 			hdr_cmn *ch = HDR_CMN(dummy);
	// 			ch->size() = max_payload;
	// 			hdr_mac *mach = HDR_MAC(dummy);
	// 			mach->macDA() = AUV_mac_addr;
	// 			// double dataPacket_duration = Mac2PhyTxDuration(dummy);
	// 			// double dataPacket_duration =
	// 			// (max_payload*8)/modem_data_bit_rate;
	// 			double dataPacket_duration = Intra_data_Guard_Time;
	// 			// cout << "Data Packet duration" << dataPacket_duration <<
	// 			// endl;
	// 			Packet::free(dummy);
	// 			/*
	// 			 * Instead of transmit the new packet just after the
	// 			 * transmission of the previous one
	// 			 * i wait for a little time (the duration of the reception of
	// 			 * the old one).
	// 			 * This way, the AUV doesn't drop the new one because it is
	// 			 * synchronized on the old one
	// 			 */
	// 			if (dataPacket_duration < 0) {
	// 				cerr << "Scheduling Tx_Data timer ----> negative value "
	// 					 << dataPacket_duration << endl;
	// 				dataPacket_duration = 8;
	// 			}
	// 			tx_data_timer.schedule(dataPacket_duration);
	// 		}
	// 		break;
	// 	}
	// 	default: {
	// 		if (debug_)
	// 		    std::cerr << getEpoch() << "::" << NOW << "::UWPOLLING_NODE(" << addr
	// 					  << ")::PHY2MACENDTX::logical_error,state = "
	// 					  << status_info[curr_state]
	// 					  << ", prev_state = " << status_info[prev_state]
	// 					  << std::endl;
	// 	}
	// }
}

void
Uwpolling_NODE::stateIdle()
{
	// if (debug_)
	//     std::cout << getEpoch() << "::" << NOW << "::Uwpolling_NODE(" << addr << ")::IDLE_STATE"
	// 			  << std::endl;
	// refreshState(UWPOLLING_NODE_STATUS_IDLE);
	// polled = false;
	// Triggered = false;
	// RxPollEnabled = false;
	// backoff_timer.force_cancel();
	// rx_poll_timer.force_cancel();
}

void
Uwpolling_NODE::Phy2MacEndRx(Packet *p)
{
	// hdr_cmn *ch = HDR_CMN(p);
	// packet_t rx_pkt_type = ch->ptype();
	// hdr_mac *mach = HDR_MAC(p);
	// int dest_mac = mach->macDA();
	// if (ch->error()) {

	// 	if (ch->ptype() == PT_TRIGGER) {
	// 		if (debug_)
	// 		    std::cout << getEpoch() << "::" << NOW << "::Uwpolling_NODE(" << addr
	// 					  << ")::PHY2MACENDRX::DROP_TRIGGER" << std::endl;
	// 		if (sea_trial && print_stats)
	// 			out_file_stats << left << getEpoch() << "::" << NOW
	// 						   << "::Uwpolling_NODE(" << addr
	// 						   << ")::PKT_TRIGGER_DROP_ERROR" << endl;
	// 		incrTriggerDropped();
	// 	} else if (ch->ptype() == PT_POLL) {
	// 		if (debug_)
	// 		    std::cout << getEpoch() << "::" << NOW << "::Uwpolling_NODE(" << addr
	// 					  << ")::PHY2MACENDRX::DROP_POLL" << std::endl;
	// 		if (sea_trial && print_stats)
	// 			out_file_stats << left << getEpoch() << "::" << NOW
	// 						   << "::Uwpolling_NODE(" << addr
	// 						   << ")::PKT_POLL_DROP_ERROR" << endl;
	// 		incrPollDropped();
	// 	}
	// 	refreshReason(UWPOLLING_NODE_REASON_PKT_ERROR);
	// 	drop(p, 1, UWPOLLING_NODE_DROP_REASON_ERROR);
	// } else if ((dest_mac == addr) || (dest_mac == (int) MAC_BROADCAST)) {
	// 	if (rx_pkt_type == PT_TRIGGER) {
	// 		refreshReason(UWPOLLING_NODE_REASON_RX_TRIGGER);
	// 		curr_trigger_pkt = p->copy();
	// 		Packet::free(p);
	// 		stateRxTrigger();
	// 	} else if (rx_pkt_type == PT_POLL) {
	// 		refreshReason(UWPOLLING_NODE_REASON_RX_POLL);
	// 		hdr_POLL *pollh = HDR_POLL(p);
	// 		if (debug_)
	// 		    std::cout << getEpoch() << "::" << NOW << "::Uwpolling_NODE(" << addr
	// 					  << ")::PHY2MACENDRX::RX_POLL::POLLED_NODE= "
	// 					  << pollh->id_ << std::endl;
	// 		rx_poll_timer.force_cancel();
	// 		curr_poll_pkt = p->copy();
	// 		Packet::free(p);
	// 		stateRxPoll();
	// 	} else {
	// 		drop(p, 1, UWPOLLING_NODE_DROP_REASON_UNKNOWN_TYPE);
	// 	}
	// } else {
	// 	incrXCtrlPktsRx();
	// 	if (rx_pkt_type == PT_TRIGGER) {
	// 		if (debug_)
	// 		    std::cout << getEpoch() << "::" << NOW << "::Uwpolling_NODE(" << addr
	// 					  << ")::PHY2MACENDRX::WARNING!!TRIGGER packet not in "
	// 						 "broadcast!!"
	// 					  << std::endl;

	// 	} else if (rx_pkt_type == PT_POLL) {
	// 		if (debug_)
	// 		    std::cout << getEpoch() << "::" << NOW << "::Uwpolling_NODE(" << addr
	// 					  << ")::PHY2MACENDRX::WARNING!!POLL packet not in "
	// 						 "broadcast!!"
	// 					  << std::endl;
	// 	}
	// 	drop(p, 1, UWPOLLING_NODE_DROP_REASON_WRONG_RECEIVER);
	// }
}
