/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/*
 * Copyright(c) 2021-2023 Intel Corporation
 */
#ifndef __iwl_fw_uefi__
#define __iwl_fw_uefi__

#define IWL_UEFI_OEM_PNVM_NAME		L"UefiCnvWlanOemSignedPnvm"
#define IWL_UEFI_REDUCED_POWER_NAME	L"UefiCnvWlanReducedPower"
#define IWL_UEFI_SGOM_NAME		L"UefiCnvWlanSarGeoOffsetMapping"
#define IWL_UEFI_STEP_NAME		L"UefiCnvCommonSTEP"

/*
 * TODO: we have these hardcoded values that the caller must pass,
 * because reading from the UEFI is not working.  To implement this
 * properly, we have to change iwl_pnvm_get_from_uefi() to call
 * efivar_entry_size() and return the value to the caller instead.
 */
#define IWL_HARDCODED_PNVM_SIZE		4096
#define IWL_HARDCODED_REDUCE_POWER_SIZE	32768
#define IWL_HARDCODED_SGOM_SIZE		339
#define IWL_HARDCODED_STEP_SIZE		6

struct pnvm_sku_package {
	u8 rev;
	u32 total_size;
	u8 n_skus;
	u32 reserved[2];
	u8 data[];
} __packed;

struct uefi_cnv_wlan_sgom_data {
	u8 revision;
	u8 offset_map[IWL_HARDCODED_SGOM_SIZE - 1];
} __packed;

struct uefi_cnv_common_step_data {
	u8 revision;
	u8 step_mode;
	u8 cnvi_eq_channel;
	u8 cnvr_eq_channel;
	u8 radio1;
	u8 radio2;
} __packed;

/*
 * This is known to be broken on v4.19 and to work on v5.4.  Until we
 * figure out why this is the case and how to make it work, simply
 * disable the feature in old kernels.
 */
#if defined(CONFIG_EFI) && LINUX_VERSION_IS_GEQ(5,4,0)
void *iwl_uefi_get_pnvm(struct iwl_trans *trans, size_t *len);
u8 *iwl_uefi_get_reduced_power(struct iwl_trans *trans, size_t *len);
int iwl_uefi_reduce_power_parse(struct iwl_trans *trans,
				const u8 *data, size_t len,
				struct iwl_pnvm_image *pnvm_data);
void iwl_uefi_get_step_table(struct iwl_trans *trans);
#else /* CONFIG_EFI */
static inline void *iwl_uefi_get_pnvm(struct iwl_trans *trans, size_t *len)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline int
iwl_uefi_reduce_power_parse(struct iwl_trans *trans,
			    const u8 *data, size_t len,
			    struct iwl_pnvm_image *pnvm_data)
{
	return -EOPNOTSUPP;
}

static inline u8 *
iwl_uefi_get_reduced_power(struct iwl_trans *trans, size_t *len)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline void iwl_uefi_get_step_table(struct iwl_trans *trans)
{
}
#endif /* CONFIG_EFI */

#if defined(CONFIG_EFI) && defined(CONFIG_ACPI) && LINUX_VERSION_IS_GEQ(5,4,0)
void iwl_uefi_get_sgom_table(struct iwl_trans *trans, struct iwl_fw_runtime *fwrt);
#else
static inline
void iwl_uefi_get_sgom_table(struct iwl_trans *trans, struct iwl_fw_runtime *fwrt)
{
}
#endif
#endif /* __iwl_fw_uefi__ */
