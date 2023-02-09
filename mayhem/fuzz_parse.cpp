#include <cstdlib>
#include <iostream>
#include <fuzzer/FuzzedDataProvider.h>

#include <highfive/H5File.hpp>
#include <highfive/H5Easy.hpp>


using namespace HighFive;
typedef typename boost::numeric::ublas::matrix<double> Matrix;

extern "C" __attribute__((unused)) int LLVMFuzzerTestOneInput(const uint8_t *fuzz_data, size_t size) {
    if (size < 1) {
        return -1;
    }
    FuzzedDataProvider fdp(fuzz_data, size);

    auto test_matrix = fdp.ConsumeBool();

    if (test_matrix) {
        File file("test", File::Truncate);
        auto size_x = static_cast<std::size_t>(fdp.ConsumeIntegralInRange<size_t>(1, 100));
        auto size_y = static_cast<std::size_t>(fdp.ConsumeIntegralInRange<size_t>(1, 100));

        Matrix matrix(size_x, size_y);
        Matrix result;

        for (std::size_t i = 0; i < size_x; ++i) {
            for (std::size_t j = 0; j < size_y; ++j) {
                matrix(i, j) = fdp.ConsumeFloatingPoint<double>();
            }
        }

        auto dataset = file.createDataSet<double>("data", DataSpace::From(matrix));
        dataset.read(result);
    } else {
        H5Easy::File file("test", H5Easy::File::Overwrite);
        H5Easy::dump(file, "data", fdp.ConsumeRemainingBytesAsString());

        auto contents = H5Easy::load<std::string>(file, "data");
    }
    return 0;
}
