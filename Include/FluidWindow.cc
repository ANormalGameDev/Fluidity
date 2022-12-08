#include "Fluidity.h"

Fluidity::FluidWindow::FluidWindow() {
    KeyframeIndex = 0;
    ElapsedTime = 0;
}

bool Fluidity::FluidWindow::OnAnimationTick() {
    Keyframe CurrentFrame = CurrentAnimation[KeyframeIndex];
    ElapsedTime += float(1)/CurrentFrame.Duration;
    if (ElapsedTime > 1) {
        // Mark Current Keyframe as Completed
        KeyframeIndex++; ElapsedTime = 0;
    }
    if (KeyframeIndex == CurrentAnimation.CountKeyframes()) {
        // Mark Current Animation as Completed
        ElapsedTime = KeyframeIndex = 0; CurrentAnimation = Animation();
        return false;
    }
    if (KeyframeIndex > 0) {
        // Interpolate to the Current Keyframe
        Keyframe PreviousFrame = CurrentAnimation[KeyframeIndex - 1];
        move(CurrentFrame.InterpolateX(PreviousFrame.X, ElapsedTime), CurrentFrame.InterpolateY(PreviousFrame.Y, ElapsedTime));
    } else { move(CurrentFrame.X, CurrentFrame.Y); }
    return true;
}

void Fluidity::FluidWindow::PlayAnimation(Animation _Animation) {
    if (_Animation.CountKeyframes() == 0 || _Animation.GetTickInterval() == 0) return;
    CurrentAnimation = _Animation;
    Glib::signal_timeout().connect(sigc::mem_fun((*this), &Fluidity::FluidWindow::OnAnimationTick), _Animation.GetTickInterval());
}