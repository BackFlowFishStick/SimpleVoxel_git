//
// Created by Danmeng on 2026/3/26.
//

#ifndef SIMPLEVOXEL_MARCHINGCUBE_H
#define SIMPLEVOXEL_MARCHINGCUBE_H

#pragma once
#include "glm/glm.hpp"
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <algorithm>

struct GridCell
{
    glm::vec3 p[8];
    float val[8];
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct Triangle
{
    glm::vec3 p[3];
};

struct Edge
{
    size_t v1, v2;
    Edge(size_t a, size_t b) : v1(std::min(a, b)), v2(std::max(a, b)) {}

    bool operator==(const Edge& other) const
    {
        return v1 == other.v1 && v2 == other.v2;
    }
};

struct EdgeHash
{
    size_t operator()(const Edge& e) const
    {
        return std::hash<size_t>()(e.v1) ^ (std::hash<size_t>()(e.v2) << 1);
    }
};

struct FlipCandidate {
    size_t tri1, tri2;
    size_t v1, v2, v3, v4; // indices in m_indices
};
class MarchingCube
{
private:
    unsigned int m_dim;
    int m_resolution;
    float m_size;
    float m_isoLevel;
    std::vector<glm::vec3> m_gridPoints;
    std::vector<float> m_scalarValues;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::unordered_map<size_t, unsigned int> m_vertexMap;
    mutable float m_averageEdgeLength;

private:
    float scalerField(glm::vec3 p);
    glm::vec3 vertexInterp(float isoLevel, const glm::vec3& p1, const glm::vec3& p2, float valp1, float valp2);
    void generateMeshInfo();
    MarchingCube(unsigned int dim, int resolution, float size, float isoLevel);
    int polygonise(GridCell grid, float isoLevel, Triangle* triangles);
    size_t hashVertex(const glm::vec3& vertex) const;
    unsigned int addVertex(const glm::vec3& position, const glm::vec3& normal);
    int splitLongEdges(float maxLength);
    int collapseShortEdges(float collapseThreshold, float preserveThreshold);
    int equalizeValences();
    void tangentialRelaxation(int innerIterations, float lambda, bool preserveBoundary);
    void detectBoundaryVertices(std::vector<bool>& isBoundary);
    void updateNormals();
    void projectToSurface();
    bool canCollapseEdge(size_t v1, size_t v2) const;
    bool canFlipEdge(size_t v1, size_t v2, size_t v3, size_t v4) const;
    float computeEdgeLength(size_t v1, size_t v2) const;
    Vertex computeMergedVertex(int v0, int v1, const std::unordered_map<int, std::unordered_set<int>>& vertexToTriangles);
    void processEdgeForCollapse(size_t v0, size_t v1, float collapseThreshold, float preserveThreshold, const std::unordered_set<int>& boundaryVertices, std::vector<std::pair<int, int>>& edgesToCollapse, std::unordered_set<int64_t>& processedEdges);
    void checkMeshIntegrity() const;
    void buildTopologyInfo(std::unordered_map<int, std::unordered_set<int>>& vertexToTriangles,std::unordered_set<int>& boundaryVertices) const;
    void buildEdgeInfo(std::unordered_set<Edge, EdgeHash>& edges, std::vector<std::vector<size_t>>& adjacencies, std::unordered_map<Edge, std::vector<size_t>, EdgeHash>& edgeToTriangles) const;
    size_t findThirdVertex(size_t triangleIdx, size_t v1, size_t v2) const;
    int computeValenceCost(size_t v1, size_t v2, size_t v3, size_t v4, const std::vector<std::vector<size_t>>& adjacencies) const;
    int computeValenceCostAfterFlip(size_t v1, size_t v2, size_t v3, size_t v4, const std::vector<std::vector<size_t>>& adjacencies) const;
    bool performEdgeFlip(size_t tri1, size_t tri2, size_t v1, size_t v2, size_t v3, size_t v4);
    void updateLocalAdjacency(size_t v1, size_t v2, size_t v3, size_t v4, std::vector<std::vector<size_t>>& adjacencies);
    glm::vec3 computeLaplacianSmooth(size_t vertexIdx, const std::vector<size_t>& neighbors) const;
    float computeAverageEdgeLength() const;

public:
    MarchingCube();
    ~MarchingCube() = default;
    void isotropicalRemesh(float targetEdgeLength = 0.1f, int iterations = 10);
    const std::vector<Vertex>& getVertices() const;
    const std::vector<unsigned int>& getIndices() const;
    const float getAverageEdgeLength() const;
};


#endif //SIMPLEVOXEL_MARCHINGCUBE_H
