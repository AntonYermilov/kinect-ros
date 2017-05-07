#include <octomap/octomap.h>
#include <octomap/OcTree.h>

using namespace std;
using namespace octomap;

int main(int argc, char** argv) {
    
    OcTree tree(0.1);
    for (int x = -20; x < 60; x++) {
        for (int y = 0; y < 40; y++) {
            for (int z = 20; z < 60; z++) {
                point3d pt((float) x * 0.05f, (float) y * 0.05f, (float) z * 0.05f);
                tree.updateNode(pt, true);
            }
        }
    }
    for (int x = -20; x < 20; x++) {
        for (int y = -40; y < 40; y++) {
            for (int z = -20; z < 20; z++) {
                point3d pt((float) x * 0.05f, (float) y * 0.05f, (float) z * 0.05f);
                tree.updateNode(pt, true);
            }
        }
    }
    for (int x = -60; x < 20; x++) {
        for (int y = -40; y < 0; y++) {
            for (int z = -60; z < -20; z++) {
                point3d pt((float) x * 0.05f, (float) y * 0.05f, (float) z * 0.05f);
                tree.updateNode(pt, true);
            }
        }
    }
    tree.writeBinary(argv[1]);

    return 0;
}

