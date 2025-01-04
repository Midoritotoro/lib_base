#include "MainQueueProcessor.h"

#include <QtCore/QMutex>
#include <QtCore/QCoreApplication>

#include <QtGui/QtEvents>

#include "../core/CoreConcurrent.h"
#include <gsl/gsl>


namespace {

	auto ProcessorEventType() {
		static const auto Result = QEvent::Type(QEvent::registerEventType());
		return Result;
	}

	QMutex ProcessorMutex;
	MainQueueProcessor* ProcessorInstance = nullptr;

	enum class ProcessState : int {
		Processed,
		FillingUp,
		Waiting,
	};

	std::atomic<ProcessState> MainQueueProcessState/* = ProcessState(0)*/;
	void (*MainQueueProcessCallback)(void*)/* = nullptr*/;
	void* MainQueueProcessArgument/* = nullptr*/;

	void PushToMainQueueGeneric(void (*callable)(void*), void* argument) {
		auto expected = ProcessState::Processed;
		const auto fill = MainQueueProcessState.compare_exchange_strong(
			expected,
			ProcessState::FillingUp);
		if (fill) {
			MainQueueProcessCallback = callable;
			MainQueueProcessArgument = argument;
			MainQueueProcessState.store(ProcessState::Waiting);
		}

		auto event = std::make_unique<QEvent>(ProcessorEventType());
		QMutexLocker lock(&ProcessorMutex);

		if (ProcessorInstance)
			QCoreApplication::postEvent(ProcessorInstance, event.release());
	}

	void DrainMainQueueGeneric() {
		if (MainQueueProcessState.load() != ProcessState::Waiting)
			return;

		const auto callback = MainQueueProcessCallback;
		const auto argument = MainQueueProcessArgument;

		MainQueueProcessState.store(ProcessState::Processed);

		callback(argument);
	}

} // namespace

MainQueueProcessor::MainQueueProcessor() {
	acquire();
	concurrent::init_main_queue(PushToMainQueueGeneric);
	DrainMainQueueGeneric();
}

bool MainQueueProcessor::event(QEvent* event) {
	if (event->type() == ProcessorEventType()) {
		DrainMainQueueGeneric();
		return true;
	}

	return QObject::event(event);
}

void MainQueueProcessor::acquire() {
	Expects(ProcessorInstance == nullptr);

	QMutexLocker lock(&ProcessorMutex);
	ProcessorInstance = this;
}

void MainQueueProcessor::release() {
	Expects(ProcessorInstance == this);

	QMutexLocker lock(&ProcessorMutex);
	ProcessorInstance = nullptr;
}

MainQueueProcessor::~MainQueueProcessor() {
	release();
}