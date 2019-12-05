#include "Factory.h"

#include "RecursiveBacktracker.h"
#include "OnlyWalls.h"

namespace Generator {

std::shared_ptr<Generator::Base> Create(EGeneratorType GeneratorType) {
	switch (GeneratorType) {
		case EGeneratorType::OnlyWalls:
			return std::make_shared<Generator::OnlyWalls>();
		case EGeneratorType::RecursiveBacktracker:
			return std::make_shared<Generator::RecursiveBacktracker>();
		default:
			return nullptr;
	}
}

}  // namespace Generator
