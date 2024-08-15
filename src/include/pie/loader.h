#pragma once
#include <filesystem>
#include <fstream>

#include <types.h>
namespace pie {
    struct ProbHeader {
        std::array<char, 20> fmtMax;
        ModelObjectType model;
        i32 version;

        bool extended;
        bool marked;
        bool checked;
    };

    class Loader {
        public:
        explicit Loader(const std::filesystem::path& model, const std::filesystem::path& assets = "./");

        void read(ModelInfo& info) const;
        void inferModelFormat();

        ProbHeader infer;
    private:
        void validateFbxModel();
        void validateObjModel();
        std::fstream io;
    };
}