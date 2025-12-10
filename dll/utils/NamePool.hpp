#pragma once
class FNameEntryHandle
{
public:

    uint32_t Block = 0;
    uint32_t Offset = 0;

    FNameEntryHandle(uint32_t block, uint32_t offset) : Block(block), Offset(offset) {};
    FNameEntryHandle(uint32_t id) : Block(id >> 16), Offset(id & 65535) {};
    operator uint32_t() const { return (Block << 16 | Offset); }
};

class FNameEntry
{
public:

    uint16_t bIsWide : 1;
    uint16_t LowercaseProbeHash : 5;
    uint16_t Len : 10;
    union
    {
        char AnsiName[1024];
        wchar_t	WideName[1024];
    };

    std::string String() {
        if (bIsWide) { return std::string(); }
        return { AnsiName, Len };
    }
};


class FNamePool {
public:
    BYTE Lock[8];
    uint32_t CurrentBlock;
    uint32_t CurrentByteCursor;
    BYTE* Blocks[8192];

    FNameEntry* GetEntry(FNameEntryHandle handle) const {
        return reinterpret_cast<FNameEntry*>(Blocks[handle.Block] + 2 * static_cast<uint64_t>(handle.Offset));
    }
};


namespace NamePool {
    inline FNamePool * NamePoolData = nullptr;

    inline void init() {
        NamePoolData = (FNamePool * ) (SDK::InSDKUtils::GetImageBase() + Offsets::GNames_());
        std::cout <<"NamePoolData: " <<  NamePoolData << std::endl;
    }

    inline std::string FNameGetName(int32_t Index) {
        return NamePoolData->GetEntry(Index)->String();
    }
}