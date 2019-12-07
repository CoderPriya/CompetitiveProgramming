#include <bits/stdc++.h>
#define FOR(i, x, y) for (int i = x; i < y; i++)
typedef long long ll;
typedef long double ld;
using namespace std;

const ld PI = 4 * atan(1);

ll a[100000];

// The polygon must be cyclic so we use the chord length formula
ld test1(ld r, int n) {
    ld angle = 0;
    FOR(i, 0, n) angle += 2 * asin(a[i] / (2 * r));
    return angle;
}
ld test2(ld r, int n) {
    ld angle = 0;
    FOR(i, 0, n - 1) angle += 2 * asin(a[i] / (2 * r));
    angle += 2 * PI - 2 * asin(a[n - 1] / (2 * r));
    return angle;
}

// Chord length formula + sine area law
void answer1(ld r, int n) {
    ld ans = 0;
    FOR(i, 0, n) {
        ld theta = 2 * asin(a[i] / (2 * r));
        ans += r * r * sin(theta) / 2;
    }
    cout << fixed << setprecision(4) << ans << '\n';
}
void answer2(ld r, int n) {
    ld ans = 0;
    FOR(i, 0, n - 1) {
        ld theta = 2 * asin(a[i] / (2 * r));
        ans += r * r * sin(theta) / 2;
    }
    ld theta = 2 * asin(a[n - 1] / (2 * r));
    ans -= r * r * sin(theta) / 2;
    cout << fixed << setprecision(4) << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    FOR(i, 0, n) cin >> a[i];
    sort(a, a + n);

    ld l = a[n - 1] / 2.0, r = 1e18;
    // Binary search for the radius
    while (r - l > 1e-9) {
        ld mid = (l + r) / 2;
        ld angle = test1(mid, n);
        if (angle >= 2 * PI) l = mid;
        else r = mid;
    }

    ld theta = test1(l, n);
    // Handle case where polygon doesn't contain centre
    if (abs(theta - 2 * PI) <= 1e-6) answer1(l, n);
    else {
        l = a[n - 1] / 2.0, r = 1e18;
        while (r - l > 1e-9) {
            ld mid = (l + r) / 2;
            ld angle = test2(mid, n);
            if (angle >= 2 * PI) r = mid;
            else l = mid;
        }
        answer2(l, n);
    }
    return 0;
}