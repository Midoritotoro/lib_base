#pragma once 

#include <QObject>


namespace base::qt::common {
	class MainQueueProcessor : public QObject {
	public:
		MainQueueProcessor();
		~MainQueueProcessor();

	protected:
		bool event(QEvent* event) override;

	private:
		void acquire();
		void release();
	};
} // namespace base::qt::common