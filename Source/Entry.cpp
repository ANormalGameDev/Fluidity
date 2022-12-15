#include "../Include/Composition.h"
#include "../Include/Math.h"

using namespace Fluidity;
using namespace Fluidity::Math;

int main(int argc, char *argv[]) {
    auto App = Gtk::Application::create(argc, argv, "ex.angd.test");

    Gtk::Window window;
    window.set_default_size(200, 200);

    Animation* anim = Animation::Create("BlahBlahBlah")->From(0)->To(60);
    anim->AddKeyframe(0)->X(1);
    anim->AddKeyframe(10)->X(10);
    anim->AddKeyframe(40)->X(50);
    anim->Easing(UnorderedPair(2, 3), BezierEasing);

    Animation* anim2 = Animation::Create("BlahBlahBlah2")->From(0)->To(60);
    anim2->AddKeyframe(0)->X(1);
    anim2->AddKeyframe(20)->X(30);
    anim2->AddKeyframe(50)->X(50);
    anim2->Easing(UnorderedPair(2, 3), BezierEasing);

    Timeline::Create(60)->FPS(60)->AddAnim(anim)->AddAnim(anim2)->Run();
    return App->run(window);
}