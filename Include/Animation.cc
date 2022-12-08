#include "Fluidity.h"

// Fluidity::Keyframe
//

Fluidity::Keyframe::Keyframe(uint _X, uint _Y, uint _Duration, uint(*_InterpolationFunction)(int, int, float)) {
    X = _X;
    Y = _Y;
    Duration = _Duration;
    InterpolationFunction = _InterpolationFunction;
}

uint Fluidity::Keyframe::InterpolateX(int Start, float ElapsedTime) {
    return (*InterpolationFunction)(Start, X, ElapsedTime);
}

uint Fluidity::Keyframe::InterpolateY(int Start, float ElapsedTime) {
    return (*InterpolationFunction)(Start, Y, ElapsedTime);
}

// Fluidity::Animation
//

Fluidity::Animation::Animation(uint16_t TickRate) {
    TickInterval = 1000/TickRate;
}

Fluidity::Animation* Fluidity::Animation::AddKeyframe(uint X, uint Y, uint Duration, uint(*InterpolationFunction)(int, int, float)) {
    if (Keyframes.size() <= 50) { Keyframes.push_back(Keyframe(X, Y, Duration, InterpolationFunction)); return this; }
    else std::__throw_length_error("Fluidity::Animation::AddKeyframe(uint)");
}

int Fluidity::Animation::CountKeyframes() {
    return Keyframes.size();
}

uint Fluidity::Animation::GetTickInterval() {
    return TickInterval;
}

Fluidity::Keyframe Fluidity::Animation::operator[](int i) {
    if (i >= 0 && i < Keyframes.size()) return Keyframes[i];
    else std::__throw_out_of_range("Fluidity::Animation::operator[](int)");
}