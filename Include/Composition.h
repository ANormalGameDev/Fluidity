#ifndef __COMPOSITION_H
#define __COMPOSITION_H

#include <string>
#include <vector>
#include <map>
#include <gtkmm-3.0/gtkmm.h>
#include "Math.h"

using namespace Fluidity::Math;

namespace Fluidity {
    class Keyframe {
        int _At, _X;
        friend class Animation;
        public:
            Keyframe(int At);
            
            bool operator > (Keyframe k);
            bool operator < (Keyframe k);
            Keyframe* X(uint Value);
    };

    class Animation {
        bool _Finished;
        int _StartAt, _EndAt;
        Gtk::Widget* _Target;
        std::vector<Keyframe> _Keyframes;
        std::map<UnorderedPair, int(*)(int, int, float)> _Easings;
        std::string _Name;
        public:
            static void Delete(Animation* Target);
            static Animation* Create(std::string Name);

            int End();
            int Start();
            void Draw(int Time);
            Animation* From(uint StartAt);
            Animation* To(uint EndAt);
            Animation* Link(Gtk::Widget* Target);
            Animation* Easing(UnorderedPair Relation, int(*Func)(int, int, float));
            Keyframe* AddKeyframe(uint At);
    };

    class Timeline {
        std::vector<Animation*> Animations;
        int _Length, _Time, _FPS;
        public:
            static void Delete(Timeline* Target);
            static Timeline* Create(uint Length);
            
            void Run();
            bool Tick();
            Timeline* AddAnim(Animation* Target);
            Timeline* FPS(uint FramesPerSecond);
    };
}

#endif