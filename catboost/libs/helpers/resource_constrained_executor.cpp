#include "resource_constrained_executor.h"

#include "exception.h"

#include <catboost/libs/logging/logging.h>

#include <util/generic/vector.h>
#include <util/stream/str.h>
#include <util/system/guard.h>

#include <exception>


namespace NCB {

    TResourceConstrainedExecutor::TResourceConstrainedExecutor(
        NPar::TLocalExecutor& localExecutor,
        const TString& resourceName,
        TResourceUnit resourceQuota,
        bool lenientMode
    )
        : LocalExecutor(localExecutor)
        , ResourceName(resourceName)
        , ResourceQuota(resourceQuota)
        , LenientMode(lenientMode)
    {}

    TResourceConstrainedExecutor::~TResourceConstrainedExecutor() noexcept(false) {
        ExecTasks();
    }

    void TResourceConstrainedExecutor::Add(TFunctionWithResourceUsage&& functionWithResourceUsage) {
        if (functionWithResourceUsage.first > ResourceQuota) {
            TStringStream message;
            message << "Resource " << ResourceName
                    << ": functionWithResourceUsage.ResourceUsage(" << functionWithResourceUsage.first
                    << ") > ResourceQuota(" << ResourceQuota << ')';
            if (LenientMode) {
                MATRIXNET_WARNING_LOG << message.Str() << Endl;
            } else {
                ythrow TCatboostException() << message.Str();
            }
        }

        Queue.insert(std::move(functionWithResourceUsage));
    }

    void TResourceConstrainedExecutor::ExecTasks() {
        while (!Queue.empty()) {
            TVector<std::function<void()>> tasks;

            TResourceUnit freeResource = ResourceQuota;

            while (true) {
                auto it = Queue.lower_bound(freeResource);
                if (it == Queue.end()) {
                    break;
                }

                freeResource -= it->first;
                tasks.push_back(std::move(it->second));
                Queue.erase(it);
            };

            if (LenientMode && tasks.empty()) {
                // execute at least one task even if it requests more than ResourceQuota
                auto it = Queue.begin();
                tasks.push_back(std::move(it->second));
                Queue.erase(it);
            } else {
                Y_ASSERT(!tasks.empty());
            }

            LocalExecutor.ExecRangeWithThrow(
                [&tasks](int id) {
                    tasks[id]();
                    tasks[id] = nullptr; // destroy early, do not wait for all tasks to finish
                },
                0,
                tasks.size(),
                NPar::TLocalExecutor::WAIT_COMPLETE
            );
        }
    }
}
