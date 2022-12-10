#include "Fluidity.h"
#include <iostream>

Fluidity::FluidWindow::FluidWindow() {
    KeyframeIndex = ElapsedTime = 0;
    TimeScale = 1;
    Start();
}


bool Fluidity::FluidWindow::OnAnimationTick() {
    if (TimeScale >= 0 ? (KeyframeIndex == CurrentAnimation.CountKeyframes() - 1) : (KeyframeIndex == 0)) {
        // Mark Current Animation as Completed
        ElapsedTime = KeyframeIndex = 0;
        CurrentAnimation = Animation();

        // Stop the ticks
        return false;
    }

    // Call the callback function when a frame is started
    OnFrameStarted();

    Keyframe NextFrame = CurrentAnimation[KeyframeIndex + (TimeScale == -1 ? -1 : 1)];
    Keyframe LastFrame = CurrentAnimation[KeyframeIndex];

    // Make Time Progress
    ElapsedTime += TimeScale == 0 ? 0 : float(1)/(TimeScale > 0 ? NextFrame : LastFrame).Duration;

    if (ElapsedTime > 1) {
        // Mark Current Keyframe as Completed
        KeyframeIndex += TimeScale;
        ElapsedTime = 0;
    }

    // Interpolate to the Next Frame
    LastFrame = CurrentAnimation[KeyframeIndex];
    if (TimeScale >= 0)
        move(NextFrame.InterpolateXFrom(LastFrame.X, ElapsedTime), NextFrame.InterpolateYFrom(LastFrame.Y, ElapsedTime));
    else
        move(LastFrame.InterpolateXTo(NextFrame.X, ElapsedTime), LastFrame.InterpolateYTo(NextFrame.Y, ElapsedTime));

    // Continue the ticks
    return true;
}


Fluidity::Animation Fluidity::FluidWindow::GetCurrentAnimation() {
    return CurrentAnimation;
}

size_t Fluidity::FluidWindow::GetCurrentKeyframeIndex() {
    return KeyframeIndex;
}

int Fluidity::FluidWindow::GetTimeScale() {
    return TimeScale;
}


void Fluidity::FluidWindow::PlayAnimation(Animation _Animation) {
    if (_Animation.CountKeyframes() == 0 || _Animation.GetTickInterval() == 0) return;
    CurrentAnimation = _Animation;
    
    Glib::signal_timeout().connect(sigc::mem_fun((*this), &Fluidity::FluidWindow::OnAnimationTick), _Animation.GetTickInterval());
}

void Fluidity::FluidWindow::SetTimeScale(int _TimeScale) {
    if (TimeScale < -1 || TimeScale > 1) std::__throw_length_error("Fluidity::FluidWindow::SetTimeScale(int)");
    // ORIGIN OF A KNOWN BUG
    if (TimeScale + _TimeScale == 0 || TimeScale + _TimeScale == -1) { KeyframeIndex += TimeScale; ElapsedTime = 1 - ElapsedTime; }
    //
    TimeScale = _TimeScale;
}