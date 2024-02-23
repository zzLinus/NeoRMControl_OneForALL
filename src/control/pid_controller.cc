#include "pid_controller.hpp"

namespace Pid
{

    Pid_ctrller::Pid_ctrller(
        uint8_t mode,
        const fp32 kp,
        const fp32 ki,
        const fp32 kd,
        const fp32 max_out,
        const fp32 max_iout)
    {
        this->mode = mode;
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;

        this->max_out = max_out;
        this->max_iout = max_iout;
        this->Dbuf[0] = this->Dbuf[1] = this->Dbuf[2] = 0.0f;
        this->error[0] = this->error[1] = this->error[2] = this->Pout = this->Iout = this->Dout = this->out = 0.0f;
    }

    Pid_ctrller::~Pid_ctrller()
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

}  // namespace Pid
