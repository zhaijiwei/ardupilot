#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include <AP_AHRS/AP_AHRS.h>     // AHRS library
#include <AP_InertialNav/AP_InertialNav.h>     // Inertial Navigation library
#include <AC_AttitudeControl/AC_AttitudeControl.h> // Attitude controller library for sqrt controller
#include <AC_Fence/AC_Fence.h>         // Failsafe fence library

#define AC_AVOID_ACCEL_CMSS_MAX         100.0f  // maximum acceleration/deceleration in cm/s/s used to avoid hitting fence

// bit masks for enabled fence types.
#define AC_AVOID_DISABLED               0       // avoidance disabled
#define AC_AVOID_STOP_AT_FENCE          1       // stop at fence

/*
 * This class prevents the vehicle from leaving a polygon fence in
 * 2 dimensions by limiting velocity (adjust_velocity).
 */
class AC_Avoid {
public:

    /// Constructor
    AC_Avoid(const AP_AHRS& ahrs, const AP_InertialNav& inav, const AC_Fence& fence);

    /*
     * Adjusts the desired velocity so that the vehicle can stop
     * before the fence/object.
     */
    void adjust_velocity(const float kP, const float accel_cmss, Vector2f &desired_vel);

    static const struct AP_Param::GroupInfo var_info[];

private:

    /*
     * Adjusts the desired velocity for the circular fence.
     */
    void adjust_velocity_circle(const float kP, const float accel_cmss, Vector2f &desired_vel);

    /*
     * Gets the current position, relative to home (not relative to EKF origin)
     */
    Vector2f get_position();

    /*
     * Computes the speed such that the stopping distance
     * of the vehicle will be exactly the input distance.
     */
    float get_max_speed(const float kP, const float accel_cmss, const float distance);

    /*
     * Computes distance required to stop, given current speed.
     */
    float get_stopping_distance(const float kP, const float accel_cmss, const float speed);

    /*
     * Gets the fence margin in cm
     */
    float get_margin() { return _fence.get_margin() * 100.0f; }

    // external references
    const AP_AHRS& _ahrs;
    const AP_InertialNav& _inav;
    const AC_Fence& _fence;

    // parameters
    AP_Int8 _enabled;
};
