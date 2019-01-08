/*
 * Copyright (C) 2012 Invensense, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define DATA_OUT_CTL1			(4 * 16)
#define DATA_OUT_CTL2			(4 * 16 + 2)
#define DATA_INTR_CTL			(4 * 16 + 12)

#define MOTION_EVENT_CTL		(4 * 16 + 14)

#define BM_BATCH_CNTR			(27 * 16)
#define BM_BATCH_THLD			(19 * 16 + 12)
#define BM_BATCH_MASK			(21 * 16 + 14)

#define ODR_ACCEL				(11 * 16 + 14)
#define ODR_GYRO				(11 * 16 + 10)
#define ODR_CPASS				(11 * 16 +  6)
#define ODR_ALS					(11 * 16 +  2)
#define ODR_QUAT6				(10 * 16 + 12)
#define ODR_QUAT9				(10 * 16 +  8)
#define ODR_PQUAT6				(10 * 16 +  4)
#define ODR_PRESSURE			(11 * 16 + 12)
#define ODR_GYRO_CALIBR			(11 * 16 +  8)
#define ODR_CPASS_CALIBR		(11 * 16 +  4)

#define ODR_CNTR_ACCEL			(9 * 16 + 14)
#define ODR_CNTR_GYRO			(9 * 16 + 10)
#define ODR_CNTR_CPASS			(9 * 16 +  6)
#define ODR_CNTR_ALS			(9 * 16 +  2)
#define ODR_CNTR_QUAT6			(8 * 16 + 12)
#define ODR_CNTR_QUAT9			(8 * 16 +  8)
#define ODR_CNTR_PQUAT6			(8 * 16 +  4)
#define ODR_CNTR_PRESSURE		(9 * 16 + 12)
#define ODR_CNTR_GYRO_CALIBR	(9 * 16 +  8)
#define ODR_CNTR_CPASS_CALIBR	(9 * 16 +  4)

#define CPASS_MTX_00            (23 * 16)
#define CPASS_MTX_01            (23 * 16 + 4)
#define CPASS_MTX_02            (23 * 16 + 8)
#define CPASS_MTX_10            (23 * 16 + 12)
#define CPASS_MTX_11            (24 * 16)
#define CPASS_MTX_12            (24 * 16 + 4)
#define CPASS_MTX_20            (24 * 16 + 8)
#define CPASS_MTX_21            (24 * 16 + 12)
#define CPASS_MTX_22            (25 * 16)

#define GYRO_SF					(19 * 16)
#define ACCEL_FB_GAIN			(34 * 16)
#define ACCEL_ONLY_GAIN			(16 * 16 + 12)

#define GYRO_BIAS_X				(139 * 16 +  4)
#define GYRO_BIAS_Y				(139 * 16 +  8)
#define GYRO_BIAS_Z				(139 * 16 + 12)
#define GYRO_ACCURACY			(138 * 16 +  2)
#define GYRO_BIAS_SET			(138 * 16 +  6)
#define GYRO_LAST_TEMPR			(134 * 16)
#define GYRO_SLOPE_X			(78 * 16 +  4)
#define GYRO_SLOPE_Y			(78 * 16 +  8)
#define GYRO_SLOPE_Z			(78 * 16 + 12)

#define ACCEL_BIAS_X            (110 * 16 +  4)
#define ACCEL_BIAS_Y            (110 * 16 +  8)
#define ACCEL_BIAS_Z            (110 * 16 + 12)
#define ACCEL_ACCURACY			(97 * 16)
#define ACCEL_CAL_RESET			(77 * 16)
#define ACCEL_VARIANCE_THRESH	(93 * 16)
#define ACCEL_CAL_RATE			(94 * 16 + 4)
#define ACCEL_PRE_SENSOR_DATA	(97 * 16 + 4)
#define ACCEL_COVARIANCE		(101 * 16 + 8)
#define ACCEL_ALPHA_VAR			(91 * 16)
#define ACCEL_A_VAR				(92 * 16)
#define ACCEL_CAL_INIT			(94 * 16 + 2)

#define CPASS_BIAS_X            (126 * 16 +  4)
#define CPASS_BIAS_Y            (126 * 16 +  8)
#define CPASS_BIAS_Z            (126 * 16 + 12)
#define CPASS_ACCURACY			(37 * 16)
#define CPASS_BIAS_SET			(34 * 16 + 14)
#define MAG_DIS9X				(112 * 16 + 12)
#define CPASS_COVARIANCE		(115 * 16)
#define CPASS_COVARIANCE_CUR	(118 * 16 +  8)
#define CPASS_REF_MAG_3D		(122 * 16)
#define CPASS_CAL_INIT			(114 * 16)
#define CPASS_EST_FIRST_BIAS	(113 * 16)
#define MAG_DISTURB_STATE		(113 * 16 + 2)
#define CPASS_VAR_COUNT			(112 * 16 + 6)
#define CPASS_COUNT_7			( 87 * 16 + 2)
#define CPASS_MAX_INNO			(124 * 16)
#define CPASS_BIAS_OFFSET		(113 * 16 + 4)
#define CPASS_CUR_BIAS_OFFSET	(114 * 16 + 4)
#define CPASS_PRE_SENSOR_DATA	( 87 * 16 + 4)
#define CPASS_TIME_BUFFER		(112 * 16 + 14)
#define CPASS_RADIUS_3D_THRESH_ANOMALY	(112 * 16 + 8)

#define CPASS_STATUS_CHK		(25 * 16 + 12)

#define MAGN_THR_9X				(35 * 16 +  8)
#define MAGN_LPF_THR_9X			(32 * 16 + 12)
#define QFB_THR_9X				(70 * 16 + 12)

#define DMPRATE_CNTR			(18 * 16 + 4)

#define PEDSTD_BP_B				(49 * 16 + 12)
#define PEDSTD_BP_A4			(52 * 16)
#define PEDSTD_BP_A3			(52 * 16 +  4)
#define PEDSTD_BP_A2			(52 * 16 +  8)
#define PEDSTD_BP_A1			(52 * 16 + 12)
#define PEDSTD_SB				(50 * 16 +  8)
#define PEDSTD_SB_TIME			(50 * 16 + 12)
#define PEDSTD_PEAKTHRSH		(57 * 16 +  8)
#define PEDSTD_TIML				(50 * 16 + 10)
#define PEDSTD_TIMH				(50 * 16 + 14)
#define PEDSTD_PEAK				(57 * 16 +  4)
#define PEDSTD_STEPCTR			(54 * 16)
#define PEDSTD_STEPCTR2			(58 * 16 +  8)
#define PEDSTD_TIMECTR			(60 * 16 +  4)
#define PEDSTD_DECI				(58 * 16)
#define PEDSTD_SB2				(60 * 16 + 14)
#define STPDET_TIMESTAMP		(18 * 16 +  8)
#define PEDSTD_DRIVE_STATE		(18 * 16)
#define PED_RATE				(58 * 16 +  4)

#define SMD_E1_THLD				(75 * 16 +  8)
#define SMD_CNTR_TH             (74 * 16 +  8)
#define SMD_CNTR_LO_TH          (74 * 16 + 12)
#define SMD_LOW_ENERGY_TIMER_TH (76 * 16 +  8)
#define SMD_E1_COUNTER_TH       (76 * 16 + 12)


// Wake on Motion
#define WOM_ENABLE              (64 * 16 + 14)
#define WOM_STATUS              (64 * 16 + 6)
#define WOM_THRESHOLD           (64 * 16)
#define WOM_CNTR_TH             (64 * 16 + 12)

#define TILT_ENABLE             (68 * 16 + 12)
#define BAC_STATE               (147 * 16)

// Accel FSR
#define ACC_SCALE               (30 * 16 + 0)
#define ACC_SCALE2              (79 * 16 + 4)

#define ACCEL_MASK		0x80
#define GYRO_MASK		0x40
#define CPASS_MASK		0x20
#define ALS_MASK		0x10
#define QUAT6_MASK		0x08
#define QUAT9_MASK		0x04
#define PQUAT6_MASK		0x02
#define FOOTER_MASK		0x01
#define PRESSURE_MASK	0x80
#define GYRO_CALIBR_MASK	0x40
#define CPASS_CALIBR_MASK	0x20
#define PED_STEPDET_MASK	0x10
#define HEADER2_MASK		0x08
#define PED_STEPIND_MASK	0x07

#define ACCEL_SET		0x8000
#define GYRO_SET		0x4000
#define CPASS_SET		0x2000
#define ALS_SET			0x1000
#define QUAT6_SET		0x0800
#define QUAT9_SET		0x0400
#define PQUAT6_SET		0x0200
#define FOOTER_SET		0x0100
#define PRESSURE_SET	0x0080
#define GYRO_CALIBR_SET	0x0040
#define CPASS_CALIBR_SET 0x0020
#define PED_STEPDET_SET	0x0010
#define HEADER2_SET		0x0008
#define PED_STEPIND_SET 0x0007

#define ACCEL_ACCURACY_MASK	0x40
#define GYRO_ACCURACY_MASK	0x20
#define CPASS_ACCURACY_MASK	0x10
#define GEOMAG_MASK			0x02
#define BATCH_MODE_MASK		0x01
#define ACT_RECOG_MASK		0x80

#define ACCEL_ACCURACY_SET	0x4000
#define GYRO_ACCURACY_SET	0x2000
#define CPASS_ACCURACY_SET	0x1000
#define GEOMAG_EN			0x0200
#define BATCH_MODE_EN		0x0100
#define ACT_RECOG_SET       0x0080

#define PEDOMETER_EN		0x40
#define PEDOMETER_INT_EN	0x20
#define SMD_EN				0x08
#define ACCEL_CAL_EN		0x02
#define GYRO_CAL_EN			0x01
#define COMPASS_CAL_EN		0x80
#define NINE_AXIS_EN        0x40

#define HEADER_SZ		2
#define ACCEL_DATA_SZ	6
#define GYRO_DATA_SZ	6
#define CPASS_DATA_SZ	6
#define ALS_DATA_SZ		8
#define QUAT6_DATA_SZ	12
#define QUAT9_DATA_SZ	14
#define PQUAT6_DATA_SZ	6
#define PRESSURE_DATA_SZ		6
#define GYRO_CALIBR_DATA_SZ		12
#define CPASS_CALIBR_DATA_SZ	12
#define PED_STEPDET_TIMESTAMP_SZ	4
#define FOOTER_SZ		2

#define HEADER2_SZ			2
#define ACCEL_ACCURACY_SZ	2
#define GYRO_ACCURACY_SZ	2
#define CPASS_ACCURACY_SZ	2
#define ACT_RECOG_SZ        6
