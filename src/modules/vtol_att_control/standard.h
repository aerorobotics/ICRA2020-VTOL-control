/****************************************************************************
 *
 *   Copyright (c) 2015 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
* @file standard.h
* VTOL with fixed multirotor motor configurations (such as quad) and a pusher
* (or puller aka tractor) motor for forward flight.
*
* @author Simon Wilks 		<simon@uaventure.com>
* @author Roman Bapst 		<bapstroman@gmail.com>
* @author Andreas Antener	<andreas@uaventure.com>
* @author Sander Smeets 	<sander@droneslab.com>
*
*/

#ifndef STANDARD_H
#define STANDARD_H
#include "vtol_type.h"
#include <parameters/param.h>
#include <drivers/drv_hrt.h>
#include <uORB/uORB.h>
#include <matrix/math.hpp>

class Standard : public VtolType
{

public:

	Standard(VtolAttitudeControl *_att_controller);
	~Standard() override = default;

	void update_vtol_state() override;
	void update_transition_state() override;
	void update_fw_state() override;
	void update_mc_state() override;
	void fill_actuator_outputs() override;
	void waiting_on_tecs() override;

private:

	struct {
		float gamma_cd0;
		float gamma_cd1;
		float gamma_cd2;
		float gamma_cl0;
		float gamma_cl1;
		float gamma_ctx;
		float gamma_ctz;
		float gamma_ctdz;
		float reset_cd0;
		float reset_cd1;
		float reset_cd2;
		float reset_cl0;
		float reset_cl1;
		float reset_ctx;
		float reset_ctz;
		float reset_ctdz;
		float lambda_a;
		float lambda_t;
		float theta_max_dev;
		float pusher_ramp_dt;
		float acc_lp_fc;
		float lambda_P;
		float back_trans_ramp;
		float down_pitch_max;
		float forward_thrust_scale;
		float pitch_setpoint_offset;
		float reverse_output;
		float reverse_delay;
	} _params_standard;

	struct {
		param_t gamma_cd0;
		param_t gamma_cd1;
		param_t gamma_cd2;
		param_t gamma_cl0;
		param_t gamma_cl1;
		param_t gamma_ctx;
		param_t gamma_ctz;
		param_t gamma_ctdz;
		param_t reset_cd0;
		param_t reset_cd1;
		param_t reset_cd2;
		param_t reset_cl0;
		param_t reset_cl1;
		param_t reset_ctx;
		param_t reset_ctz;
		param_t reset_ctdz;
		param_t lambda_a;
		param_t lambda_t;
		param_t theta_max_dev;
		param_t pusher_ramp_dt;
		param_t acc_lp_fc;
		param_t lambda_P;
		param_t back_trans_ramp;
		param_t down_pitch_max;
		param_t forward_thrust_scale;
		param_t pitch_setpoint_offset;
		param_t reverse_output;
		param_t reverse_delay;
	} _params_handles_standard;

	enum vtol_mode {
		MC_MODE = 0,
		TRANSITION_TO_FW,
		TRANSITION_TO_MC,
		FW_MODE
	};

	struct {
		vtol_mode flight_mode;			// indicates in which mode the vehicle is in
		hrt_abstime transition_start;	// at what time did we start a transition (front- or backtransition)
	} _vtol_schedule;

	float _pusher_throttle{0.0f};
	float _reverse_output{0.0f};
	float _airspeed_trans_blend_margin{0.0f};
	orb_advert_t _pub_dbg_val;
	orb_advert_t _pub_dbg_vect;

	int	_sensor_combined_sub{-1};

	hrt_abstime prev_iteration_time{0};
	matrix::Matrix<float, 3, 3> prev_Phi_T;
	matrix::Matrix<float, 3, 5> prev_Phi_A;
	matrix::Vector<float, 3> theta_T; // [CTx, CTz, CTDz]
	matrix::Vector<float, 3> theta_T0;
	matrix::Vector<float, 5> theta_A; // [CD0, CD1, CD2, CL0, CL1]
	matrix::Vector<float, 5> theta_A0;
	matrix::Vector<float, 5> Gamma_A_diag;
	matrix::Vector<float, 3> Gamma_T_diag;
	matrix::Matrix<float, 3, 8> W; // filtered [Phi_T Phi_A]
	matrix::Matrix<float, 8, 8> P; // covariance  [CTx, CTz, CTDz, CD0, CD1, CD2, CL0, CL1]
	matrix::Vector<float, 3> a_f; // filtered acceleration
	void parameters_update() override;
};
#endif
