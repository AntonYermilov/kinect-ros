#include <octomap/octomap.h>
#include <octomap/OcTree.h>

using namespace std;
using namespace octomap;

int main(int argc, char** argv) {
    
    OcTree tree(0.1);
    for (int x = -20; x < 60; x++) {
        for (int y = -20; y < 60; y++) {
            for (int z = -20; z < 20; z++) {
                if (x >= 20 && y >= 20)
                    continue;
                point3d pt((float) x * 0.05f, (float) y * 0.05f, (float) z * 0.05f);
                tree.updateNode(pt, true);
            }
        }
    }
    tree.writeBinary(argv[1]);

    return 0;
}
