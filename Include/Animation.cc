#include "Fluidity.h"

// Fluidity::Keyframe
//

Fluidity::Keyframe::Keyframe(uint _X, uint _Y, uint _Duration, uint(*_InterpolationFunction)(int, int, float)) {
    X = _X;
    Y = _Y;
    Duration = _Duration;
    InterpolationFunction = _InterpolationFunction;
}


uint Fluidity::Keyframe::InterpolateXFrom(int Start, float ElapsedTime) {
    return (*InterpolationFunction)(Start, X, ElapsedTime);
}

uint Fluidity::Keyframe::InterpolateYFrom(int Start, float ElapsedTime) {
    return (*InterpolationFunction)(Start, Y, ElapsedTime);
}

uint Fluidity::Keyframe::InterpolateXTo(int End, float ElapsedTime) {
    return (*InterpolationFunction)(X, End, ElapsedTime);
}

uint Fluidity::Keyframe::InterpolateYTo(int End, float ElapsedTime) {
    return (*InterpolationFunction)(Y, End, ElapsedTime);
}

// Fluidity::Animation
//

Fluidity::Animation::Animation() { TickInterval = 0; }

Fluidity::Animation::Animation(uint16_t TickRate) { TickInterval = 1000/TickRate; }


uint Fluidity::Animation::GetTickInterval() {
    return TickInterval;
}

int Fluidity::Animation::CountKeyframes() {
    return Keyframes.size();
}


Fluidity::Animation* Fluidity::Animation::AddKeyframe(uint X, uint Y, uint Duration, uint(*InterpolationFunction)(int, int, float)) {
    if (Keyframes.size() == 50) std::__throw_length_error("Fluidity::Animation::AddKeyframe(uint)");
    Keyframes.push_back(Keyframe(X, Y, Duration, InterpolationFunction));
    return this;
}


Fluidity::Keyframe Fluidity::Animation::operator[](int i) {
    if (i < 0 && i >= Keyframes.size()) std::__throw_out_of_range("Fluidity::Animation::operator[](int)");
    return Keyframes[i];
}