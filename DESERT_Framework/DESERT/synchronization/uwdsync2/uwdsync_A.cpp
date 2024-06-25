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
 * @file   uwdsync_A.cpp
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Implementation of uwdsync_A class
 *
 */

#include "uwdsync2_cmn_hdr.h"
#include "uwdsync_A.h"
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
static class UwDSync_A_Class : public TclClass
{
public:
    /**
     * Constructor of the class
     */
    UwDSync_A_Class()
        : TclClass("Module/UW/DSYNC/A")
    {
    }

    /**
     * Creates the TCL object needed for the tcl language interpretation
     * @return Pointer to a TclObject
     */
    TclObject *
    create(int, const char *const *)
    {
        return (new UwDSync_A());  // Return a TclObject pointer
    }
} class_module_uwdsync_A;

int UwDSync_A::command(int argc, const char *const *argv) {
    Tcl &tcl = Tcl::instance();
    if (argc == 2) {
        if (strcasecmp(argv[1], "start") == 0) {
            // Logic to start the initial timer
            initialTimer();
            return TCL_OK;
        } else if (strcasecmp(argv[1], "stop") == 0) {
            // Logic to stop the timer
            timer.stop();
            return TCL_OK;
        }
    }
    return MMac::command(argc, argv);
}


int UwDSync_A::crLayCommand(ClMessage *m) {
    switch (m->type()) {
    default:
        return MMac::crLayCommand(m);
    }
}

UwDSync_A::UwDSync_A()
    : MMac(), num_equations(5), pktid(0), timer(this) 
{
}

UwDSync_A::~UwDSync_A()
{
}

std::map<UwDSync_A::UWDSYNC_A_PKT_TYPE, std::string> UwDSync_A::pkt_type_info;

double UwDSync_A::initialTimer()
{
    // Check if the timer is not already running
    if (!timer.isRunning()) {
        // Schedule the timer for 10 minutes (600 seconds)
        timer.schedule(600);
    }

    // When the timer expires, it will call the expire method
}

// Assuming you have defined the expire method to handle the transition to stateTxData
void UwDSync_A::BackOffTimer::expire(Event *e)
{
    if (module != NULL) {
        module->stateTxData();
    }
}

void UwDSync_A::stateTxData()
{
    Packet *p = Packet::alloc();
    initPkt(p);
    Mac2PhyStartTx(p);
}


void UwDSync_A::initPkt(Packet *p)
{
    p = Packet::alloc(); // Allocate memory for the packet
    hdr_mac *mach = HDR_MAC(p);
    hdr_cmn *ch = HDR_CMN(p);
    hdr_DATA *datah = HDR_DATA(p); // Access the hdr_DATA header

    ch->ptype() = PT_MAC;
    ch->size() = sizeof(hdr_mac) + sizeof(hdr_DATA) + sizeof(double); // Include hdr_DATA and t1 data size

    datah->ts_[0] = NOW; // Set ts_ to current time
    datah->ID() = 1; // Initialize packet ID, if needed

    mach->macSA() = addr;
    mach->macDA() = MAC_BROADCAST;
}

void UwDSync_A::Phy2MacEndRx(Packet *p)
{
    // Transition to the state where the packet is ready to be received
    stateRxTrigger(p);
}
void UwDSync_A::stateRxTrigger(Packet *p)
{
    hdr_DATA *datah = HDR_DATA(p);  // Correct macro and type for hdr_DATA
    int pktid = datah->ID();        // Use datah to access hdr_DATA members

    if (pktid == 3) {
        // Add t4 to the packet and send it again to Node B
        hdr_cmn *ch = HDR_CMN(p);   // Correct macro and type for common header
        ch->size() += sizeof(double); // Add space for t4

        datah->ts_[3] = NOW;          // Set ts_ to current time (t4)

        datah->ID() = 4;            // Update packet ID

        initialTimer();             // Reset the timer
        num_equations++;

        Mac2PhyStartTx(p);          // Send the packet

        stateIdle();                // Transition to idle state


    } else {
        Packet::free(p);            // Drop the packet
        initialTimer();             // Reset the timer
        stateIdle();                // Transition to idle state
    }
}

void UwDSync_A::Phy2MacEndTx(const Packet* p){}

void UwDSync_A::stateIdle(Packet *p)
{
    // Idle state logic
    // Assuming num_equations is a member variable of UwDSync_A
    if (p != nullptr) {
        hdr_DATA *datah = HDR_DATA(p); // Access hdr_DATA
        int pktid = datah->ID();       // Get packet ID

        if (pktid == 3) {
            // Received packet with ID 3
            stateRxTrigger(p);
        } else if (pktid == 4 && num_equations < 5) {
            initialTimer();
        } else {
            std::cerr << "Not a valid situation" << std::endl;
            Packet::free(p); // Free the packet if it's not valid
        }
    } else {
        // Handle idle state logic when no packet is provided
        if (pktid == 0) {
            // Initial state logic
            // Implement the initial state logic if required
        }
    }
}
