#include "../Include/Fluidity.h"
#include <iostream>

class TestWindow : public Fluidity::FluidWindow {
    private:
        int LoopCounter;
        int FrameCounter;

    protected:
        void Start() {
            LoopCounter = FrameCounter = 0;
            set_default_size(200, 200);
        }

        void OnFrameStarted() {
            FrameCounter++;
            if (FrameCounter == 65) SetTimeScale(-1);
        }

        void OnAnimationCompleted() {
            std::cout << GetCurrentAnimation()->GetName() << "\n";
            LoopCounter++;
            if (LoopCounter == 5) {
                GetCurrentAnimation()->SetLooping(false);
            }
        }
};

int main(int argc, char *argv[]) {
    auto App = Gtk::Application::create(argc, argv, "ex.angd.FluidityTest");

    TestWindow window;
    Fluidity::Animation animation("ExampleAnimation", 60);
    animation.AddKeyframe(0, 0, 1, Fluidity::Interpolation::Null)
            ->AddKeyframe(0, 0, 20, Fluidity::Interpolation::Null)
            ->AddKeyframe(1166, 568, 30, Fluidity::Interpolation::SmootherStep)
            ->AddKeyframe(1166, 0, 20, Fluidity::Interpolation::SmootherStep)
            ->SetLooping(true);
    window.PlayAnimation(animation);

    return App->run(window);
}