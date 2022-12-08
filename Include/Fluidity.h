#ifndef _FLUIDITY_INCLUDED
#define _FLUIDITY_INCLUDED

#include <gtkmm-3.0/gtkmm.h>
#include <vector>

namespace Fluidity {
    // Animation.cc
    //

    struct Keyframe {
        uint X, Y;
        uint Duration;
        uint(*InterpolationFunction)(int, int, float);
        Keyframe(uint _X, uint _Y, uint _Duration, uint(*InterpolationFunction)(int, int, float));
        uint InterpolateX(int Start, float ElapsedTime);
        uint InterpolateY(int Start, float ElapsedTime);
    };

    class Animation {
        private:
            std::vector<Keyframe> Keyframes;
            uint TickInterval;
        public:
            Animation() { TickInterval = 0; }
            Animation(uint16_t TickRate);
            Animation* AddKeyframe(uint X, uint Y, uint Duration, uint(*InterpolationFunction)(int, int, float));
            int CountKeyframes();
            uint GetTickInterval();
            Keyframe operator[](int i);
    };

    // Interpolation.cc
    //

    class Interpolation {
        public:
            static uint Null(int Start, int End, float ElapsedTime);
            static uint Lerp(int Start, int End, float ElapsedTime);
            static uint SmoothStep(int Start, int End, float ElapsedTime);
            static uint SmootherStep(int Start, int End, float ElapsedTime);
    };

    // FluidWindow.cc
    //

    class FluidWindow : public Gtk::Window {
        private:
            Animation CurrentAnimation;
            size_t KeyframeIndex;
            float ElapsedTime;
            bool OnAnimationTick();
        public:
            FluidWindow();
            void PlayAnimation(Animation _Animation);
    };
}

#endif