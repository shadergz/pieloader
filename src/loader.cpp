#include <vector>
#include <cstring>
#include <regex>

#include <pie/loader.h>
namespace pie {
    Loader::Loader(const std::filesystem::path& model, const std::filesystem::path& assets) {
        io = std::fstream(model, std::ios::binary | std::ios::in);
        if (!io.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        inferModelFormat();

        if (!infer.checked)
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

    void Loader::inferModelFormat() {
        std::vector<char> header(120);
        io.read(&header[0], sizeof(infer));
        io.seekg(0, std::ios::beg);
        std::memcpy(&infer, &header[0], sizeof(infer));

        const std::string fbxHdr{"Kaydara FBX Binary  \x00"};
        if (!std::memcmp(&infer.fmtMax[0], &fbxHdr[0], fbxHdr.size())) {
            validateFbxModel();
            return;
        }
        std::vector<std::streamsize> rollback;
        const std::string_view line{&header[0], header.size()};
        do {
            rollback.push_back(io.tellg());
            io.getline(&header[0], static_cast<std::streamsize>(header.size()));
        } while (io.gcount() && line.starts_with("#"));

        if (line.contains("mtllib") || line.contains("o ")) {
            io.seekg(rollback.back());
            validateObjModel();
        }
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

    void Loader::validateObjModel() {
        std::vector<char> buffer(100);

        // Contains at least one MTL file
        bool hasMtl{};
        // The two below are mandatory for any model
        bool hasModel{};
        bool hasVertices{};

        const std::regex mtlRegex("^mtllib (\\S+)?\\.mtl$");
        const std::regex oRegex("^o (\\S+)?$");
        const std::regex verticesRegex("^v\\s-?\\d+(\\.\\d+)?\\s-?\\d+(\\.\\d+)?\\s-?\\d+(\\.\\d+)?$");

        const auto isChecked = [&] {
            return hasMtl && hasModel && hasVertices;
        };

        do {
            io.getline(&buffer[0], buffer.size());
            const std::string line{&buffer[0]};

            if (std::regex_match(line, mtlRegex))
                hasMtl = true;
            else if (std::regex_match(line, oRegex))
                hasModel = true;
            else if (std::regex_match(line, verticesRegex))
                hasVertices = true;

        } while (io.gcount() && !isChecked());

        if (io.gcount())
            infer.model = WaveFront;
        infer.checked = isChecked();
    }
}
