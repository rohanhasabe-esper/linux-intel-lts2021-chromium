/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef __I915_SYSFS_H__
#define __I915_SYSFS_H__

struct device;
struct drm_i915_private;

struct drm_i915_private *kdev_minor_to_i915(struct device *kdev);

void i915_setup_sysfs(struct drm_i915_private *i915);
void i915_teardown_sysfs(struct drm_i915_private *i915);

#if IS_ENABLED(CONFIG_DRM_I915_MEMTRACK)
int i915_gem_create_sysfs_file_entry(struct drm_device *dev,
				     struct drm_file *file);
void i915_gem_remove_sysfs_file_entry(struct drm_device *dev,
				      struct drm_file *file);
#endif

#endif /* __I915_SYSFS_H__ */
