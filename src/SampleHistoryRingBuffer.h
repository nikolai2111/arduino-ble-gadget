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
#ifndef _SAMPLE_HISTORY_RING_BUFFER_
#define _SAMPLE_HISTORY_RING_BUFFER_

#include "ByteArray.h"
#include "Config.h"
#include "Download.h"
#include "Sample.h"

const static size_t SAMPLE_HISTORY_RING_BUFFER_SIZE_BYTES = 30000;

// Logs Samples over time to be downloaded
class SampleHistoryRingBuffer
    : public ByteArray<SAMPLE_HISTORY_RING_BUFFER_SIZE_BYTES> {
  public:
    void addSample(const Sample& sample);
    void setSampleSize(size_t sampleSize);
    size_t sampleCapacity() const;
    bool isWrapped() const;
    int getSampleIndex() const;
    DownloadPacket buildDownloadPacket(const SampleConfig& config,
                                       int downloadSequenceIdx);

  private:
    void _writeSample(const Sample& sample);
    // downloadIndex
    // lastTimeStamp

    // indicates arrayPosistion to be written to next
    int _sampleIndex = 0;
    bool _bufferIsWrapped = false;
    size_t _sampleSize = 0;
};

#endif /* _SAMPLE_HISTORY_RING_BUFFER_ */