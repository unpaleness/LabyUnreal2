#pragma once

#include <memory>

#include "Base.h"
#include "Types.h"

namespace Generator {

std::shared_ptr<Generator::Base> Create(EGeneratorType GeneratorType);

}  // namespace Generator
