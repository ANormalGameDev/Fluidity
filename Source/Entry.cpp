#include "../Include/Fluidity.h"
#include <iostream>

using namespace Fluidity;
using namespace Fluidity::Math;

void OnAnimIntDraw(int val, Animation<int>* data) {
    std::cout << val << " " << data->Name() << "\n";
}

int main(int argc, char *argv[]) {
    auto App = Gtk::Application::create(argc, argv, "ex.angd.test");

    Gtk::Window window;
    window.set_default_size(200, 200);

    Animation<int>* anim_int = NewAnimation<int>("anim_int1");
    anim_int->AddKeyframe(5)->Data(40);
    anim_int->AddKeyframe(10)->Data(50);
    anim_int->AddKeyframe(60)->Data(120);
    anim_int->Easing(UnorderedPair(1, 2), Lerp)
            ->Easing(UnorderedPair(2, 3), BezierEasing);
    anim_int->Link(&OnAnimIntDraw);

    Timeline* tl = NewTimeline(60)->FPS(60)->AddAnim(anim_int);
    tl->Run();

    return App->run(window);
}