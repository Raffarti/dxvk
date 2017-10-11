#include "dxvk_vulkan_loader.h"

namespace dxvk::vk {
  
  PFN_vkVoidFunction LibraryLoader::sym(const char* name) const {
    return ::vkGetInstanceProcAddr(nullptr, name);
  }
  
  
  InstanceLoader::InstanceLoader(VkInstance instance)
  : m_instance(instance) { }
  
  
  PFN_vkVoidFunction InstanceLoader::sym(const char* name) const {
    return ::vkGetInstanceProcAddr(m_instance, name);
  }
  
  
  DeviceLoader::DeviceLoader(VkInstance instance, VkDevice device)
  : m_getDeviceProcAddr(reinterpret_cast<PFN_vkGetDeviceProcAddr>(
      ::vkGetInstanceProcAddr(instance, "vkGetDeviceProcAddr"))),
    m_device(device) { }
  
  
  PFN_vkVoidFunction DeviceLoader::sym(const char* name) const {
    return m_getDeviceProcAddr(m_device, name);
  }
  
  
  LibraryFn::LibraryFn() { }
  LibraryFn::~LibraryFn() { }
  
  
  InstanceFn::InstanceFn(VkInstance instance)
  : InstanceLoader(instance) { }
  InstanceFn::~InstanceFn() { }
  
  
  DeviceFn::DeviceFn(VkInstance instance, VkDevice device)
  : DeviceLoader(instance, device) { }
  DeviceFn::~DeviceFn() { }
  
}