//
// Copyright (c) 2014 Regents of the SIGNET lab, University of Padova.
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
 * @file   uwtracker-packet.h
 * @author Filippo Campagnaro, Vincenzo Cimino
 * @version 1.0.0
 * 
 * \brief Provides both <i>UWROV</i> monitoring and control packets header description.
 * 
 * Provides both <i>UWROV</i> monitoring and control packets header description, in 
 * particular the header structure.
 */

#ifndef UWSCFTRACKER_HDR_H
#define UWSCFTRACKER_HDR_H

extern packet_t PT_UWSCFTRACK;

/**
 * <i>hdr_uwROV_ctr</i> describes <i>UWROV_ctr</i> packets for controlling the ROV.
 */
typedef struct hdr_uwSCFTracker {
    float timestamp_; /**<timestamp when the target was tracked*/
	bool mine_remove_;

    static int offset_; /**< Required by the PacketHeaderManager. */

    /**
     * Reference to the offset_ variable.
     */
    inline static int& offset() {
        return offset_;
    }
    /**
     * static method to access the packet header
     */
    inline static struct hdr_uwSCFTracker * access(const Packet * p) {
        return (struct hdr_uwSCFTracker*) p->access(offset_);
    }
    /**
     * Reference to the timestamp variable.
     */
    inline float& timestamp() {
        return timestamp_;
    }

    /**
     * Reference to the speed variable.
     */
    inline bool& mine_remove() {
        return mine_remove_;
    }
} hdr_uwSCFTracker;


#endif // UWROV_MODULE_H
