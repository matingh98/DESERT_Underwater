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
 * @file   uwsync_REG.cpp
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Implementation of Uw Sync class for regular node
 *
 **/

#include <node_REG.h>
#include <sync_header.h>
#include "mac.h"
#include "mmac.h"
#include "mphy_pktheader.h"
#include <sstream>
#include <sys/time.h>
#include <iostream>
#include <random>

static class UwSyncREG_Class : public TclClass
{
public:
    UwSyncREG_Class() : TclClass("Module/UW/SYNC/REG") {}
    TclObject *create(int, const char *const *)
    {
        return (new UwSyncREG());
    }
} class_module_uwsync_reg;

UwSyncREG::UwSyncREG() : MMac()
{
    std::cout << "UwSyncREG constructor called" << std::endl;
}

UwSyncREG::~UwSyncREG() {}




int UwSyncREG::command(int argc, const char *const *argv) {
    Tcl &tcl = Tcl::instance();
   if (strcasecmp(argv[1], "get_timestamp") == 0) {
            std::stringstream ss;
            for (int i = 0; i < 4; i++) {
                ss << receivedTimeStamp[i] << " ";
            }
            tcl.resultf("%s", ss.str().c_str());
            return TCL_OK;
        }

    return MMac::command(argc, argv);
}


int UwSyncREG::crLayCommand(ClMessage *m) {
    switch (m->type()) {
    default:
        return MMac::crLayCommand(m);
    }
}


void UwSyncREG::StateRxPacket(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    if (pktid == 1)
    {
        std::cout << "Packet ID 1 received from node Reference" << std::endl;

        synch->ts_[1] = NOW;
        synch->ID() = 2;

        UwSyncREG_Timer *timer = new UwSyncREG_Timer(this, p);
        timer->BackoffTimer(); // Start the backoff timer
    }
    else
    {
        std::cout << "Not a valid situation" << std::endl;
    }
}


void UwSyncREG::TransmittingToNodeREF(Packet *p)
{

    hdr_SYNC *synch = HDR_SYNC(p);

    synch->ts_[2] = NOW;
    synch->ID() = 3;

    MMac::Mac2PhyStartTx(p);
    stateIdle(p);
}

void UwSyncREG::stateIdle(Packet *p)
{
    if (p != nullptr)
    {
        hdr_SYNC *synch = HDR_SYNC(p);
        int pktid = synch->ID();

        if (pktid == 1)
        {
            StateRxPacket(p);
        }
        else if (pktid == 4)
        {
            sendReceivedTimestamp(p);
        }
        else
        {
            std::cout << "Not a valid situation" << std::endl;
            Packet::free(p);
        }
    }
}

std::vector<double> UwSyncREG::sendReceivedTimestamp(Packet *p)
{
    std::cout << "Sending received timestamps" << std::endl;
    hdr_SYNC *synch = HDR_SYNC(p);
    for (int i = 0; i < 4; i++)
    {
        receivedTimeStamp[i] = synch->ts_[i];
    }
    return std::vector<double>(receivedTimeStamp, receivedTimeStamp + 4);
}


void UwSyncREG::recv(Packet *p){
    hdr_cmn *ch = HDR_CMN(p);
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    if (ch->direction()==hdr_cmn::UP){
    stateIdle(p);
    }

    if (ch->direction()==hdr_cmn::DOWN){
    stateIdle(p);

    }

}