#include "Composition.h"
#include <iostream>

namespace Fluidity {
    Keyframe::Keyframe(int At) { _At = At; }

    bool Keyframe::operator > (Keyframe k) { return _At > k._At; }

    bool Keyframe::operator < (Keyframe k) { return _At < k._At; }

    Keyframe* Keyframe::X(uint Value) { _X = Value;  return this; }

    //
    //
    //
    //

    void Animation::Delete(Animation* Target) {
        if (Target == nullptr) return;
        delete Target;
    }

    Animation* Animation::Create(std::string Name) {
        Animation* _tmp = new Animation();
        _tmp->_Name = Name;
        return _tmp;
    }

    int Animation::Start() { return _StartAt; }

    int Animation::End() { return _EndAt; }

    void Animation::Draw(int Time) {
        if (Time < _StartAt || _Finished) return;

        auto it = std::lower_bound(_Keyframes.begin(), _Keyframes.end(), Keyframe(Time));
        if (it == _Keyframes.end()) { _Finished = true;  return; }

        Keyframe LastFrame = (*(it - 1)), NextFrame = (*it);
        float _Time = float(Time - LastFrame._At) / float(NextFrame._At - LastFrame._At);
        
        auto func = _Easings.find(UnorderedPair(it - _Keyframes.begin(), it - _Keyframes.begin() + 1));
        if (func == _Easings.end()) return;

        // Perform your interpolation here
        std::cout << (*func).second(LastFrame._X, NextFrame._X, _Time) << " " << Time << " " << _Name << "\n";
    }

    Animation* Animation::From(uint StartAt) { _StartAt = StartAt;  return this; }

    Animation* Animation::To(uint EndAt) { _EndAt = EndAt;  return this; }

    Animation* Animation::Link(Gtk::Widget* Target) { _Target = Target;  return this; }

    Animation* Animation::Easing(UnorderedPair Relation, int(*Func)(int, int, float)) {
        _Easings[Relation] = Func;
        return this;
    }

    Keyframe* Animation::AddKeyframe(uint At) {
        _Keyframes.push_back(Keyframe(At));
        return &_Keyframes[_Keyframes.size() - 1];
    }

    //
    //
    //
    //

    void Timeline::Delete(Timeline* Target) {
        if (Target == nullptr) return;
        for (Animation* anim : Target->Animations) {
            Animation::Delete(anim);
        }
        delete Target;
    }

    Timeline* Timeline::Create(uint Length) {
        Timeline* _tmp = new Timeline();
        _tmp->_Length = Length;
        return _tmp;
    }

    void Timeline::Run() { Glib::signal_timeout().connect(sigc::mem_fun((*this), &Timeline::Tick), 1000/_FPS); }

    bool Timeline::Tick() {
        _Time++;
        for (Animation* anim : Animations) {
            anim->Draw(_Time);
        }
        return _Time < _Length;
    }

    Timeline* Timeline::AddAnim(Animation* Target) {
        if (Animations.size() == 5) return this;
        if (Target->End() > _Length) return this;

        Animations.push_back(Target);
        return this;
    }

    Timeline* Timeline::FPS(uint FramesPerSecond) { _FPS = FramesPerSecond; return this; }
}