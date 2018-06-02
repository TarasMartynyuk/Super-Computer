#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;
//region struct

class Computation {
public:
    int start;
    int end;

    Computation& operator=(const Computation& other) = default;

    bool overlaps(const Computation& other) {
        return other.end > start ||
               other.start < end;
    }
};//endregion
std::ostream& operator<<(std::ostream& os, const Computation& comp) {
    os << "(" << comp.start << ", " << comp.end << ")";
    return os;
}
struct StartComparator {
    bool operator() (const Computation& left, const Computation& right) {
        return left.start < right.start;
    }
};

void readComputation(Computation& c) {
    int J;
    int D;
    cin >> J >> D;

    c.start = J;
    c.end = J + D;

    cin.ignore();
}
vector<Computation> readComputations() {
    int length;     cin >> length;    cin.ignore();
    vector<Computation> comps(length);

    for (size_t i = 0; i < length; ++i) {
        readComputation(comps.at(i));
    }

    return comps;
}

int main()
{
    vector<Computation> comps = readComputations();

    ostream_iterator<Computation> output(cerr, " ");
    std::copy(comps.begin(), comps.end(), output);


//
//    cout << "answer" << endl;

//    int array[6] = { 1, 2, 3, 4, 5, 6 };
//    std::vector<int> integers(array, array + 6);
//    std::ostream_iterator<int> output(cout, " ");
//
//    integers.push_back(2);
//    integers.push_back(3);
//    integers.push_back(4);
//
//    cout << "Vector integers contains: ";
//    std::copy(integers.begin(), integers.end(), output);
    return 0;
}