/*
 * Copyright (c) 2012-2017, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TEGRA_CLK_H
#define __TEGRA_CLK_H

#include <linux/clk-provider.h>
#include <linux/clkdev.h>

/**
 * struct tegra_clk_sync_source - external clock source from codec
 *
 * @hw: handle between common and hardware-specific interfaces
 * @rate: input frequency from source
 * @max_rate: max rate allowed
 */
struct tegra_clk_sync_source {
	struct		clk_hw hw;
	unsigned long	rate;
	unsigned long	max_rate;
};

#define to_clk_sync_source(_hw)					\
	container_of(_hw, struct tegra_clk_sync_source, hw)

extern const struct clk_ops tegra_clk_sync_source_ops;
extern int *periph_clk_enb_refcnt;
extern bool has_ccplex_therm_control;
extern bool div1_5_not_allowed;

struct clk *tegra_clk_register_sync_source(const char *name,
					   unsigned long max_rate);

/**
 * struct tegra_clk_frac_div - fractional divider clock
 *
 * @hw:		handle between common and hardware-specific interfaces
 * @reg:	register containing divider
 * @flags:	hardware-specific flags
 * @shift:	shift to the divider bit field
 * @width:	width of the divider bit field
 * @frac_width:	width of the fractional bit field
 * @lock:	register lock
 *
 * Flags:
 * TEGRA_DIVIDER_ROUND_UP - This flags indicates to round up the divider value.
 * TEGRA_DIVIDER_FIXED - Fixed rate PLL dividers has addition override bit, this
 *      flag indicates that this divider is for fixed rate PLL.
 * TEGRA_DIVIDER_INT - Some modules can not cope with the duty cycle when
 *      fraction bit is set. This flags indicates to calculate divider for which
 *      fracton bit will be zero.
 * TEGRA_DIVIDER_UART - UART module divider has additional enable bit which is
 *      set when divider value is not 0. This flags indicates that the divider
 *      is for UART module.
 */
struct tegra_clk_frac_div {
	struct clk_hw	hw;
	void __iomem	*reg;
	u8		flags;
	u8		shift;
	u8		width;
	u8		frac_width;
	spinlock_t	*lock;
};

#define to_clk_frac_div(_hw) container_of(_hw, struct tegra_clk_frac_div, hw)

#define TEGRA_DIVIDER_ROUND_UP BIT(0)
#define TEGRA_DIVIDER_FIXED BIT(1)
#define TEGRA_DIVIDER_INT BIT(2)
#define TEGRA_DIVIDER_UART BIT(3)

extern const struct clk_ops tegra_clk_frac_div_ops;
struct clk *tegra_clk_register_divider(const char *name,
		const char *parent_name, void __iomem *reg,
		unsigned long flags, u8 clk_divider_flags, u8 shift, u8 width,
		u8 frac_width, spinlock_t *lock);
struct clk *tegra_clk_register_mc(const char *name, const char *parent_name,
				  void __iomem *reg, spinlock_t *lock);
struct clk *tegra_clk_register_mc_t210(const char *name,
		const char *parent_name, void __iomem *reg, spinlock_t *lock);

/*
 * Tegra PLL:
 *
 * In general, there are 3 requirements for each PLL
 * that SW needs to be comply with.
 * (1) Input frequency range (REF).
 * (2) Comparison frequency range (CF). CF = REF/DIVM.
 * (3) VCO frequency range (VCO).  VCO = CF * DIVN.
 *
 * The final PLL output frequency (FO) = VCO >> DIVP.
 */

/**
 * struct tegra_clk_pll_freq_table - PLL frequecy table
 *
 * @input_rate:		input rate from source
 * @output_rate:	output rate from PLL for the input rate
 * @n:			feedback divider
 * @m:			input divider
 * @p:			post divider
 * @cpcon:		charge pump current
 * @sdm_data:		fraction divider setting (0 = disabled)
 */
struct tegra_clk_pll_freq_table {
	unsigned long	input_rate;
	unsigned long	output_rate;
	u32		n;
	u32		m;
	u8		p;
	u8		cpcon;
	u16		sdm_data;
};

/**
 * struct pdiv_map - map post divider to hw value
 *
 * @pdiv:		post divider
 * @hw_val:		value to be written to the PLL hw
 */
struct pdiv_map {
	u8 pdiv;
	u8 hw_val;
};

/**
 * struct div_nmp - offset and width of m,n and p fields
 *
 * @divn_shift:	shift to the feedback divider bit field
 * @divn_width:	width of the feedback divider bit field
 * @divm_shift:	shift to the input divider bit field
 * @divm_width:	width of the input divider bit field
 * @divp_shift:	shift to the post divider bit field
 * @divp_width:	width of the post divider bit field
 * @override_divn_shift: shift to the feedback divider bitfield in override reg
 * @override_divm_shift: shift to the input divider bitfield in override reg
 * @override_divp_shift: shift to the post divider bitfield in override reg
 */
struct div_nmp {
	u8		divn_shift;
	u8		divn_width;
	u8		divm_shift;
	u8		divm_width;
	u8		divp_shift;
	u8		divp_width;
	u8		override_divn_shift;
	u8		override_divm_shift;
	u8		override_divp_shift;
};

#define MAX_PLL_MISC_REG_COUNT	6

struct tegra_clk_pll;

/**
 * struct tegra_clk_pll_params - PLL parameters
 *
 * @input_min:			Minimum input frequency
 * @input_max:			Maximum input frequency
 * @cf_min:			Minimum comparison frequency
 * @cf_max:			Maximum comparison frequency
 * @vco_min:			Minimum VCO frequency
 * @vco_max:			Maximum VCO frequency
 * @base_reg:			PLL base reg offset
 * @misc_reg:			PLL misc reg offset
 * @lock_reg:			PLL lock reg offset
 * @lock_mask:			Bitmask for PLL lock status
 * @lock_enable_bit_idx:	Bit index to enable PLL lock
 * @iddq_reg:			PLL IDDQ register offset
 * @iddq_bit_idx:		Bit index to enable PLL IDDQ
 * @reset_reg:			Register offset of where RESET bit is
 * @reset_bit_idx:		Shift of reset bit in reset_reg
 * @sdm_din_reg:		Register offset where SDM settings are
 * @sdm_din_mask:		Mask of SDM divider bits
 * @sdm_ctrl_reg:		Register offset where SDM enable is
 * @sdm_ctrl_en_mask:		Mask of SDM enable bit
 * @sdm_ctrl_reset_mask:	Mask of SDM reset bit
 * @ssc_ctrl_reg:		Register offset where SSC settings are
 * @ssc_ctrl_en_mask:		Mask of SSC enable bit
 * @aux_reg:			AUX register offset
 * @dyn_ramp_reg:		Dynamic ramp control register offset
 * @ext_misc_reg:		Miscellaneous control register offsets
 * @pmc_divnm_reg:		n, m divider PMC override register offset (PLLM)
 * @pmc_divp_reg:		p divider PMC override register offset (PLLM)
 * @flags:			PLL flags
 * @stepa_shift:		Dynamic ramp step A field shift
 * @stepb_shift:		Dynamic ramp step B field shift
 * @lock_delay:			Delay in us if PLL lock is not used
 * @max_p:			maximum value for the p divider
 * @defaults_set:		Boolean signaling all reg defaults for PLL set.
 * @pdiv_tohw:			mapping of p divider to register values
 * @div_nmp:			offsets and widths on n, m and p fields
 * @freq_table:			array of frequencies supported by PLL
 * @fixed_rate:			PLL rate if it is fixed
 * @mdiv_default:		Default value for fixed mdiv for this PLL
 * @round_p_to_pdiv:		Callback used to round p to the closed pdiv
 * @set_gain:			Callback to adjust N div for SDM enabled
 *				PLL's based on fractional divider value.
 * @calc_rate:			Callback used to change how out of table
 *				rates (dividers and multipler) are calculated.
 * @adjust_vco:			Callback to adjust the programming range of the
 *				divider range (if SDM is present)
 * @set_defaults:		Callback which will try to initialize PLL
 *				registers to sane default values. This is first
 *				tried during PLL registration, but if the PLL
 *				is already enabled, it will be done the first
 *				time the rate is changed while the PLL is
 *				disabled.
 * @dyn_ramp:			Callback which can be used to define a custom
 *				dynamic ramp function for a given PLL.
 *
 * Flags:
 * TEGRA_PLL_USE_LOCK - This flag indicated to use lock bits for
 *     PLL locking. If not set it will use lock_delay value to wait.
 * TEGRA_PLL_HAS_CPCON - This flag indicates that CPCON value needs
 *     to be programmed to change output frequency of the PLL.
 * TEGRA_PLL_SET_LFCON - This flag indicates that LFCON value needs
 *     to be programmed to change output frequency of the PLL.
 * TEGRA_PLL_SET_DCCON - This flag indicates that DCCON value needs
 *     to be programmed to change output frequency of the PLL.
 * TEGRA_PLLU - PLLU has inverted post divider. This flags indicated
 *     that it is PLLU and invert post divider value.
 * TEGRA_PLLM - PLLM has additional override settings in PMC. This
 *     flag indicates that it is PLLM and use override settings.
 * TEGRA_PLL_FIXED - We are not supposed to change output frequency
 *     of some plls.
 * TEGRA_PLLE_CONFIGURE - Configure PLLE when enabling.
 * TEGRA_PLL_LOCK_MISC - Lock bit is in the misc register instead of the
 *     base register.
 * TEGRA_PLL_BYPASS - PLL has bypass bit
 * TEGRA_PLL_HAS_LOCK_ENABLE - PLL has bit to enable lock monitoring
 * TEGRA_MDIV_NEW - Switch to new method for calculating fixed mdiv
 *     it may be more accurate (especially if SDM present)
 * TEGRA_PLLMB - PLLMB has should be treated similar to PLLM. This
 *     flag indicated that it is PLLMB.
 * TEGRA_PLL_VCO_OUT - Used to indicate that the PLL has a VCO output
 */
struct tegra_clk_pll_params {
	unsigned long	input_min;
	unsigned long	input_max;
	unsigned long	cf_min;
	unsigned long	cf_max;
	unsigned long	vco_min;
	unsigned long	vco_max;

	u32		base_reg;
	u32		misc_reg;
	u32		lock_reg;
	u32		lock_mask;
	u32		lock_enable_bit_idx;
	u32		iddq_reg;
	u32		iddq_bit_idx;
	u32		reset_reg;
	u32		reset_bit_idx;
	u32		sdm_din_reg;
	u32		sdm_din_mask;
	u32		sdm_ctrl_reg;
	u32		sdm_ctrl_en_mask;
	u32		sdm_ctrl_reset_mask;
	u32		ssc_ctrl_reg;
	u32		ssc_ctrl_en_mask;
	u32		aux_reg;
	u32		dyn_ramp_reg;
	u32		ext_misc_reg[MAX_PLL_MISC_REG_COUNT];
	u32		pmc_divnm_reg;
	u32		pmc_divp_reg;
	u32		flags;
	int		stepa_shift;
	int		stepb_shift;
	int		lock_delay;
	int		max_p;
	bool		defaults_set;
	const struct pdiv_map *pdiv_tohw;
	struct div_nmp	*div_nmp;
	struct tegra_clk_pll_freq_table	*freq_table;
	unsigned long	fixed_rate;
	u16		mdiv_default;
	u32	(*round_p_to_pdiv)(u32 p, u32 *pdiv);
	void	(*set_gain)(struct tegra_clk_pll_freq_table *cfg);
	int	(*calc_rate)(struct clk_hw *hw,
			struct tegra_clk_pll_freq_table *cfg,
			unsigned long rate, unsigned long parent_rate);
	unsigned long	(*adjust_vco)(struct tegra_clk_pll_params *pll_params,
				unsigned long parent_rate);
	void	(*set_defaults)(struct tegra_clk_pll *pll);
	int	(*dyn_ramp)(struct tegra_clk_pll *pll,
			struct tegra_clk_pll_freq_table *cfg);
};

#define TEGRA_PLL_USE_LOCK BIT(0)
#define TEGRA_PLL_HAS_CPCON BIT(1)
#define TEGRA_PLL_SET_LFCON BIT(2)
#define TEGRA_PLL_SET_DCCON BIT(3)
#define TEGRA_PLLU BIT(4)
#define TEGRA_PLLM BIT(5)
#define TEGRA_PLL_FIXED BIT(6)
#define TEGRA_PLLE_CONFIGURE BIT(7)
#define TEGRA_PLL_LOCK_MISC BIT(8)
#define TEGRA_PLL_BYPASS BIT(9)
#define TEGRA_PLL_HAS_LOCK_ENABLE BIT(10)
#define TEGRA_MDIV_NEW BIT(11)
#define TEGRA_PLLMB BIT(12)
#define TEGRA_PLL_VCO_OUT BIT(13)
#define TEGRA_PLLDP BIT(14)

/**
 * struct tegra_clk_pll - Tegra PLL clock
 *
 * @hw:		handle between common and hardware-specifix interfaces
 * @clk_base:	address of CAR controller
 * @pmc:	address of PMC, required to read override bits
 * @lock:	register lock
 * @params:	PLL parameters
 * @prepared:	clock is prepared or not
 */
struct tegra_clk_pll {
	struct clk_hw	hw;
	void __iomem	*clk_base;
	void __iomem	*pmc;
	spinlock_t	*lock;
	struct tegra_clk_pll_params	*params;
	bool		prepared;
};

#define to_clk_pll(_hw) container_of(_hw, struct tegra_clk_pll, hw)

/**
 * struct tegra_audio_clk_info - Tegra Audio Clk Information
 *
 * @name:	name for the audio pll
 * @pll_params:	pll_params for audio pll
 * @clk_id:	clk_ids for the audio pll
 * @parent:	name of the parent of the audio pll
 */
struct tegra_audio_clk_info {
	char *name;
	struct tegra_clk_pll_params *pll_params;
	int clk_id;
	char *parent;
	struct clk *(*register_fn)(const char *name, const char *parent_name,
		void __iomem *clk_base, void __iomem *pmc, unsigned long flags,
		struct tegra_clk_pll_params *pll_params, spinlock_t *lock);
};

extern const struct clk_ops tegra_clk_pll_ops;
extern const struct clk_ops tegra_clk_plle_ops;
struct clk *tegra_clk_register_pll(const char *name, const char *parent_name,
		void __iomem *clk_base, void __iomem *pmc,
		unsigned long flags, struct tegra_clk_pll_params *pll_params,
		spinlock_t *lock);

struct clk *tegra_clk_register_plle(const char *name, const char *parent_name,
		void __iomem *clk_base, void __iomem *pmc,
		unsigned long flags, struct tegra_clk_pll_params *pll_params,
		spinlock_t *lock);

struct clk *tegra_clk_register_pllxc(const char *name, const char *parent_name,
			    void __iomem *clk_base, void __iomem *pmc,
			    unsigned long flags,
			    struct tegra_clk_pll_params *pll_params,
			    spinlock_t *lock);

struct clk *tegra_clk_register_pllxc_tegra210(const char *name,
			const char *parent_name, void __iomem *clk_base,
			void __iomem *pmc, unsigned long flags,
			struct tegra_clk_pll_params *pll_params,
			spinlock_t *lock);

struct clk *tegra_clk_register_pllm(const char *name, const char *parent_name,
			   void __iomem *clk_base, void __iomem *pmc,
			   unsigned long flags,
			   struct tegra_clk_pll_params *pll_params,
			   spinlock_t *lock);

struct clk *tegra_clk_register_pllc(const char *name, const char *parent_name,
			   void __iomem *clk_base, void __iomem *pmc,
			   unsigned long flags,
			   struct tegra_clk_pll_params *pll_params,
			   spinlock_t *lock);

struct clk *tegra_clk_register_pllre(const char *name, const char *parent_name,
			   void __iomem *clk_base, void __iomem *pmc,
			   unsigned long flags,
			   struct tegra_clk_pll_params *pll_params,
			   spinlock_t *lock, unsigned long parent_rate);

struct clk *tegra_clk_register_pllre_tegra210(const char *name,
			   const char *parent_name, void __iomem *clk_base,
			   void __iomem *pmc, unsigned long flags,
			   struct tegra_clk_pll_params *pll_params,
			   spinlock_t *lock, unsigned long parent_rate);

struct clk *tegra_clk_register_plle_tegra114(const char *name,
				const char *parent_name,
				void __iomem *clk_base, unsigned long flags,
				struct tegra_clk_pll_params *pll_params,
				spinlock_t *lock);

struct clk *tegra_clk_register_plle_tegra210(const char *name,
				const char *parent_name,
				void __iomem *clk_base, unsigned long flags,
				struct tegra_clk_pll_params *pll_params,
				spinlock_t *lock);

struct clk *tegra_clk_register_pllc_tegra210(const char *name,
				const char *parent_name, void __iomem *clk_base,
				void __iomem *pmc, unsigned long flags,
				struct tegra_clk_pll_params *pll_params,
				spinlock_t *lock);

struct clk *tegra_clk_register_pllss_tegra210(const char *name,
				const char *parent_name, void __iomem *clk_base,
				unsigned long flags,
				struct tegra_clk_pll_params *pll_params,
				spinlock_t *lock);

struct clk *tegra_clk_register_pllss(const char *name, const char *parent_name,
			   void __iomem *clk_base, unsigned long flags,
			   struct tegra_clk_pll_params *pll_params,
			   spinlock_t *lock);

struct clk *tegra_clk_register_pllmb(const char *name, const char *parent_name,
			   void __iomem *clk_base, void __iomem *pmc,
			   unsigned long flags,
			   struct tegra_clk_pll_params *pll_params,
			   spinlock_t *lock);

/**
 * struct tegra_clk_pll_out - PLL divider down clock
 *
 * @hw:			handle between common and hardware-specific interfaces
 * @reg:		register containing the PLL divider
 * @enb_bit_idx:	bit to enable/disable PLL divider
 * @rst_bit_idx:	bit to reset PLL divider
 * @lock:		register lock
 * @flags:		hardware-specific flags
 */
struct tegra_clk_pll_out {
	struct clk_hw	hw;
	void __iomem	*reg;
	u8		enb_bit_idx;
	u8		rst_bit_idx;
	spinlock_t	*lock;
	u8		flags;
};

#define to_clk_pll_out(_hw) container_of(_hw, struct tegra_clk_pll_out, hw)

extern const struct clk_ops tegra_clk_pll_out_ops;
struct clk *tegra_clk_register_pll_out(const char *name,
		const char *parent_name, void __iomem *reg, u8 enb_bit_idx,
		u8 rst_bit_idx, unsigned long flags, u8 pll_div_flags,
		spinlock_t *lock);

/**
 * struct tegra_clk_periph_regs -  Registers controlling peripheral clock
 *
 * @enb_reg:		read the enable status
 * @enb_set_reg:	write 1 to enable clock
 * @enb_clr_reg:	write 1 to disable clock
 * @rst_reg:		read the reset status
 * @rst_set_reg:	write 1 to assert the reset of peripheral
 * @rst_clr_reg:	write 1 to deassert the reset of peripheral
 */
struct tegra_clk_periph_regs {
	u32 enb_reg;
	u32 enb_set_reg;
	u32 enb_clr_reg;
	u32 rst_reg;
	u32 rst_set_reg;
	u32 rst_clr_reg;
};

/**
 * struct tegra_clk_periph_gate - peripheral gate clock
 *
 * @magic:		magic number to validate type
 * @hw:			handle between common and hardware-specific interfaces
 * @clk_base:		address of CAR controller
 * @regs:		Registers to control the peripheral
 * @flags:		hardware-specific flags
 * @clk_num:		Clock number
 * @enable_refcnt:	array to maintain reference count of the clock
 * @prepared:		clock is prepared or not
 *
 * Flags:
 * TEGRA_PERIPH_NO_RESET - This flag indicates that reset is not allowed
 *     for this module.
 * TEGRA_PERIPH_MANUAL_RESET - This flag indicates not to reset module
 *     after clock enable and driver for the module is responsible for
 *     doing reset.
 * TEGRA_PERIPH_ON_APB - If peripheral is in the APB bus then read the
 *     bus to flush the write operation in apb bus. This flag indicates
 *     that this peripheral is in apb bus.
 * TEGRA_PERIPH_WAR_1005168 - Apply workaround for Tegra114 MSENC bug
 */
struct tegra_clk_periph_gate {
	u32			magic;
	struct clk_hw		hw;
	void __iomem		*clk_base;
	u8			flags;
	int			clk_num;
	int			*enable_refcnt;
	const struct tegra_clk_periph_regs *regs;
	bool			prepared;
};

#define to_clk_periph_gate(_hw)					\
	container_of(_hw, struct tegra_clk_periph_gate, hw)

#define TEGRA_CLK_PERIPH_GATE_MAGIC 0x17760309

#define TEGRA_PERIPH_NO_RESET BIT(0)
#define TEGRA_PERIPH_MANUAL_RESET BIT(1)
#define TEGRA_PERIPH_ON_APB BIT(2)
#define TEGRA_PERIPH_WAR_1005168 BIT(3)
#define TEGRA_PERIPH_NO_DIV BIT(4)
#define TEGRA_PERIPH_NO_GATE BIT(5)

extern const struct clk_ops tegra_clk_periph_gate_ops;
struct clk *tegra_clk_register_periph_gate(const char *name,
		const char *parent_name, u8 gate_flags, void __iomem *clk_base,
		unsigned long flags, int clk_num, int *enable_refcnt);

struct tegra_clk_periph_fixed {
	struct clk_hw hw;
	void __iomem *base;
	const struct tegra_clk_periph_regs *regs;
	unsigned int mul;
	unsigned int div;
	unsigned int num;
};

struct clk *tegra_clk_register_periph_fixed(const char *name,
					    const char *parent,
					    unsigned long flags,
					    void __iomem *base,
					    unsigned int mul,
					    unsigned int div,
					    unsigned int num);

struct tegra_clk_periph_reparent_policy {
	u8 low_rate_parent_idx;
	u8 high_rate_parent_idx;
	unsigned long threshold;
};

/**
 * struct clk-periph - peripheral clock
 *
 * @magic:	magic number to validate type
 * @hw:		handle between common and hardware-specific interfaces
 * @mux:	mux clock
 * @divider:	divider clock
 * @gate:	gate clock
 * @rpolicy	reparent policy
 * @mux_ops:	mux clock ops
 * @div_ops:	divider clock ops
 * @gate_ops:	gate clock ops
 * @prepare:	clock is prepared or not
 */
struct tegra_clk_periph {
	u32			magic;
	struct clk_hw		hw;
	struct clk_mux		mux;
	struct tegra_clk_frac_div	divider;
	struct tegra_clk_periph_gate	gate;
	struct tegra_clk_periph_reparent_policy	rpolicy;

	const struct clk_ops	*mux_ops;
	const struct clk_ops	*div_ops;
	const struct clk_ops	*gate_ops;
	bool			prepared;
};

#define to_clk_periph(_hw) container_of(_hw, struct tegra_clk_periph, hw)

#define TEGRA_CLK_PERIPH_MAGIC 0x18221223

extern const struct clk_ops tegra_clk_periph_ops;
struct clk *tegra_clk_register_periph(const char *name,
		const char **parent_names, int num_parents,
		struct tegra_clk_periph *periph, void __iomem *clk_base,
		u32 offset, unsigned long flags);
struct clk *tegra_clk_register_periph_nodiv(const char *name,
		const char **parent_names, int num_parents,
		struct tegra_clk_periph *periph, void __iomem *clk_base,
		u32 offset);

#define TEGRA_CLK_PERIPH(_mux_shift, _mux_mask, _mux_flags,		\
			 _div_shift, _div_width, _div_frac_width,	\
			 _div_flags, _clk_num,\
			 _gate_flags, _table, _lock)			\
	{								\
		.mux = {						\
			.flags = _mux_flags,				\
			.shift = _mux_shift,				\
			.mask = _mux_mask,				\
			.table = _table,				\
			.lock = _lock,					\
		},							\
		.divider = {						\
			.flags = _div_flags,				\
			.shift = _div_shift,				\
			.width = _div_width,				\
			.frac_width = _div_frac_width,			\
			.lock = _lock,					\
		},							\
		.gate = {						\
			.flags = _gate_flags,				\
			.clk_num = _clk_num,				\
		},							\
		.mux_ops = &clk_mux_ops,				\
		.div_ops = &tegra_clk_frac_div_ops,			\
		.gate_ops = &tegra_clk_periph_gate_ops,			\
	}

struct tegra_periph_init_data {
	const char *name;
	int clk_id;
	union {
		const char **parent_names;
		const char *parent_name;
	} p;
	int num_parents;
	struct tegra_clk_periph periph;
	u32 offset;
	const char *con_id;
	const char *dev_id;
	unsigned long flags;
};

#define TEGRA_INIT_DATA_TABLE(_name, _con_id, _dev_id, _parent_names, _offset,\
			_mux_shift, _mux_mask, _mux_flags, _div_shift,	\
			_div_width, _div_frac_width, _div_flags,	\
			_clk_num, _gate_flags, _clk_id, _table,		\
			_flags, _lock) \
	{								\
		.name = _name,						\
		.clk_id = _clk_id,					\
		.p.parent_names = _parent_names,			\
		.num_parents = ARRAY_SIZE(_parent_names),		\
		.periph = TEGRA_CLK_PERIPH(_mux_shift, _mux_mask,	\
					   _mux_flags, _div_shift,	\
					   _div_width, _div_frac_width,	\
					   _div_flags, _clk_num,	\
					   _gate_flags, _table, _lock),	\
		.offset = _offset,					\
		.con_id = _con_id,					\
		.dev_id = _dev_id,					\
		.flags = _flags						\
	}

#define TEGRA_INIT_DATA(_name, _con_id, _dev_id, _parent_names, _offset,\
			_mux_shift, _mux_width, _mux_flags, _div_shift,	\
			_div_width, _div_frac_width, _div_flags, \
			_clk_num, _gate_flags, _clk_id)	\
	TEGRA_INIT_DATA_TABLE(_name, _con_id, _dev_id, _parent_names, _offset,\
			_mux_shift, BIT(_mux_width) - 1, _mux_flags,	\
			_div_shift, _div_width, _div_frac_width, _div_flags, \
			_clk_num, _gate_flags, _clk_id,\
			NULL, 0, NULL)

/**
 * struct clk_super_mux - super clock
 *
 * @hw:		handle between common and hardware-specific interfaces
 * @reg:	register controlling multiplexer
 * @width:	width of the multiplexer bit field
 * @flags:	hardware-specific flags
 * @div2_index:	bit controlling divide-by-2
 * @pllx_index:	PLLX index in the parent list
 * @lock:	register lock
 *
 * Flags:
 * TEGRA_DIVIDER_2 - LP cluster has additional divider. This flag indicates
 *     that this is LP cluster clock.
 */
struct tegra_clk_super_mux {
	struct clk_hw	hw;
	void __iomem	*reg;
	struct tegra_clk_frac_div frac_div;
	const struct clk_ops	*div_ops;
	u8		width;
	u8		flags;
	u8		div2_index;
	u8		pllx_index;
	spinlock_t	*lock;
};

#define to_clk_super_mux(_hw) container_of(_hw, struct tegra_clk_super_mux, hw)

#define TEGRA_DIVIDER_2 BIT(0)

extern const struct clk_ops tegra_clk_super_ops;
struct clk *tegra_clk_register_super_mux(const char *name,
		const char **parent_names, u8 num_parents,
		unsigned long flags, void __iomem *reg, u8 clk_super_flags,
		u8 width, u8 pllx_index, u8 div2_index, spinlock_t *lock);
struct clk *tegra_clk_register_super_clk(const char *name,
		const char **parent_names, u8 num_parents,
		unsigned long flags, void __iomem *reg, u8 clk_super_flags,
		spinlock_t *lock);

/**
 * struct clk_init_table - clock initialization table
 * @clk_id:	clock id as mentioned in device tree bindings
 * @parent_id:	parent clock id as mentioned in device tree bindings
 * @rate:	rate to set
 * @state:	enable/disable
 * @flags	clock initialization flags
 */
struct tegra_clk_init_table {
	unsigned int	clk_id;
	unsigned int	parent_id;
	unsigned long	rate;
	int		state;
	u32		flags;
};

#define TEGRA_TABLE_RATE_CHANGE_OVERCLOCK	BIT(0)

/**
 * struct clk_duplicate - duplicate clocks
 * @clk_id:	clock id as mentioned in device tree bindings
 * @lookup:	duplicate lookup entry for the clock
 */
struct tegra_clk_duplicate {
	int			clk_id;
	struct clk_lookup	lookup;
};

#define TEGRA_CLK_DUPLICATE(_clk_id, _dev, _con) \
	{					\
		.clk_id = _clk_id,		\
		.lookup = {			\
			.dev_id = _dev,		\
			.con_id = _con,		\
		},				\
	}

struct tegra_clk {
	int			dt_id;
	bool			present;
	bool			use_integer_div;
};

struct tegra_devclk {
	int		dt_id;
	char		*dev_id;
	char		*con_id;
};

void tegra_init_special_resets(unsigned int num, int (*assert)(unsigned long),
			       int (*deassert)(unsigned long));

void tegra_init_from_table(struct tegra_clk_init_table *tbl,
		struct clk *clks[], int clk_max);

void tegra_init_dup_clks(struct tegra_clk_duplicate *dup_list,
		struct clk *clks[], int clk_max);

const struct tegra_clk_periph_regs *get_reg_bank(int clkid);
struct clk **tegra_clk_init(void __iomem *clk_base, int num, int periph_banks);

struct clk **tegra_lookup_dt_id(int clk_id, struct tegra_clk *tegra_clk);

void tegra_add_of_provider(struct device_node *np);
void tegra_register_devclks(struct tegra_devclk *dev_clks, int num);

void tegra_audio_clk_init(void __iomem *clk_base,
			void __iomem *pmc_base, struct tegra_clk *tegra_clks,
			struct tegra_audio_clk_info *audio_info,
			unsigned int num_plls, unsigned long sync_max_rate);

void tegra_periph_clk_init(void __iomem *clk_base, void __iomem *pmc_base,
			struct tegra_clk *tegra_clks,
			struct tegra_clk_pll_params *pll_params);

void tegra_pmc_clk_init(void __iomem *pmc_base, struct tegra_clk *tegra_clks);
void tegra_fixed_clk_init(struct tegra_clk *tegra_clks);
int tegra_osc_clk_init(void __iomem *clk_base, struct tegra_clk *clks,
		       unsigned long *input_freqs, unsigned int num,
		       unsigned int clk_m_div, unsigned long *osc_freq,
		       unsigned long *pll_ref_freq);
void tegra_super_clk_gen4_init(void __iomem *clk_base,
			void __iomem *pmc_base, struct tegra_clk *tegra_clks,
			struct tegra_clk_pll_params *pll_params);
void tegra_super_clk_gen5_init(void __iomem *clk_base,
			void __iomem *pmc_base, struct tegra_clk *tegra_clks,
			struct tegra_clk_pll_params *pll_params);
void tegra210b01_pll_init(void __iomem *car, void __iomem *pmc,
		unsigned long osc, unsigned long ref, bool emc_is_native,
		struct clk **clks);
void tegra210b01_audio_clk_init(void __iomem *clk_base,
				void __iomem *pmc_base,
				struct tegra_clk *tegra_clks);
void tegra210b01_super_clk_init(void __iomem *clk_base,
				void __iomem *pmc_base,
				struct tegra_clk *tegra_clks);
int tegra210b01_init_pllu(void);
struct tegra_clk_pll_params *tegra210b01_get_pllp_params(void);
struct tegra_clk_pll_params *tegra210b01_get_pllc4_params(void);
const struct clk_div_table *tegra210b01_get_pll_vco_post_div_table(void);
void tegra210b01_adjust_clks(struct tegra_clk *tegra_clks);
void tegra210b01_clock_table_init(struct clk **clks);

#ifdef CONFIG_TEGRA_CLK_EMC
struct clk *tegra_clk_register_emc(void __iomem *base, struct device_node *np,
				   spinlock_t *lock);
#else
static inline struct clk *tegra_clk_register_emc(void __iomem *base,
						 struct device_node *np,
						 spinlock_t *lock)
{
	return NULL;
}
#endif

/**
  * struct clk-emc - emc clock
  *
  * @hw:                handle between common and hardware-specific interfaces
  * @periph:    periph clock
  * @periph_ops:        periph clock ops
  * @emc_ops:   emc ops
  */
struct tegra_clk_emc {
        struct clk_hw                   hw;
        struct tegra_clk_periph         *periph;

        const struct clk_ops            *periph_ops;
        const struct emc_clk_ops        *emc_ops;
};

#define to_clk_emc(_hw) container_of(_hw, struct tegra_clk_emc, hw)

struct clk *tegra_clk_register_emc_t210(const char *name,
	const char **parent_names,
	int num_parents, struct tegra_clk_periph *periph,
	void __iomem *clk_base, u32 offset, unsigned long flags,
	const struct emc_clk_ops *emc_ops);

enum shared_bus_users_mode {
	SHARED_FLOOR = 0,
	SHARED_BW,
	SHARED_ISO_BW,
	SHARED_CEILING,
	SHARED_CEILING_BUT_ISO,
	SHARED_AUTO,
	SHARED_OVERRIDE,
};

#define TEGRA_CLK_SHARED_MAGIC	0x18ce213d

#define TEGRA_SHARED_BUS_RATE_LIMIT	BIT(0)
#define TEGRA_SHARED_BUS_RETENTION	BIT(1)
#define TEGRA_SHARED_BUS_RACE_TO_SLEEP	BIT(2)
#define TEGRA_SHARED_BUS_ROUND_PASS_THRU	BIT(3)
#define TEGRA_SHARED_BUS_EMC_NATIVE	BIT(4)

struct clk_div_sel {
	struct clk_hw *src;
	u32 div; /* stored as a 7.1 divider */
	unsigned long rate;
};

struct tegra_clk_cbus_shared {
	u32			magic;
	struct clk_hw		hw;
	struct list_head	shared_bus_list;
	struct clk		*shared_bus_backup;
	u32			flags;
	unsigned long		iso_usages;
	unsigned long		min_rate;
	unsigned long		max_rate;
	unsigned long		users_default_rate;
	bool			rate_update_started;
	bool			rate_updating;
	bool			rate_propagating;
	int			(*bus_update)(struct tegra_clk_cbus_shared *);
	struct clk_hw		*top_user;
	struct clk_hw		*slow_user;
	struct clk		*top_clk;
	unsigned long		override_rate;
	union {
		struct {
			struct clk_hw	*mux_clk;
			struct clk_hw	*div_clk;
			struct clk_hw	*pclk;
			struct clk_hw	*hclk;
			struct clk_hw	*sclk_low;
			struct clk_hw	*sclk_high;
			struct clk_hw	*apb_bus;
			struct clk_hw	*ahb_bus;
			unsigned long	threshold;
			int round_table_size;
			bool fallback;
			struct clk_div_sel *round_table;
		} system;
		struct {
			struct list_head	node;
			bool			enabled;
			unsigned long		rate;
			struct clk		*client;
			u32			client_div;
			enum shared_bus_users_mode mode;
			struct clk		*inputs[2];
		} shared_bus_user;
	} u;
};

#define to_clk_cbus_shared(_hw) \
			container_of(_hw, struct tegra_clk_cbus_shared, hw)

struct clk *tegra_clk_register_shared(const char *name,
		const char **parent, u8 num_parents, unsigned long flags,
		unsigned long usages, enum shared_bus_users_mode mode,
		const char *client);
struct clk *tegra_clk_register_cbus(const char *name,
		const char *parent, unsigned long flags,
		const char *backup, unsigned long min_rate,
		unsigned long max_rate);
struct clk *tegra_clk_register_gbus(const char *name,
		const char *parent, unsigned long flags,
		unsigned long min_rate, unsigned long max_rate);
struct clk *tegra_clk_register_shared_master(const char *name,
		const char *parent, unsigned long flags,
		unsigned long min_rate, unsigned long max_rate);
struct clk *tegra_clk_register_sbus_cmplx(const char *name,
		const char *parent, const char *mux_clk, const char *div_clk,
		unsigned long flags, const char *pclk, const char *hclk,
		const char *sclk_low, const char *sclk_high,
		unsigned long threshold, unsigned long min_rate,
		unsigned long max_rate);
void tegra_shared_clk_init(struct tegra_clk *tegra_clks);
struct clk *tegra_clk_register_cascade_master(const char *name,
		const char *parent, const char *topclkname,
		unsigned long flags);
struct clk *tegra_clk_register_shared_connect(const char *name,
		const char **parent, u8 num_parents, unsigned long flags,
		unsigned long usages, enum shared_bus_users_mode mode,
		const char *client);

struct tegra_clk_skipper {
	struct clk_hw hw;
	void __iomem *reg;
	spinlock_t *lock;
};
#define to_clk_skipper(_hw) container_of(_hw, struct tegra_clk_skipper, hw)

struct clk *tegra_clk_register_skipper(const char *name,
		const char *parent_name, void __iomem *reg,
		unsigned long flags, spinlock_t *lock);

void tegra114_clock_tune_cpu_trimmers_high(void);
void tegra114_clock_tune_cpu_trimmers_low(void);
void tegra114_clock_tune_cpu_trimmers_init(void);
void tegra114_clock_assert_dfll_dvco_reset(void);
void tegra114_clock_deassert_dfll_dvco_reset(void);

typedef void (*tegra_clk_apply_init_table_func)(void);
extern tegra_clk_apply_init_table_func tegra_clk_apply_init_table;
int tegra_pll_wait_for_lock(struct tegra_clk_pll *pll);
u16 tegra_pll_get_fixed_mdiv(struct clk_hw *hw, unsigned long input_rate);
int tegra_pll_p_div_to_hw(struct tegra_clk_pll *pll, u8 p_div);
unsigned long tegra_pll_adjust_vco_min_sdm(struct tegra_clk_pll_params *params,
				unsigned long parent_rate, u32 sdm_coeff);

#ifdef CONFIG_PM_SLEEP
void tegra_clk_pll_resume(struct clk *c, unsigned long rate);
void tegra_clk_pllcx_resume(struct clk *c, unsigned long rate);
void tegra_clk_pllxc_resume(struct clk *c, unsigned long rate);
void tegra_clk_pllre_vco_resume(struct clk *c, unsigned long rate);
void tegra_clk_pllu_resume(struct clk *c, unsigned long rate);
void tegra_clk_pllss_resume(struct clk *c, unsigned long rate);
void tegra_clk_divider_resume(struct clk_hw *hw, unsigned long rate);
void tegra_clk_pll_out_resume(struct clk *clk, unsigned long rate);
void tegra_clk_plle_tegra210_resume(struct clk *c);
void tegra_clk_sync_state_pllcx(struct clk *c);
void tegra_clk_sync_state_iddq(struct clk *c);
void tegra_clk_sync_state_pll(struct clk *c);
void tegra_clk_sync_state_pll_out(struct clk *clk);
void tegra_clk_periph_suspend(void __iomem *clk_base);
void tegra_clk_periph_resume(void __iomem *clk_base);
void tegra_clk_periph_force_on(u32 *clks_on, int count, void __iomem *clk_base);
void tegra_clk_osc_resume(void __iomem *clk_base);
#endif

int div71_get(unsigned long rate, unsigned parent_rate, u8 width,
	      u8 frac_width, u8 flags);

struct tegra_pto_table {
	int clk_id;
	int divider;
	u32 pto_id;
	u32 presel_value;
	u32 presel_reg;
	u32 presel_mask;
	u8 cycle_count;
};
void tegra_register_ptos(struct tegra_pto_table *ptodefs, int num_pto_defs);
void tegra_register_pto(struct clk *clk, struct tegra_pto_table *ptodef);

/* add some extra nodes to debugfs */
void tegra_clk_debugfs_add(struct clk *clk);

/* Combined read fence with delay */
#define fence_udelay(delay, reg)	\
	do {				\
		readl_relaxed(reg);	\
		udelay(delay);		\
	} while (0)

#endif /* TEGRA_CLK_H */
