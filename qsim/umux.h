// Copyright 2019 Google LLC. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef UMUX_H_
#define UMUX_H_

#ifdef __AVX512F__
# include "unitary_calculator_avx512.h"
  namespace qsim {
  namespace unitary {
    template <typename For>
    using UnitaryCalculator = UnitaryCalculatorAVX512<For>;
  }
  }
#elif __AVX2__
# include "unitary_calculator_avx.h"
  namespace qsim {
  namespace unitary {
    template <typename For>
    using UnitaryCalculator = UnitaryCalculatorAVX<For>;
  }
  }
#elif __SSE4_1__
# include "unitary_calculator_sse.h"
  namespace qsim {
  namespace unitary {
    template <typename For>
    using UnitaryCalculator = UnitaryCalculatorSSE<For>;
  }
  }
#else
# include "unitary_calculator_basic.h"
  namespace qsim {
  namespace unitary {
    template <typename For>
    using UnitaryCalculator = UnitaryCalculatorBasic<For>;
  }
  }
#endif

#endif  // UMUX_H_
