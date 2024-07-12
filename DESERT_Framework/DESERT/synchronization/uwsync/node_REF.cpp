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
 * @file   uwsync_ref.cpp
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Implementation of uwsync_ref class
 *
 */

#include <node_REF.h>
#include <sync_header.h>
#include "mac.h"
#include "mmac.h"
#include "mphy_pktheader.h"
#include <sstream>
#include <sys/time.h>
#include <iostream>
#include <random>

static class UwSyncREF_Class : public TclClass
{
public:
    UwSyncREF_Class() : TclClass("Module/UW/SYNC/REF") {}
    TclObject *create(int, const char *const *)
    {
        return (new UwSyncREF());
    }
} class_module_uwsync_ref;

UwSyncREF::UwSyncREF()
    : MMac()
    , pktid(0)
    ,timer(this)  
{
    std::cout << "UwDSync_A constructor called" << std::endl;
    bind("start_time_", (double *)&start_time);
    bind("stop_time_", (double *)&stop_time);
}



UwSyncREF::~UwSyncREF(){};



// start Methode
void UwSyncREF::start() {
    std::cout << "UwSyncREF start method called" << std::endl;
    timer.initialTimer();
}

//stop Methode
void UwSyncREF::stop() {
    std::cout << "UwSyncREF stop method called" << std::endl;
    timer.stop();
}


void UwSyncREF_Timer::initialTimer() {
    // Check if the timer is not already running
    if (*start_time == 0) {
        // Schedule the timer for 10 minutes (600 seconds)
        schedule(*stop_time);
    } else {
        std::cout << "Something wrong happened" << std::endl;
    }
}
void UwSyncREF_Timer::expire(Event *e)
{
    if (module != nullptr) {  // Check if module is not null
        module->stateTxData();  // Call the stateTxData method of the module
    }
}

void UwSyncREF::initPkt(Packet *p)
{
    hdr_cmn *ch = HDR_CMN(p);
    hdr_SYNC *synch = HDR_SYNC(p);


    synch->ts_[0] = NOW; // Set ts_ to current time
    synch->ID() = 1;     // Initialize packet ID, if needed
    MMac::Mac2PhyStartTx(p);

}

void UwSyncREF::RxPacket(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    synch->ts_[3] = NOW;
    synch->ID() = 4;
    MMac::Mac2PhyStartTx(p);
    stateIdle(p);
}

void UwSyncREF::stateIdle(Packet *p)
{

    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    if (pktid == 3)
    {

        RxPacket(p);
    }
    else if (pktid == 4)
    {
    }
    else
    {
        std::cout << "Not a valid situation" << std::endl;
        Packet::free(p); // Free the packet if it's not valid
    }
}

void UwSyncREF::recv(Packet *p){
    hdr_cmn *ch = HDR_CMN(p);
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    if (ch->direction()==hdr_cmn::UP){
        stateIdle(p);
    }
    if (ch->direction()==hdr_cmn::DOWN){

    }

}