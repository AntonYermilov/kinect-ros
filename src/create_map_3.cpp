#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <cmath>

using namespace std;
using namespace octomap;

int main(int argc, char** argv) {
    
    OcTree tree(0.05);
    for (int x = -90; x < 90; x++) {
        for (int y = -90; y < 90; y++) {
            for (int z = -90; z < 90; z++) {
                if (z < 0 && sqrt((double) x * x + y * y) < 35.0)
                    continue;
                if (abs(sqrt((double) x * x + y * y + z * z) - 80.0) > 0.5)
                    continue;

                point3d pt((float) x * 0.025f, (float) y * 0.025f, (float) z * 0.025f);
                tree.updateNode(pt, true);
            }
        }
    }
    tree.writeBinary(argv[1]);

    return 0;
}

