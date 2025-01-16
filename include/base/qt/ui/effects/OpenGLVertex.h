#pragma once

#include <QVector3D>

namespace base::qt::gl {
	using vec2 = QVector2D;
	using vec3 = QVector3D;
	using vec4 = QVector4D;

	class Vertex
	{
	public:
		constexpr inline Vertex() {};
		constexpr inline explicit Vertex(const QVector3D& position) :
			m_position(position)
		{};

		constexpr inline const QVector3D& position() const noexcept {
			return m_position;
		}

		void inline setPosition(const QVector3D& position) {
			m_position = position;
		}

		static constexpr inline
			[[nodiscard]] int positionOffset() {
			return offsetof(Vertex, m_position);
		}

		static constexpr inline
			[[nodiscard]] int stride() {
			return sizeof(Vertex);
		}

		static const int PositionTupleSize = 3;
	private:
		QVector3D m_position;
	};

	Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);
}