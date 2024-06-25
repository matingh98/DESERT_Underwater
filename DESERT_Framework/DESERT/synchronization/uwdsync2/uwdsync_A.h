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
 * @file   uwdsync_A.h
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Class that represents a node of UWDSYNC
 *
 *
 */
#ifndef UWDSYNC_A_H
#define UWDSYNC_A_H

#include "uwdsync2_cmn_hdr.h"
#include "mmac.h"
#include "packet.h"
#include "rng.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <list>
#include <chrono>

/**
 * Class used to represent the UWDSYNC MAC layer of a node.
 */
class UwDSync_A : public MMac
{
public:
    /**
     * Constructor of the UwDSync_A class
     */
    UwDSync_A();
    
    /**
     * Destructor of the UwDSync_A class
     */
    virtual ~UwDSync_A();
    
    /**
     * TCL command interpreter. It implements the following OTcl methods:
     *
     * @param argc Number of arguments in <i>argv</i>.
     * @param argv Array of strings which are the command parameters (Note that
     * <i>argv[0]</i> is the name of the object).
     * @return TCL_OK or TCL_ERROR whether the command has been dispatched
     * successfully or not.
     */
    virtual int command(int argc, const char *const *argv);
    
    /**
     * Cross-Layer messages interpreter
     *
     * @param ClMessage* an instance of ClMessage that represents the message
     * received
     * @return <i>0</i> if successful.
     */
    virtual int crLayCommand(ClMessage *m);

    /**
     * Initial timer to start the synchronization process
     */
    virtual double initialTimer();
    
    /**
     * State to transmit data
     */
    virtual void stateTxData();
    
    /**
     * Initializes the packet
     */
    virtual void initPkt(Packet *p);
    
	virtual void stateRxTrigger(Packet *p);

    
    virtual void stateIdle(Packet *p = nullptr);

    
    /**
     * Pass the packet to the PHY layer
     * @param Packet* Pointer to an object of type Packet that represents the
     * Packet to transmit
     */

    /**
     * Method called when the PHY layer finishes transmitting the packet.
     * @param Packet* Pointer to an object of type Packet that represents the
     * Packet transmitted
     */
    virtual void Phy2MacEndTx(const Packet* p);

    /**
     * Method called when the Phy Layer starts to receive a Packet
     * @param const Packet* Pointer to an object of type Packet that represents
     * the Packet that is in reception
     */

    /**
     * Method called when the Phy Layer finishes receiving a Packet
     * @param const Packet* Pointer to an object of type Packet that represents
     * the packet received
     */
    virtual void Phy2MacEndRx(Packet *p);

    /**
     * Static map for packet type information
     */

protected:
    int pktid;                  /**< Packet ID */
    int num_equations;          /**< Number of equations */
    double initial_timer;       /**< Initial timer */
    double receivedTimeStamp[4]; // Array to hold four timestamps

    /**< Type of the packet */
    enum UWDSYNC_A_PKT_TYPE {
        UWPOLLING_DATA_PKT = 1,
    };

    /**< Status of the timer */
    enum UWDSYNC_A_TIMER_STATUS {
        UWDSYNC_A_IDLE = 1,
        UWDSYNC_A_RUNNING,
    };

    /**
     * Timer handler for UwDSync_A
     */
    class UwDSync_A_Timer : public TimerHandler {
    public:
        UwDSync_A_Timer(UwDSync_A *m);
        virtual ~UwDSync_A_Timer();

        virtual void stop();
        virtual void schedule(double val);
        bool isIdle();
        bool isRunning();
        void resetCounter();
        void incrCounter();
        int getCounter();
        double getDuration();

    protected:
        double start_time;
        double left_duration;
        int counter;
        UwDSync_A *module;
        UWDSYNC_A_TIMER_STATUS timer_status;
    };

    /**
     * Back-off timer for UwDSync_A
     */
    class BackOffTimer : public UwDSync_A_Timer {
    public:
        BackOffTimer(UwDSync_A *m);
        virtual ~BackOffTimer();

    protected:
        virtual void expire(Event *e);
    };

    BackOffTimer timer; /**< Timer instance */

	static std::map<UwDSync_A::UWDSYNC_A_PKT_TYPE, std::string> pkt_type_info;

};

#endif // UWDSYNC_A_H
