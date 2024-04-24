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
 * @file   uwdsync_node.h
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Class that represents a node of UWPOLLING
 *
 *
 */

#ifndef UwDSync_NODE_H
#define UwDSync_NODE_H

#include "uwdsync_cmn_hdr.h"
#include "mmac.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <chrono>



const long double Alpha = 0.0005; 
const long double Beta = 0.05; 
/**
 * Class used to represents the UWPOLLING MAC layer of a node.
 */
class UwDSync_node : public MMac
{
public:
	/**
	 * Constructor of the Uwpolling_NODE class
	 */
	UwDSync_node();
	/**
	 * Destructor of the Uwpolling_NODE class
	 */
	virtual ~UwDSync_node();
	/**
	 * TCL command interpreter. It implements the following OTcl methods:
	 *
	 * @param argc Number of arguments in <i>argv</i>.
	 * @param argv Array of strings which are the command parameters (Note that
	 *<i>argv[0]</i> is the name of the object).
	 * @return TCL_OK or TCL_ERROR whether the command has been dispatched
	 *successfully or not.
	 *
	 **/
	virtual int command(int argc, const char *const *argv);
	/**
	 * Cross-Layer messages interpreter
	 *
	 * @param ClMessage* an instance of ClMessage that represent the message
	 * received
	 * @return <i>0</i> if successful.
	 */
	virtual int crLayCommand(ClMessage *m);

protected:
	/**< Variable that rapresents the status of the protocol machine state */
	

	/**
	 * Class that describes the timer in the Node
	 */
	class UwDSync_node_Timer : public TimerHandler
	{
	public:
		/**
		 * Constructor of the Uwpolling_NODE_Timer class
		 * @param UwDSync_node* a pointer to an object of type UwDSync_node
		 */
		UwDSync_node_Timer(UwDSync_node *m)
			: TimerHandler()
			, module(m)
		{
			assert(m != NULL);
		}

		/**
		 * Destructor of the Uwpolling_NODE_Timer class
		 */
		virtual ~UwDSync_node_Timer()
		{
		}

		/**
		 * stops the timer
		 */
		virtual void
		stop()
		{
			force_cancel();
		}

		
	protected:
		double start_time; /**< Start Time of the timer */
		double left_duration; /**< Left duration of the timer */
		int counter; /**< counter of the timer */
		UwDSync_node *module; /**< Pointer to an object of type Uwpolling_AUV */
	};


	/**
	 * Pass the packet to the PHY layer
	 * @param Packet* Pointer to an object of type Packet that rapresent the
	 * Packet to transmit
	 */
	virtual void Mac2PhyStartTx(Packet *p);
	/**
	 * Method called when the PHY layer finish to transmit the packet.
	 * @param Packet* Pointer to an object of type Packet that rapresent the
	 * Packet transmitted
	 */
	virtual void Phy2MacEndTx(const Packet *p);
	/**
	 * Method called when the Phy Layer start to receive a Packet
	 * @param const Packet* Pointer to an object of type Packet that rapresent
	 * the Packet that is in reception
	 */
	virtual void
	Phy2MacStartRx(const Packet *p)
	{
	}
	/**
	 * Method called when the Phy Layer finish to receive a Packet
	 * @param const Packet* Pointer to an object of type Packet that rapresent
	 * the packet received
	 */
	virtual void Phy2MacEndRx(Packet *p);
	// /**
	//  * Inits the packet with the MAC address of the receiver and the sender,
	//  * the size of the packet and the type
	//  * @param UWPOLLING_PKT_TYPE the type of the packet
	//  */
	// virtual void initPkt(UWPOLLING_PKT_TYPE pkt_type);
	// /**
	//  * Initializes the protocol at the beginning of the simulation. This method
	//  * is called by
	//  * a command in tcl.
	//  * @param double delay
	//  * @see command method
	//  */
	// virtual void initInfo();

	virtual void stateIdle();
	/**
	 * State of the protocol in which a DATA packet is sent. Here we compute
	 * some statistics
	 * related to the transmission of the packet (i.e. the number of data
	 * packets transmitted
	 * is incremented and we check the number of data packet to transmit
	 */
	virtual void stateTxData();

	/**
	 * Calculate the epoch of the event. Used in sea-trial mode
	 * @return the epoch of the system
	 */
	
};
#endif