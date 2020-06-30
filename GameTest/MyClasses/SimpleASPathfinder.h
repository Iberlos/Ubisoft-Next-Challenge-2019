#ifndef __SIMPLEASPATHFINDER_H__
#define __SIMPLEASPATHFINDER_H__

class CSimpleTileMap;
class SimpleScene;

struct PathNode
{
    enum PathNodeStatus
    {
        Unchecked,
        Open,
        Closed,
    };

    int parentNodeIndex;
    PathNodeStatus status;

    float f; // Current sum.
    float g; // Cost to get to current node.
    float h; // Heuristic: Manhatten distance to destination.
};

class SimpleASPathFinder
{
public:
    SimpleASPathFinder();
    virtual ~SimpleASPathFinder();

    void Reset(SimpleScene* a_scene);

    // Start a search, supply the starting tile x,y and ending tile x,y.
    // Returns true if path found, false otherwise.
    bool FindPath(SimpleScene* a_scene, int sx, int sy, int ex, int ey);

    // Retrieve the final path, pass a nullptr for "path" to just get length of the path.
    int GetPath(int* path, int maxdistance, int ex, int ey);

protected:
    int m_mapWidth;
    int m_mapHeight;
    CSimpleTileMap* m_tilemap;

    PathNode* m_Nodes;
    int m_NumNodes;

    int* m_OpenNodes;
    int m_NumOpen;

    // Internal methods.
    void AddToOpen(int nodeindex);
    void RemoveFromOpen(int nodeindex);
    int FindNodeIndexWithLowestFInOpen();

    int CalculateNodeIndex(int tx, int ty);
    void CalculateNodeCoord(int ti, int* x, int*y);
    int CheckIfNodeIsClearAndReturnNodeIndex(int tx, int ty);
    void AddNeighboursToOpenList(int nodeIndex, int endNodeIndex);

    float CalculateH(int nodeIndex1, int nodeIndex2);
};

#endif