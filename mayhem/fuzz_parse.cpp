#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include <fuzzer/FuzzedDataProvider.h>

#include <highfive/H5Easy.hpp>
#include <highfive/H5File.hpp>

using namespace HighFive;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* fuzz_data, size_t size) {
    if (size < 1) {
        return -1;
    }
    FuzzedDataProvider fdp(fuzz_data, size);

    auto test_matrix = fdp.ConsumeBool();

    if (test_matrix) {
        File file("/tmp/highfive_fuzz.h5", File::Truncate);
        auto size_x = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        auto size_y = fdp.ConsumeIntegralInRange<size_t>(1, 100);

        std::vector<std::vector<double>> matrix(size_x, std::vector<double>(size_y));
        std::vector<std::vector<double>> result;

        for (std::size_t i = 0; i < size_x; ++i) {
            for (std::size_t j = 0; j < size_y; ++j) {
                matrix[i][j] = fdp.ConsumeFloatingPoint<double>();
            }
        }

        auto dataset = file.createDataSet<double>("data", DataSpace::From(matrix));
        dataset.write(matrix);
        dataset.read(result);
    } else {
        H5Easy::File file("/tmp/highfive_fuzz.h5", H5Easy::File::Overwrite);
        H5Easy::dump(file, "data", fdp.ConsumeRemainingBytesAsString());

        auto contents = H5Easy::load<std::string>(file, "data");
    }
    return 0;
}
