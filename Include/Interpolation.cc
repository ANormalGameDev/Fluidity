#include "Fluidity.h"

// Null Interpolation
// --> End
uint Fluidity::Interpolation::Null(int Start, int End, float ElapsedTime) {
    return End;
}

// Linear Interpolation
// --> Assuming k (Elapsed Time) belongs to the closed interval [0.0, 1.0]
// --> Start + (End - Start) * k
uint Fluidity::Interpolation::Lerp(int Start, int End, float ElapsedTime) {
    float k = std::min(float(1), std::max(float(0), ElapsedTime));
    return Start + (End - Start) * k;
}

// Third-degree Sigmoid Interpolation
// --> Assuming k (Elapsed Time) belongs to the closed interval [0.0, 1.0]
// --> Start + (End - Start) * k * k * (3 - 2 * k)
uint Fluidity::Interpolation::SmoothStep(int Start, int End, float ElapsedTime) {
    float k = std::min(float(1), std::max(float(0), ElapsedTime));
    return Start + (End - Start) * k * k * (3 - 2 * k);
}

// Fifth-degree Sigmoid Interpolation
// --> Assuming k (Elapsed Time) belongs to the closed interval [0.0, 1.0]
// --> Start + (End - Start) * k * k * k * (k * (k * 6 - 15) + 10)
uint Fluidity::Interpolation::SmootherStep(int Start, int End, float ElapsedTime) {
    float k = std::min(float(1), std::max(float(0), ElapsedTime));
    return Start + (End - Start) * k * k * k * (k * (k * 6 - 15) + 10);
}