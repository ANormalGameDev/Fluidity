#include "Fluidity.h"

Fluidity::FluidWindow::FluidWindow() {
    TimeFrozen = KeyframeIndex = ElapsedTime = 0;
    TimeScale = 1;
    Start();
}


bool Fluidity::FluidWindow::OnAnimationTick() {
    // Call the callback function when a frame is started
    OnFrameStarted();

    if (TimeFrozen == true) return true;

    if (TimeScale > 0 ? (KeyframeIndex == CurrentAnimation.CountKeyframes() - 1) : (KeyframeIndex == 0)) {
        // Call the callback function when the current animation is completed
        OnAnimationCompleted();

        // Mark Current Animation as Completed
        ElapsedTime = 0;
        KeyframeIndex = (CurrentAnimation.CountKeyframes() - 1) * (TimeScale < 0);
        
        // Continue the ticks if the current animation is loop
        if (CurrentAnimation.GetLooping()) return true;

        // Reset the current animation and stop the ticks if it is not looping
        CurrentAnimation = Animation();
        return false;
    }

    Keyframe NextFrame = CurrentAnimation[KeyframeIndex + (TimeScale)];
    Keyframe LastFrame = CurrentAnimation[KeyframeIndex];

    // Make Time Progress
    ElapsedTime += float(1)/(TimeScale > 0 ? NextFrame : LastFrame).Duration;

    if (ElapsedTime > 1) {
        // Mark Current Keyframe as Completed
        KeyframeIndex += TimeScale;
        ElapsedTime = 0;
    }

    // Interpolate to the Next Frame
    LastFrame = CurrentAnimation[KeyframeIndex];
    if (TimeScale > 0)
        move(NextFrame.InterpolateXFrom(LastFrame.X, ElapsedTime), NextFrame.InterpolateYFrom(LastFrame.Y, ElapsedTime));
    else
        move(LastFrame.InterpolateXTo(NextFrame.X, ElapsedTime), LastFrame.InterpolateYTo(NextFrame.Y, ElapsedTime));

    // Continue the ticks
    return true;
}


Fluidity::Animation* Fluidity::FluidWindow::GetCurrentAnimation() {
    return &CurrentAnimation;
}

size_t Fluidity::FluidWindow::GetCurrentKeyframeIndex() {
    return KeyframeIndex;
}

int Fluidity::FluidWindow::GetTimeScale() {
    return (TimeFrozen ? 0 : TimeScale);
}


void Fluidity::FluidWindow::PlayAnimation(Animation _Animation) {
    if (_Animation.CountKeyframes() == 0 || _Animation.GetTickInterval() == 0) return;
    CurrentAnimation = _Animation;
    
    Glib::signal_timeout().connect(sigc::mem_fun((*this), &Fluidity::FluidWindow::OnAnimationTick), _Animation.GetTickInterval());
}

void Fluidity::FluidWindow::SetTimeScale(int _TimeScale) {
    if (_TimeScale < -1 || _TimeScale > 1) std::__throw_length_error("Fluidity::FluidWindow::SetTimeScale(int)");
    TimeFrozen = _TimeScale == 0;
    if (TimeScale + _TimeScale == 0) {
        // Perform necessary changes when the sign of TimeScale is changed
        KeyframeIndex += TimeScale;
        TimeScale = _TimeScale;
        ElapsedTime = 1 - ElapsedTime;
    }
}