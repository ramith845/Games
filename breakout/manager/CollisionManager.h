#pragma once
#include "object/GameObject.h"
#include "object/BallObject.h"


struct MinMax
{
	float min{ std::numeric_limits<float>::max() };
	float max{ std::numeric_limits<float>::lowest() };
};

class CollisionManager
{
public:
	using Overlap_t = std::tuple<bool, float>;
	using SATResult_t = std::tuple<bool, glm::vec2>;
	using MovableObj_t = std::optional<std::reference_wrapper<GameObject>>;
public:
	CollisionManager() = default;
	SATResult_t SATCollisionTest(const GameObject& obj_a, const GameObject& obj_b, MovableObj_t movable_obj);
private:
	void SetVertices(const GameObject& objA, std::vector<glm::vec2>& verts);
	void GetAxes(std::vector<glm::vec2>& verts);
	void AddCircleAxis(const GameObject& circleObj, const GameObject& rectObj);
	void ProjectOntoAxis(const GameObject& objA, const GameObject& objB);
	Overlap_t OverlapOnAxis(MinMax A, MinMax B);
	glm::vec2 FindMinTranslationVector(glm::vec2 d);
	bool ProcessProjections(bool isOverlapping);
	void ResolveCollision(const GameObject& objA, const GameObject& objB, MovableObj_t& movable_obj, glm::vec2& mtv, const bool isOverlapping);
private:
	std::vector<glm::vec2> m_Axes;
	std::vector<std::tuple<MinMax, MinMax>> m_Projections;
	std::vector<glm::vec2> m_VerticesA;
	std::vector<glm::vec2> m_VerticesB;
	std::vector<float> m_Overlaps;
	//std::function<void()>
};

