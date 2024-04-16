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
 * @file   uwdsync_ref.h
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Class that represents a node of UWPOLLING
 *
 *
 */

#ifndef UwDSync_REF_H
#define UwDSync_REF_H

// #include "uwdsync_cmn_hdr.h" 
#include "mmac.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <list>
#include <chrono>


// 	"DNP" /**< The node is not in the polling list */

	/**
 * Class used to represents the UWPOLLING MAC layer of a node.
 */
class UwDSync_ref : public MMac
{
public:
	/**
	 * Constructor of the Uwpolling_SINK class
	 */
	UwDSync_ref();
	/**
	 * Destructor of the Uwpolling_SINK class
	 */
	virtual ~UwDSync_ref();
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
	enum UWPOLLING_SINK_STATUS {
		UWPOLLING_SINK_STATUS_IDLE = 1,
		UWPOLLING_SINK_STATUS_RX_TRIGGER,
		UWPOLLING_SINK_STATUS_TX_PROBE,
		UWPOLLING_SINK_STATUS_WAIT_DATA,
		UWPOLLING_SINK_STATUS_RX_DATA,
		UWPOLLING_SINK_STATUS_TX_ACK
	};

	/**< Type of the packet */
	enum UWPOLLING_PKT_TYPE {
		UWPOLLING_DATA_PKT = 1,
		UWPOLLING_POLL_PKT,
		UWPOLLING_TRIGGER_PKT,
		UWPOLLING_PROBE_PKT,
		UWPOLLING_ACK_PKT
	};

	
	/**
	 * Class that describes the timer in the SINK
	 */
	class UwDSync_Timer : public TimerHandler
	{
	public:
		/**
		 * Constructor of the Uwpolling_SINK_Timer class
		 * @param Uwpolling_SINK* a pointer to an object of type Uwpolling_AUV
		 */
		UwDSync_Timer(UwDSync_ref *m)
			: TimerHandler()
			, module(m)
			
		{
			assert(m != NULL);
		}

		
		virtual ~UwDSync_Timer()
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
		UwDSync_ref *module; /**< Pointer to an object of type Uwpolling_SINK */
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


	// /**
	//  * Returns the number of ACK packets sent during the simulation
	//  * @return int n_ack_sent the number of ACK packets sent
	//  */
	// inline int
	// getAckSent()
	// {
	// 	return n_ack_sent;
	// }


	/**
	 * IDLE state. Each variable is resetted
	 */
	virtual void stateIdle();

	// /**
	//  * The ACK Packet is sent down to the PHY layer
	//  */
	// virtual void txAck();

	// /**
	//  * State in which the ACK is sent
	//  */
	// virtual void stateTxAck();

	/**
	 * Calculate the epoch of the event. Used in sea-trial mode
	 * @return the epoch of the system
	 */
	inline unsigned long int
	getEpoch()
	{
	  unsigned long int timestamp =
		  (unsigned long int) (std::chrono::duration_cast<std::chrono::milliseconds>(
			  std::chrono::system_clock::now().time_since_epoch()).count() );
	  return timestamp;
	}
};
#endif