#ifndef __MATH_H
#define __MATH_H

namespace Fluidity {
    namespace Math {
        struct Vector2 {
            int _X, _Y;
            Vector2() { _X = _Y = 0; }
            Vector2(int X, int Y) { _X = X; _Y = Y; }
            bool operator ==(const Vector2& P) const { return _X == P._X && _Y == P._Y; }
            Vector2* X(int Value) { _X = Value;  return this; }
            Vector2* Y(int Value) { _Y = Value;  return this; }
            Vector2* Ease(Vector2& To, float Elapsed, int(*Via)(int, int, float)) {
                _X = Via(_X, To._X, Elapsed);
                _Y = Via(_Y, To._Y, Elapsed);
                return this;
            }
        };

        struct UnorderedPair {
            const int _A, _B;
            UnorderedPair(int A, int B) : _A{A}, _B{B} {}
            bool operator ==(const UnorderedPair& P) const { return (_A + _B == P._A + P._B); }
            bool operator <(const UnorderedPair& P) const { return (_A + _B < P._A + P._B); }
            bool operator >(const UnorderedPair& P) const { return (_A + _B > P._A + P._B); }
        };

        static int BezierEasing(int Start, int End, float Elapsed) {
            return Start + (End - Start) * Elapsed * Elapsed * (3 - 2 * Elapsed);
        }
    }
}

#endif