/*
 * Copyright (c) 2022, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SAMPLES_H_
#define _SAMPLES_H_

#include <Arduino.h>
#include <map>
#include <vector>

const static size_t SAMPLE_SIZE_BYTES = 12;
const static size_t ADVERTISEMENT_HEADER_SIZE_BYTES = 6;
const static size_t SAMPLE_HISTORY_RING_BUFFER_SIZE_BYTES = 30000;
const static size_t DOWNLOAD_PACKET_SIZE_BYTES = 20;

// Must explicitly instantiate template in Samples.cpp before usage
template <size_t SIZE> class ByteArray {
  public:
    std::string getDataString();

  protected:
    void _writeByte(uint8_t byte, size_t position);
    void _write16BitLittleEndian(uint16_t value, size_t position);
    void _write16BitBigEndian(uint16_t value, size_t position);
    std::array<uint8_t, SIZE> _data = {};
};

// Holds sensor values following the set SampleConfig
class Sample: public ByteArray<SAMPLE_SIZE_BYTES> {
  public:
    void writeValue(uint16_t value, size_t position);
};

class AdvertisementHeader: public ByteArray<ADVERTISEMENT_HEADER_SIZE_BYTES> {
  public:
    void writeCompanyId(uint16_t companyID);
    void writeSensirionAdvertisementType(uint8_t advType);
    void writeSampleType(uint8_t sampleType);
    void writeDeviceId(uint16_t deviceID);
};

// Logs Samples over time to be downloaded
class SampleHistoryRingBuffer
    : public ByteArray<SAMPLE_HISTORY_RING_BUFFER_SIZE_BYTES> {
    // add logic for ring buffering, includeing index, iteration
};

class DownloadHeader: public ByteArray<DOWNLOAD_PACKET_SIZE_BYTES> {
    // add approptiate write methods (see Bjoerns implementation)
};

class DownloadPacket: public ByteArray<DOWNLOAD_PACKET_SIZE_BYTES> {
    // add methods to add as many samples from SampleHistoryRingBuffer as will
    // fit
};

#endif /* _SAMPLES_H_ */