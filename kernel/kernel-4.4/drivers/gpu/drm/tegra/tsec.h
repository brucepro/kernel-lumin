/*
 * Copyright (C) 2016 NVIDIA CORPORATION.  All rights reserved.
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

#ifndef TEGRA_TSEC_H
#define TEGRA_TSEC_H

#include "drm.h"
#include "falcon.h"

struct tsec_config {
	/* Firmware name */
	const char *ucode_name;
	const struct tegra_drm_client_ops *drm_client_ops;

	u32 class_id;
};

struct tsec {
	struct falcon falcon;
	bool booted;

	void __iomem *regs;
	struct tegra_drm_client client;
	struct host1x_channel *channel;
	struct iommu_domain *domain;
	struct device *dev;
	struct clk *clk;
	struct reset_control *rst;

	/* Platform configuration */
	const struct tsec_config *config;

	/* for firewall - this determines if method 1 should be regarded
	 * as an address register */
	bool method_data_is_addr_reg;
};

static inline struct tsec *to_tsec(struct tegra_drm_client *client)
{
	return container_of(client, struct tsec, client);
}

static void __maybe_unused tsec_writel(struct tsec *tsec, u32 value, unsigned int offset)
{
	writel(value, tsec->regs + offset);
}

int tsec_open_channel(struct tegra_drm_client *client,
			    struct tegra_drm_context *context);
void tsec_close_channel(struct tegra_drm_context *context);

#endif /* TEGRA_TSEC_H */
