#include "stdafx.h"

#include "SimpleASPathfinder.h"
#include "SimpleScene.h"
#include "..\SimpleTileMap.h"
#include <cassert>


SimpleASPathFinder::SimpleASPathFinder()
{
    m_Nodes = nullptr;
    m_OpenNodes = nullptr;
}

SimpleASPathFinder::~SimpleASPathFinder()
{
    delete[] m_Nodes;
    delete[] m_OpenNodes;
}

void SimpleASPathFinder::Reset( SimpleScene* a_scene)
{
    //If the active level is not the same as the last run re initialize the pathfinder.
    CSimpleTileMap* activeTileMap = a_scene->GetMap();
    if (m_tilemap != activeTileMap)
    {
        m_tilemap = activeTileMap;

        m_mapWidth = activeTileMap->GetMapSize();
        m_mapHeight = activeTileMap->GetMapSize();

        m_NumNodes = m_mapWidth * m_mapHeight;

        if (m_Nodes != nullptr)
        {
            delete[] m_Nodes;
        }
        if (m_OpenNodes != nullptr)
        {
            delete[] m_OpenNodes;
        }
        m_Nodes = new PathNode[m_NumNodes];
        m_OpenNodes = new int[m_NumNodes];
    }

    //Reset values
    m_NumOpen = 0;

    for (int i = 0; i < m_NumNodes; i++)
    {
        m_Nodes[i].parentNodeIndex = -1;
        m_Nodes[i].status = PathNode::Unchecked;

        m_Nodes[i].f = 0;
        m_Nodes[i].g = FLT_MAX; // Set G to be highest cost possible, so any comparison will be better.
        m_Nodes[i].h = -1; // -1 indicates the heuristic hasn't been calculated yet.
    }
}

bool SimpleASPathFinder::FindPath(SimpleScene* a_scene, int sx, int sy, int ex, int ey)
{
    // Reset the pathfinder.
    Reset(a_scene);
    // Get the starting tile index and the destination tile index.
    int si = CalculateNodeIndex(sx, sy);
    int ei = CalculateNodeIndex(ex, ey);
    // Set starting node cost to zero, then add it to the open list to start the process.
    m_Nodes[si].g = 0.0f;
    AddToOpen(si);
    while (true)
    {
        // Find the lowest F and remove it from the open list.
        int fi = FindNodeIndexWithLowestFInOpen();
        RemoveFromOpen(fi);
        // If we found the end node, we're done.
        if (fi == ei)
        {
            return true;
        }
        // Mark the node as closed.
        m_Nodes[fi].status = PathNode::PathNodeStatus::Closed;
        // Add neighbours to open list.
        AddNeighboursToOpenList(fi, ei);
        // If we're out of nodes to check, then we're done without finding the end node.
        if (m_NumOpen == 0)
            return false;
    }

    // This shouldn't happen, we should either find the destination or run out of nodes in the open list.
    assert(false);
    return false;
}

int SimpleASPathFinder::GetPath(int* path, int maxdistance, int ex, int ey)
{
    int nodeIndex = CalculateNodeIndex(ex, ey);

    int length = 0;
    while (true)
    {
        if (path != nullptr && length >= maxdistance)
            return -1; // Path didn't fit in size.

        if (path != nullptr) // If no path array is passed in, just get the length.
            path[length] = nodeIndex;
        length++;

        nodeIndex = m_Nodes[nodeIndex].parentNodeIndex;

        if (nodeIndex == -1)
            return length;
    }

    // There should be no way to get here.
    assert(false);
    return -1;
}

void SimpleASPathFinder::AddToOpen(int nodeindex)
{
    assert(m_Nodes[nodeindex].status != PathNode::Closed);

    // If the node isn't already open, then add it to the list.
    if (m_Nodes[nodeindex].status != PathNode::Open)
    {
        m_OpenNodes[m_NumOpen] = nodeindex;
        m_NumOpen++;
        m_Nodes[nodeindex].status = PathNode::Open;
    }
}

void SimpleASPathFinder::RemoveFromOpen(int nodeindex)
{
    // Remove the node from the open list, since we don't care about order, replace the node we're removing with the last node in list.
    for (int i = 0; i < m_NumOpen; i++)
    {
        if (m_OpenNodes[i] == nodeindex)
        {
            m_NumOpen--;
            m_OpenNodes[i] = m_OpenNodes[m_NumOpen];
            return;
        }
    }
}

int SimpleASPathFinder::FindNodeIndexWithLowestFInOpen()
{
    // Loop through the nodes in the open list, then find and return the node with the lowest F score.
    int lowestF = m_OpenNodes[0];
    for (int i = 1; i < m_NumOpen; i++)
    {
        if (m_Nodes[lowestF].f > m_Nodes[m_OpenNodes[i]].f)
        {
            lowestF = m_OpenNodes[i];
        }
    }
    return lowestF; // PLACEHOLDER: to allow code to compile until method is implemented
}

int SimpleASPathFinder::CalculateNodeIndex(int tx, int ty)
{
    assert(tx >= 0 && tx < m_mapWidth &&
        ty >= 0 && ty < m_mapHeight);

    // Calculate the node index based on the tiles x/y.
    int index = ty * m_mapWidth + tx;
    return index; // PLACEHOLDER: to allow code to compile until method is implemented
}

void SimpleASPathFinder::CalculateNodeCoord(int ti, int* x, int*y)
{
    *x = ti % m_mapWidth;
    *y = ti / m_mapWidth;
}

int SimpleASPathFinder::CheckIfNodeIsClearAndReturnNodeIndex(int tx, int ty)
{
    int index = CalculateNodeIndex(tx, ty);
    // If the node is out of bounds, return -1 (an invalid tile index).
    if ((tx < 0 || tx >= m_mapWidth) && (ty < 0 || ty >= m_mapHeight)) index = -1;

    // If the node is already closed, return -1 (an invalid tile index).
    if (m_Nodes[index].status == PathNode::PathNodeStatus::Closed) index = -1;

    // If the node can't be walked on, return -1 (an invalid tile index).
    if (m_tilemap->GetTileMapValue(tx, ty)!=EMapValue::FLOOR) index = -1; //***************************************HARDCODE*******************

    // Return a valid tile index.
    return index; // PLACEHOLDER: to allow code to compile until method is implemented
}

void SimpleASPathFinder::AddNeighboursToOpenList(int nodeIndex, int endNodeIndex)
{
    // Calculate the tile x/y based on the nodeIndex.
    int nx, ny;
    CalculateNodeCoord(nodeIndex, &nx, &ny);
    // Fill an array with the four neighbour tile indices. (use CheckIfNodeIsClearAndReturnNodeIndex() for each to see if it's valid).
    int nodestoadd[4];
    nodestoadd[0] = CheckIfNodeIsClearAndReturnNodeIndex(nx + 1, ny);
    nodestoadd[1] = CheckIfNodeIsClearAndReturnNodeIndex(nx - 1, ny);
    nodestoadd[2] = CheckIfNodeIsClearAndReturnNodeIndex(nx, ny + 1);
    nodestoadd[3] = CheckIfNodeIsClearAndReturnNodeIndex(nx, ny - 1);

    // Loop through the array.
    for (int i = 0; i < 4; i++)
    {
        // Check if the node to add has a valid node index.
        if (nodestoadd[i] != -1)
        {
            int cost = 1; // Assume a travel cost of 1 for each tile.

            // Add the node to the open list.
            AddToOpen(nodestoadd[i]);
            // If the cost to get there from here (new G) is less than the previous cost (old G) to get there, then overwrite the values.
            if (m_Nodes[nodestoadd[i]].g > m_Nodes[nodeIndex].g + 1)
            {
                // Set the parent node.
                m_Nodes[nodestoadd[i]].parentNodeIndex = nodeIndex;
                // Set the new cost to travel to that node.
                m_Nodes[nodestoadd[i]].g = m_Nodes[nodeIndex].g + 1;

                // If we haven't already calculated the heuristic, calculate it.
                if (m_Nodes[nodestoadd[i]].h = -1)
                {
                    m_Nodes[nodestoadd[i]].h = CalculateH(nodestoadd[i], endNodeIndex);
                }
                // Calculate the final value.
                m_Nodes[nodestoadd[i]].f = m_Nodes[nodestoadd[i]].h + m_Nodes[nodestoadd[i]].g;
            }
        }
    }
}

float SimpleASPathFinder::CalculateH(int nodeIndex1, int nodeIndex2)
{
    // Calculate the h score using the Manhatten distance (absolute diff in x + absolute diff in y).
    int i_x, i_y, e_x, e_y;
    CalculateNodeCoord(nodeIndex1, &i_x, &i_y);
    CalculateNodeCoord(nodeIndex1, &e_x, &e_y);
    int h = abs(e_x - i_x) + abs(e_y - i_y);
    return (float)h;
}