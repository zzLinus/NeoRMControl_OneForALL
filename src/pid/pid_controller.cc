#include "pid_controller.hpp"

namespace Pid
{

    Pid_controller::Pid_controller()
    {
    }
    Pid_controller::~Pid_controller()
    {
    }

    /** TODO :pub all these into constructor
     * @brief          pid struct data init
     * @param[out]     pid: PID struct data point
     * @param[in]      mode: PID_POSITION: normal pid
     *                 PID_DELTA: delta pid
     * @param[in]      PID: 0: kp, 1: ki, 2:kd
     * @param[in]      max_out: pid max out
     * @param[in]      max_iout: pid max iout
     * @retval         none
     */
    /**
     * @brief          pid struct data init
     * @param[out]     pid: PID结构数据指针
     * @param[in]      mode: PID_POSITION:普通PID
     *                 PID_DELTA: 差分PID
     * @param[in]      PID: 0: kp, 1: ki, 2:kd
     * @param[in]      max_out: pid最大输出
     * @param[in]      max_iout: pid最大积分输出
     * @retval         none
     */

    bool Pid_controller::init()
    {
        return OK;
    }

}  // namespace Pid
