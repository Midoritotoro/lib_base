#pragma once 


#ifdef LIB_BASE_ENABLE_QT
#include <QThreadPool>
#include <base/core/Types.h>


namespace base::images {
    void stackblurJob(
        uchar* src,
        uint width,
        uint height,
        uint radius,
        int cores,
        int core,
        int step,
        uchar* stack
    );

    class StackBlurTask: public QRunnable
    {
    public:
        uchar* src_;
        uint w_;
        uint h_;
        uint radius_;
        int cores_;
        int core_;
        int step_;
        uchar* stack_;

        StackBlurTask(
            uchar* _src, uint _w, uint _h,
            uint _radius, int _cores, int _core,
            int _step, uchar* _stack
        )
            : src_(_src)
            , w_(_w)
            , h_(_h)
            , radius_(_radius)
            , cores_(_cores)
            , core_(_core)
            , step_(_step)
            , stack_(_stack)
        {
        }

        void run() override
        {
            stackblurJob(
                src_, w_, h_, radius_, 
                cores_, core_, step_, stack_);
        }
    };
} // namespace base::images
#endif