#pragma once
#include <vector>
#include "mmtTableBase.h"
#include "mmtDescriptors.h"

namespace MmtTlv {

// MH-Application Information Table
class MhAit : public MmtTableBase {
public:
    bool unpack(Common::ReadStream& stream);

    class ApplicationIdentifier {
    public:
        bool unpack(Common::ReadStream& stream);
        uint8_t applicationControlCode;
        uint16_t applicationDescriptorLoopLength;
        MmtDescriptors descriptors;
    };

    uint16_t sectionSyntaxIndicator;
    uint16_t sectionLength;

    uint16_t applicationType;
    uint8_t versionNumber;
    bool currentNextIndicator;
    uint8_t sectionNumber;
    uint8_t lastSectionNumber;

    uint16_t commonDescriptorLength;
    MmtDescriptors descriptors;
    uint16_t applicationLoopLength;

    std::list<ApplicationIdentifier> applicationIdentifiers;
    uint32_t crc32;
};

}