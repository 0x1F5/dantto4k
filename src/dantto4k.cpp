﻿#include <iostream>
#include "stream.h"
#include "MmtTlvDemuxer.h"
#include "remuxerHandler.h"
#include "bonTuner.h"
#include "config.h"
#include "dantto4k.h"

MmtTlv::MmtTlvDemuxer demuxer;
std::vector<uint8_t> output;
RemuxerHandler handler(demuxer, output);
CBonTuner bonTuner;

#ifdef _WIN32
extern "C" __declspec(dllexport) IBonDriver* CreateBonDriver()
{
    return &bonTuner;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
    {
        try {
            std::string path = getConfigFilePath(hModule);
            Config config = loadConfig(path);

            demuxer.init();
            demuxer.setDemuxerHandler(handler);

            bonTuner.init(config);
        }
        catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }

    return true;
}
#endif

namespace {
    size_t getLeftBytes(std::ifstream& file) {
        std::streampos currentPos = file.tellg();

        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();

        file.seekg(currentPos);

        return static_cast<size_t>(fileSize - currentPos);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "dantto4k.exe <input.mmts> <output.ts>" << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    demuxer.init();
    demuxer.setDemuxerHandler(handler);

    std::string inputPath, outputPath;
    inputPath = argv[1];
    outputPath = argv[2];

    std::ifstream inputFs(inputPath, std::ios::binary);
    if (!inputFs) {
        std::cerr << "Unable to open input file: " << inputPath << std::endl;
        return 1;
    }

    std::ofstream outputFs(outputPath, std::ios::binary);
    if (!inputFs) {
        std::cerr << "Unable to output input file: " << inputPath << std::endl;
        return 1;
    }

    const size_t chunkSize = 1024 * 1024 * 20;
    std::vector<uint8_t> buffer;

    while (!inputFs.eof()) {
        if (buffer.size() < chunkSize) {
            size_t readSize = std::min(chunkSize, getLeftBytes(inputFs));
            if (readSize == 0) {
                break;
            }

            buffer.resize(buffer.size() + readSize);
            inputFs.read(reinterpret_cast<char*>(buffer.data() + buffer.size() - readSize), readSize);
        }

        
		MmtTlv::Common::ReadStream stream(buffer);
		while (!stream.isEof()) {
			int pos = stream.getCur();
			int n = demuxer.processPacket(stream);

			// not valid tlv
			if (n == -2) {
				continue;
			}

			// not enough buffer for tlv payload
			if (n == -1) {
				stream.setCur(pos);
				break;
			}
		}

		buffer.erase(buffer.begin(), buffer.begin() + (buffer.size() - stream.leftBytes()));

        outputFs.write(reinterpret_cast<const char*>(output.data()), output.size());
        output.clear();
    }

    inputFs.close();
    outputFs.close();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cerr << "Elapsed time: " << elapsed_seconds.count() << " seconds\n";
    return 0;
}
