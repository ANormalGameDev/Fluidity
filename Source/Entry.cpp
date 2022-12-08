#include "../Include/Fluidity.h"

int main(int argc, char *argv[]) {
    auto App = Gtk::Application::create(argc, argv, "ex.angd.FluidityTest");

    Fluidity::FluidWindow window;
    window.set_default_size(200, 200);

    Fluidity::Animation animation(60);
    animation.AddKeyframe(0, 0, 1, Fluidity::Interpolation::Null)
            ->AddKeyframe(0, 0, 20, Fluidity::Interpolation::Null)
            ->AddKeyframe(1166, 568, 30, Fluidity::Interpolation::SmootherStep);
    window.PlayAnimation(animation);

    App->run(window);
}