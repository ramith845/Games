#include "pch.h"
#include "CollisionManager.h"
#include "object/GameObject.h"
#include "object/BallObject.h"

void CollisionManager::SetVertices(const GameObject& obj, std::vector<glm::vec2>& verts)
{
	BoundinBox bb{ obj.m_bb };
	verts.clear();
	if (obj.GetType() == ObjectType::Ball)
	{
		verts.push_back(obj.GetCenter());
		return;
	}
	verts.push_back(glm::vec2(bb.min.x, bb.min.y)); // top-left
	verts.push_back(glm::vec2(bb.max.x, bb.min.y)); // top-right
	verts.push_back(glm::vec2(bb.max.x, bb.max.y)); // bottom-right
	verts.push_back(glm::vec2(bb.min.x, bb.max.y)); // bottom-left
}

void CollisionManager::GetAxes(std::vector<glm::vec2>& verts)
{
	size_t count = verts.size();

	for (size_t i = 0; i < count; ++i)
	{
		glm::vec2 p1 = verts.at(i);
		glm::vec2 p2 = verts.at((i + 1) % count);
		glm::vec2 edge = p2 - p1;
		glm::vec2 normal = glm::normalize(glm::vec2(-edge.y, edge.x)); // Perpendicular
		m_Axes.push_back(normal);
	}
}

void CollisionManager::AddCircleAxis(const GameObject& circleObj, const GameObject& rectObj)
{
	if (circleObj.GetType() != ObjectType::Ball)
		return;

	glm::vec2 circleCenter = circleObj.GetCenter();
	glm::vec2 nearestPoint = glm::clamp(circleCenter, rectObj.m_bb.min, rectObj.m_bb.max);

	glm::vec2 axis = glm::normalize(circleCenter - nearestPoint);
	if (glm::length(axis) > 0.00001f)
		m_Axes.push_back(axis);
}

void set_min_max(MinMax& minMax, float value)
{
	minMax.min = std::min(minMax.min, value);
	minMax.max = std::max(minMax.max, value);
}

void CollisionManager::ProjectOntoAxis(const GameObject& objA, const GameObject& objB)
{
	for (const auto& axis : m_Axes)
	{
		// default value of members are correct vals of -inf and inf for float
		MinMax minMaxA{};
		MinMax minMaxB{};

		for (const auto& vert : m_VerticesA)
		{
			float proj = glm::dot(vert, axis);
			if (objA.GetType() == ObjectType::Ball)
			{
				float radius = objA.m_Size.x / 2.0f;
				minMaxA.min = proj - radius; // Ball radius
				minMaxA.max = proj + radius; // Ball radius
			}
			else
				set_min_max(minMaxA, proj);
		}

		for (const auto& vert : m_VerticesB)
		{
			float proj = glm::dot(vert, axis);
			if (objB.GetType() == ObjectType::Ball)
			{
				float radius = objB.m_Size.x / 2.0f;
				minMaxB.min = proj - radius; // Ball radius
				minMaxB.max = proj + radius; // Ball radius
			}
			else
				set_min_max(minMaxB, proj);
		}
		m_Projections.push_back(std::make_tuple(minMaxA, minMaxB));
	}
}

CollisionManager::Overlap_t CollisionManager::OverlapOnAxis(MinMax A, MinMax B)
{
	float value = std::min(std::abs(A.max - B.min), std::abs(B.max - A.min));
	bool isOverlap = (A.max > B.min) && (B.max > A.min);
	return std::make_tuple(isOverlap, value);
}

glm::vec2 CollisionManager::FindMinTranslationVector(glm::vec2 d)
{
	// Find the minimum translation vector (MTV) based on the overlaps
	if (m_Overlaps.empty())
		return glm::vec2(0.0f);
	float minOverlap = m_Overlaps[0];
	glm::vec2 mtvAxis = m_Axes[0];
	for (size_t i = 1; i < m_Overlaps.size(); ++i)
	{
		if (m_Overlaps[i] < minOverlap)
		{
			minOverlap = m_Overlaps[i];
			mtvAxis = m_Axes[i];
		}
	}
	// Apply the MTV to resolve the collision
	if (glm::dot(d, mtvAxis) < 0)
		mtvAxis = -mtvAxis;
	glm::vec2 mtv = mtvAxis * minOverlap;
	return mtv;
}

void ObjectMoveByMTV(glm::vec2 mtv, CollisionManager::MovableObj_t& movable_obj)
{
	movable_obj->get().SetPos(movable_obj->get().GetPos() + mtv); // apply MTV to the movable object
}

bool check_overlap(const CollisionManager::Overlap_t& overlap)
{
	return std::get<0>(overlap);
}

bool CollisionManager::ProcessProjections(bool isOverlapping)
{
	const int A{ 0 }, B{ 1 };
	for (const auto& proj : m_Projections)
	{
		MinMax minMaxA = std::get<A>(proj);
		MinMax minMaxB = std::get<B>(proj);

		Overlap_t overlap = OverlapOnAxis(minMaxA, minMaxB);

		isOverlapping &= std::get<0>(overlap);
		m_Overlaps.push_back(std::get<1>(overlap));
	}
	return isOverlapping;
}

void CollisionManager::ResolveCollision(const GameObject& objA, const GameObject& objB, MovableObj_t& movable_obj, glm::vec2& mtv, const bool isOverlapping)
{
	glm::vec2 dir{ objB.GetCenter() - objA.GetCenter() };
	if (isOverlapping)
	{
		if (movable_obj && &movable_obj->get() == &objB)
		{
			mtv = FindMinTranslationVector(dir);
			ObjectMoveByMTV(mtv, movable_obj);
			std::println("[COLLISION DETECTED] vec2( {}, {} )", mtv.x, mtv.y);
		}
		else if (movable_obj && &movable_obj->get() == &objA)
		{
			mtv = FindMinTranslationVector(-dir);
			ObjectMoveByMTV(mtv, movable_obj);
			std::println("[COLLISION DETECTED] vec2( {}, {} )", mtv.x, mtv.y);
		}
		/*else if (mtv != glm::vec2(0.f))
		{
			ObjectMoveByMTV(mtv / 2.0f, objB);
			ObjectMoveByMTV(-mtv / 2.0f, objA);
		}*/
	}
	movable_obj = std::nullopt;
}

CollisionManager::SATResult_t
CollisionManager::SATCollisionTest(const GameObject& objA, const GameObject& objB, MovableObj_t movable_obj)
{
	m_Axes.clear();
	m_Projections.clear();
	m_Overlaps.clear();

	SetVertices(objA, m_VerticesA);
	SetVertices(objB, m_VerticesB);

	if (objA.GetType() == ObjectType::Ball)
		AddCircleAxis(objA, objB); // if ball we need to get axes from both objects
	else
		GetAxes(m_VerticesA);

	if (objB.GetType() == ObjectType::Ball)
		AddCircleAxis(objB, objA);
	else
		GetAxes(m_VerticesB);

	// Project both objects onto each axis and check for overlap
	ProjectOntoAxis(objA, objB);

	bool isOverlapping{ true };
	glm::vec2 mtv{ 0.0f, 0.0f };

	isOverlapping = ProcessProjections(isOverlapping);

	if (isOverlapping)
		ResolveCollision(objA, objB, movable_obj, mtv, isOverlapping);

	return std::make_tuple(isOverlapping, mtv);
}
