//
// Copyright (c) 2019 Regents of the SIGNET lab, University of Padova.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions wSyncREFand the following disclaimer.
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
    : MMac(), pktid(10), start_time(0), stop_time(1001)
{
    std::cout << "UwDSync_A constructor called" << std::endl;
    bind("start_time_", (double *)&start_time);
    bind("stop_time_", (double *)&stop_time);
}

UwSyncREF::~UwSyncREF(){};


int UwSyncREF::crLayCommand(ClMessage *m) {
    switch (m->type()) {
    default:
        return MMac::crLayCommand(m);
    }
}

void UwSyncREF::initPkt()
{

    Packet *p = Packet::alloc();

    hdr_cmn *ch = HDR_CMN(p);
    hdr_SYNC *synch = HDR_SYNC(p);

    pktid = 0;
    stateIdle(p);
}

void UwSyncREF::RxPacket(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    synch->ts_[3] = NOW;
    synch->ID() = 4;

    Transmitting(p);
    stateIdle(p);
}

void UwSyncREF::stateIdle(Packet *p)
{

    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    if (pktid == 0)
    {
        TransmittingToNodeREG(p);
    }

    if (pktid == 3)
    {

        RxPacket(p);
    }
    else if (pktid == 4)
    {
        TransmittingToNodeREG(p);
    }
    else
    {
        std::cout << "Not a valid situation" << std::endl;
        Packet::free(p); // Free the packet if it's not valid
    }
}
void UwSyncREF::TransmittingToNodeREG(Packet *p) 
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    if (pktid == 0)
    {
        synch->ts_[0] = NOW; // Set ts_ to current time
        synch->ID() = 1;
        Transmitting(p);
    }
    else if (pktid == 4) // Changed to else if to avoid redundant checks
    {
        synch->ts_[3] = NOW; // Set ts_ to current time
        synch->ID() = 4;

        Transmitting(p);
    }
}


void UwSyncREF::Transmitting(Packet *p) {
    MMac::Mac2PhyStartTx(p);
}

void UwSyncREF::recv(Packet *p)
{
    hdr_cmn *ch = HDR_CMN(p);
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    if (ch->direction() == hdr_cmn::UP)
    {
        stateIdle(p);
    }
    if (ch->direction() == hdr_cmn::DOWN)
    {
        stateIdle(p);
    }
}
