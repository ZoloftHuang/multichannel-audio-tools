/*
 * Copyright 2018 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AUDIO_DSP_RESAMPLER_H_
#define AUDIO_DSP_RESAMPLER_H_

#include <vector>

#include "glog/logging.h"

#include "audio/dsp/porting.h"  // auto-added.


namespace audio_dsp {

// Abstract resampler interface.
//
// Usage: Call Valid after construction or calling Reset() to check whether the
// Resampler successfully initialized with the provided parameters. Calling
// methods on an invalid Resampler produces undefined behavior. Call
// ProcessSamples zero or more times. Call Reset at any time to return the
// sampler to a pristine state. When all samples have been processed, call
// Flush. [Flush also calls Reset.]
template <typename ValueType>
class Resampler {
 public:
  Resampler() {}
  virtual ~Resampler() {}

  void Reset() {
    DCHECK(Valid());
    ResetImpl();
  }

  bool Valid() const {
    return ValidImpl();
  }

  // In-place processing is not supported.
  void ProcessSamples(const std::vector<ValueType>& input,
                      std::vector<ValueType>* output) {
    DCHECK(Valid());
    DCHECK(output != nullptr);
    DCHECK(output != &input) << "Cannot resample in place!";
    output->clear();
    ProcessSamplesImpl(input, output);
  }

  // Flush the output by adding enough zeros so that any previous
  // non-zeros are fully processed.
  void Flush(std::vector<ValueType>* output) {
    DCHECK(Valid());
    FlushImpl(output);
    Reset();
  }

 protected:
  virtual void ResetImpl() = 0;
  virtual bool ValidImpl() const = 0;
  virtual void ProcessSamplesImpl(const std::vector<ValueType>& input,
                                  std::vector<ValueType>* output) = 0;
  virtual void FlushImpl(std::vector<ValueType>* output) = 0;
};

}  // namespace audio_dsp

#endif  // AUDIO_DSP_RESAMPLER_H_
