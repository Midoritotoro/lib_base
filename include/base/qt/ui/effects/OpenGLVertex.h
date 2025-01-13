#pragma once

#include <QVector3D>

class Vertex
{
public:
	constexpr Vertex();
	constexpr explicit Vertex(const QVector3D &position);

	constexpr const QVector3D& position() const;
    void setPosition(const QVector3D& position);

  // OpenGL Helpers
  static const int PositionTupleSize = 3;
  static constexpr int positionOffset();
  static constexpr int stride();

private:
  QVector3D m_position;
};

Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

constexpr inline Vertex::Vertex() {}
constexpr inline Vertex::Vertex(const QVector3D &position) : m_position(position) {}

constexpr inline const QVector3D& Vertex::position() const { return m_position; }
void inline Vertex::setPosition(const QVector3D& position) { m_position = position; }

constexpr inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }
constexpr inline int Vertex::stride() { return sizeof(Vertex); }

