#include "../Include/Fluidity.h"
#include <iostream>

class TestWindow : public Fluidity::FluidWindow {
    private:
        int FrameCounter;

    protected:
        void Start() {
            FrameCounter = 0;
            set_default_size(200, 200);
        }

        void OnFrameStarted() {
            FrameCounter++;
            if (FrameCounter == 60) {
                std::cout << "reverse!\n";
                SetTimeScale(-1);
            }
            if (FrameCounter == 85) {
                std::cout << "freeze!\n";
                SetTimeScale(0);
            }
            if (FrameCounter == 115) {
                std::cout << "reverse!\n";
                SetTimeScale(1);
            }
        }
};

int main(int argc, char *argv[]) {
    auto App = Gtk::Application::create(argc, argv, "ex.angd.FluidityTest");

    TestWindow window;
    Fluidity::Animation animation(60);
    animation.AddKeyframe(0, 0, 1, Fluidity::Interpolation::Null)
            ->AddKeyframe(0, 0, 20, Fluidity::Interpolation::Null)
            ->AddKeyframe(1166, 568, 30, Fluidity::Interpolation::SmootherStep)
            ->AddKeyframe(1166, 0, 20, Fluidity::Interpolation::SmootherStep);
    window.PlayAnimation(animation);

    return App->run(window);
}