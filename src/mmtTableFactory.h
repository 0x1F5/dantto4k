#pragma once
#include <memory>

namespace MmtTlv {
class MmtTableBase;
class MmtTableFactory {
public:
	static std::shared_ptr<MmtTableBase> create(uint8_t id);
	static bool isValidId(uint8_t id);
	
};

}