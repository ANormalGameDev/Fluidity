#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <map>
#include <gtkmm-3.0/gtkmm.h>
#include "Math.h"

using namespace Fluidity::Math;

namespace Fluidity {
    template <typename T>
    class Keyframe {
        public:
            const int _At;
            T _Data;
            bool operator > (Keyframe<T> k) { return _At > k._At; }
            bool operator < (Keyframe<T> k) { return _At < k._At; }
            Keyframe<T>* Data(T data) { _Data = data;  return this; }
            Keyframe(uint At) : _At{At} {}
    };

    class DrawerObject {
        public:
            virtual void Draw(int Time) {}
    };

    template <typename T>
    class Animation : public DrawerObject {
        bool _Finished;
        int _Start, _End;
        void(*_OnDrawCallback)(T, Animation<T>*);
        std::vector<Keyframe<T>> _Keyframes;
        std::map<UnorderedPair, T(*)(T, T, float)> _Easings;
        std::string _Name;
        public:
            int End() { return _End; }
            int Start() { return _Start; }
            void Draw(int Time) {
                if (Time < _Start || _Finished) return;

                auto it = std::lower_bound(_Keyframes.begin(), _Keyframes.end(), Keyframe<T>(Time));
                if (it == _Keyframes.end()) { _Finished = true;  return; }

                Keyframe<T> LastFrame = *(it - 1), NextFrame = *it;
                float _Time = float(Time - LastFrame._At) / float(NextFrame._At - LastFrame._At);

                auto func = _Easings.find(UnorderedPair(it - _Keyframes.begin(), it - _Keyframes.begin() + 1));
                if (func == _Easings.end()) return;

                (*_OnDrawCallback)((*func).second(LastFrame._Data, NextFrame._Data, _Time), this);
            }
            Animation<T>* To(uint at) { _End = at; }
            Animation<T>* From(uint at) { _Start = at; }
            Animation<T>* Link(void(*OnDrawCallback)(T, Animation<T>*)) { _OnDrawCallback = OnDrawCallback;  return this; }
            Animation<T>* Easing(UnorderedPair Relation, T(*Func)(T, T, float)) {
                _Easings[Relation] = Func;
                return this;
            }
            Keyframe<T>* AddKeyframe(uint At) {
                _Keyframes.push_back(Keyframe<T>(At));
                return &_Keyframes[_Keyframes.size() - 1];
            }
            std::string Name() { return _Name; }
            Animation(std::string Name) { _Name = Name; }
    };

    template <typename T>
    Animation<T>* NewAnimation(std::string Name) {
        Animation<T>* _tmp = new Animation<T>(Name);
        return _tmp;
    }

    template <typename T>
    void DeleteAnimation(Animation<T>* Target) {
        if (Target == nullptr) return;
        // Deleting action here
    }

    class Timeline {
        std::vector<DrawerObject*> _Drawers;
        int _Length, _Time, _FPS;
        public:
            inline void Run() { Glib::signal_timeout().connect(sigc::mem_fun(this, &Timeline::Tick), 1000/_FPS); }
            bool Tick() {
                _Time++;
                for (DrawerObject* drawer : _Drawers) {
                    drawer->Draw(_Time);
                }
                return _Time < _Length;
            }
            template <typename T>
            Timeline* AddAnim(Animation<T>* Target) {
                if (Target == nullptr) return this;
                if (_Drawers.size() == 5) return this;
                if (Target->End() > _Length) return this;

                _Drawers.push_back(Target);
                return this;
            }
            Timeline* FPS(uint FramesPerSecond) { _FPS = FramesPerSecond;  return this; }
            Timeline(uint Length) { _Length = Length; _Time = _FPS = 0; }
    };

    Timeline* NewTimeline(uint Length) {
        Timeline* _tmp = new Timeline(Length);
        return _tmp;
    }
    void DeleteTimeline(Timeline* Target) {
        if (Target == nullptr) return;
        delete Target;
    }
}

#endif