#include "accessControlDescriptor.h"

namespace MmtTlv {

bool AccessControlDescriptor::unpack(Common::ReadStream& stream)
{
    try {
        if (!MmtDescriptorTemplate::unpack(stream)) {
            return false;
        }

        Common::ReadStream nstream(stream, descriptorLength);

        caSystemId = nstream.getBe16U();
        if (!locationInfo.unpack(nstream)) {
            return false;
        }

        privateData.resize(nstream.leftBytes());
        nstream.read(privateData.data(), nstream.leftBytes());

        stream.skip(descriptorLength);
    }
    catch (const std::out_of_range&) {
        return false;
    }

    return true;
}

}