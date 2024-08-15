#include <print>

#include <pie/loader.h>
pie::i32 main(pie::i32 argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    const std::filesystem::path objFile{argv[1]};
    std::filesystem::path assetsPath{"./"};
    if (argc > 2)
        assetsPath = argv[2];

    auto reader{pie::Loader(objFile, assetsPath)};
    pie::ModelInfo info;
    reader.read(info);

    std::print("Provided model format: {}", info.format);

    return {};
}
