#include <vector>
#include <cstring>

#include <pie/loader.h>
namespace pie {
    Loader::Loader(const std::filesystem::path& model, const std::filesystem::path& assets) {
        io = std::fstream(model, std::ios::binary | std::ios::in);
        if (!io.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
        if (!inferModelFormat())
            throw std::runtime_error("Failed to infer model format");
    }
    void Loader::read(ModelInfo& info) const {
        if (!infer.checked)
            throw std::runtime_error("");

        if (infer.model == Kaydara) {
            if (infer.extended)
                info.format = "Kaydara (FBX) Binary";
            else
                info.format = "Kaydara (FBX)";
        } else if (infer.model == WaveFront) {
            info.format = "WaveFront (OBJ)";
        }
    }

    bool Loader::inferModelFormat() {
        std::vector<char> header(sizeof(infer));
        io.readsome(&header[0], sizeof(infer));
        std::memcpy(&infer, &header[0], sizeof(infer));

        const std::string fbxHdr{"Kaydara FBX Binary  \x00"};
        if (!std::memcmp(&infer.fmtMax[0], &fbxHdr[0], fbxHdr.size())) {
            validateFbxModel();
        }
        if (infer.model == Kaydara)
            return infer.checked;

        while (io.getline(&header[0], static_cast<std::streamsize>(header.size()))) {

        }
        return infer.model;
    }

    void Loader::validateFbxModel() {
        std::vector<char> buffer(4);

        io.seekg(21, std::ios::beg);
        io.read(&buffer[0], buffer.size());

        u16 unknown,
            version;
        std::memcpy(&unknown, &buffer[0], sizeof(u16));
        std::memcpy(&version, &buffer[2], sizeof(u16));

        if (unknown == 0x001a && version) {
            infer.model = Kaydara;
            infer.version = version;
        }

        infer.checked = true;
    }
}
