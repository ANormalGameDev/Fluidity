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

        uint InterpolateXFrom(int Start, float ElapsedTime);
        uint InterpolateYFrom(int Start, float ElapsedTime);
        uint InterpolateXTo(int End, float ElapsedTime);
        uint InterpolateYTo(int End, float ElapsedTime);
    };

    class Animation {
        private:
            uint TickInterval;
            std::vector<Keyframe> Keyframes;

        public:
            Animation();
            Animation(uint16_t TickRate);

            uint GetTickInterval();
            int CountKeyframes();

            Animation* AddKeyframe(uint X, uint Y, uint Duration, uint(*InterpolationFunction)(int, int, float));

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
            int TimeScale;

            bool OnAnimationTick();

        protected:
            Animation GetCurrentAnimation();
            size_t GetCurrentKeyframeIndex();

            virtual void Start() {};
            virtual void OnFrameStarted() {};

        public:
            FluidWindow();

            int GetTimeScale();

            void PlayAnimation(Animation _Animation);
            void SetTimeScale(int _TimeScale);
    };
}

#endif