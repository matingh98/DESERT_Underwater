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
#include "uwdsync_cmn_hdr.h"
#include "mac.h"
#include "mmac.h"
#include "uwcbr-module.h"
#include "mphy_pktheader.h"
#include "rng.h"

#include <sstream>
#include <sys/time.h>
#include <iostream>
#include <random> // Include the random library

/**
 * Class that represents the binding with the tcl configuration script
 */

static class UwDSync_NODE_Class : public TclClass
{
public:
    /**
     * Constructor of the class
     */
    UwDSync_NODE_Class()
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
        return (new UwDSync_node());
    }
} class_module_uwdsync_node;

UwDSync_node::UwDSync_node()
    : MMac(), receivedTimeStamp(0.0)
{
}

UwDSync_node::~UwDSync_node()
{
}

int UwDSync_node::command(int argc, const char *const *argv)
{
    Tcl &tcl = Tcl::instance();

    if (argc == 2) { 
        if (strcasecmp(argv[1], "get_timestamp") == 0) {
            tcl.resultf("%f", receivedTimeStamp); 
            return TCL_OK;
        }
    }
    return MMac::command(argc, argv);
}

int UwDSync_node::crLayCommand(ClMessage *m)
{
    switch (m->type())
    {
    default:
        return MMac::crLayCommand(m);
    }
}

void UwDSync_node::Phy2MacStartRx(const Packet *p)
{
    // This function is intentionally left empty
}

void UwDSync_node::Phy2MacEndRx(Packet *p)
{
    // Extract DSYNC header from the received packet
    hdr_DSYNC *dysnc_hdr = HDR_DSYNC(p);

    double originalTimeStamp = dysnc_hdr->TIME_STAMP;
    int receivedUid = dysnc_hdr->DSYNC_uid_;


    // Calculate the received time incorporating Alpha and Beta
    receivedTimeStamp = 10;

    // Print the received information
    std::cout << "Original TIME_STAMP: " << originalTimeStamp << std::endl;
    std::cout << "Received DSYNC_uid_: " << receivedUid << std::endl;
    std::cout << "Received Time (with Alpha and Beta): " << receivedTimeStamp << std::endl;

    stateIdle();
}


void UwDSync_node::Mac2PhyStartTx(Packet *p)
{
    MMac::Mac2PhyStartTx(p);
}

void UwDSync_node::Phy2MacEndTx(const Packet *p)
{
    // This function is intentionally left empty
}

void UwDSync_node::stateIdle()
{
    // This function is intentionally left empty
}

